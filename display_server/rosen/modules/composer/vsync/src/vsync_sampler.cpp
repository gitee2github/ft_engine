/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "vsync_sampler.h"
#include <cmath>
#include "vsync_generator.h"
#include "vsync_log.h"

namespace OHOS {
namespace Rosen {
namespace impl {
std::once_flag VSyncSampler::createFlag_;
sptr<OHOS::Rosen::VSyncSampler> VSyncSampler::instance_ = nullptr;

namespace {
constexpr double PI = 3.1415926;
constexpr int64_t g_errorThreshold = 40000000000; // 200 usec squared
constexpr int32_t INVAILD_TIMESTAMP = -1;
constexpr uint32_t MINES_SAMPLE_NUMS = 3;
}
sptr<OHOS::Rosen::VSyncSampler> VSyncSampler::GetInstance() noexcept
{
    std::call_once(createFlag_, []() {
        auto vsyncSampler = new VSyncSampler();
        instance_ = vsyncSampler;
    });

    return instance_;
}

VSyncSampler::VSyncSampler()
    : period_(0), phase_(0), referenceTime_(0),
    error_(0), firstSampleIndex_(0), numSamples_(0),
    modeUpdated_(false)
{
}

void VSyncSampler::Reset()
{
    std::lock_guard<std::mutex> lock(mutex_);
    period_ = 0;
    phase_ = 0;
    referenceTime_ = 0;
    error_ = 0;
    firstSampleIndex_ = 0;
    numSamples_ = 0;
    modeUpdated_ = false;
    hardwareVSyncStatus_ = true;
}

void VSyncSampler::ResetErrorLocked()
{
    presentFenceTimeOffset_ = 0;
    error_ = 0;
    for (uint32_t i = 0; i < NUM_PRESENT; i++) {
        presentFenceTime_[i] = INVAILD_TIMESTAMP;
    }
}

void VSyncSampler::BeginSample()
{
    std::lock_guard<std::mutex> lock(mutex_);
    numSamples_ = 0;
    modeUpdated_ = false;
    hardwareVSyncStatus_ = true;
}

void VSyncSampler::SetHardwareVSyncStatus(bool enabled)
{
    std::lock_guard<std::mutex> lock(mutex_);
    hardwareVSyncStatus_ = enabled;
}

bool VSyncSampler::GetHardwareVSyncStatus() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return hardwareVSyncStatus_;
}

void VSyncSampler::RegSetScreenVsyncEnabledCallback(VSyncSampler::SetScreenVsyncEnabledCallback cb)
{
    setScreenVsyncEnabledCallback_ = cb;
}

void VSyncSampler::SetScreenVsyncEnabledInRSMainThread(bool enabled)
{
    if (setScreenVsyncEnabledCallback_ == nullptr) {
        VLOGE("SetScreenVsyncEnabled:%{public}d failed, setScreenVsyncEnabledCallback_ is null", enabled);
        return;
    }
    setScreenVsyncEnabledCallback_(enabled);
}

bool VSyncSampler::AddSample(int64_t timeStamp)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (numSamples_ == 0) {
        phase_ = 0;
        referenceTime_ = timeStamp;
        CreateVSyncGenerator()->UpdateMode(period_, phase_, referenceTime_);
    }

    if (numSamples_ < MAX_SAMPLES - 1) {
        numSamples_++;
    } else {
        firstSampleIndex_ = (firstSampleIndex_ + 1) % MAX_SAMPLES;
    }

    uint32_t index = (firstSampleIndex_ + numSamples_ - 1) % MAX_SAMPLES;
    samples_[index] = timeStamp;

    UpdateModeLocked();

    if (numResyncSamplesSincePresent_++ > MAX_SAMPLES_WITHOUT_PRESENT) {
        ResetErrorLocked();
    }

    // 1/2 just a empirical value
    bool shouldDisableScreenVsync = modeUpdated_ && (error_ < g_errorThreshold / 2);

    if (shouldDisableScreenVsync) {
        // disabled screen vsync in rsMainThread
        VLOGD("Disable Screen Vsync");
        SetScreenVsyncEnabledInRSMainThread(false);
    }

    return !shouldDisableScreenVsync;
}


void VSyncSampler::UpdateModeLocked()
{
    if (numSamples_ >= MIN_SAMPLES_FOR_UPDATE) {
        int64_t sum = 0;
        int64_t min = INT64_MAX;
        int64_t max = 0;
        for (uint32_t i = 1; i < numSamples_; i++) {
            int64_t prevSample = samples_[(firstSampleIndex_ + i - 1 + MAX_SAMPLES) % MAX_SAMPLES];
            int64_t currentSample = samples_[(firstSampleIndex_ + i) % MAX_SAMPLES];
            int64_t diff = currentSample - prevSample;
            min = min < diff ? min : diff;
            max = max > diff ? max : diff;
            sum += diff;
        }
        sum -= min;
        sum -= max;

        period_ = sum / (int64_t)(numSamples_ - MINES_SAMPLE_NUMS);

        double scale = 2.0 * PI / period_;
        double deltaAvgX = 0;
        double deltaAvgY = 0;
        for (uint32_t i = 1; i < numSamples_; i++) {
            double delta = (samples_[(firstSampleIndex_ + i) % MAX_SAMPLES] - referenceTime_) % period_ * scale;
            deltaAvgX += cos(delta);
            deltaAvgY += sin(delta);
        }

        deltaAvgX /= double(numSamples_ - 1);
        deltaAvgY /= double(numSamples_ - 1);

        phase_ = int64_t(::atan2(deltaAvgY, deltaAvgX) / scale);

        modeUpdated_ = true;
        CreateVSyncGenerator()->UpdateMode(period_, phase_, referenceTime_);
    }
}

void VSyncSampler::UpdateErrorLocked()
{
    if (!modeUpdated_) {
        return;
    }

    int numErrSamples = 0;
    int64_t sqErrSum = 0;

    for (uint32_t i = 0; i < NUM_PRESENT; i++) {
        int64_t t = presentFenceTime_[i];
        if (t <= 0) {
            continue;
        }

        int64_t sample = t - referenceTime_;
        if (sample <= phase_) {
            continue;
        }

        int64_t sampleErr = (sample - phase_) % period_;
        // 1/2 just a empirical value
        if (sampleErr > period_ / 2) {
            sampleErr -= period_;
        }
        sqErrSum += sampleErr * sampleErr;
        numErrSamples++;
    }

    if (numErrSamples > 0) {
        error_ = sqErrSum / numErrSamples;
    } else {
        error_ = 0;
    }
}

bool VSyncSampler::AddPresentFenceTime(int64_t timestamp)
{
    std::lock_guard<std::mutex> lock(mutex_);
    presentFenceTime_[presentFenceTimeOffset_] = timestamp;
    presentFenceTimeOffset_ = (presentFenceTimeOffset_ + 1) % NUM_PRESENT;
    numResyncSamplesSincePresent_ = 0;

    UpdateErrorLocked();

    return !modeUpdated_ || error_ > g_errorThreshold;
}

int64_t VSyncSampler::GetPeriod() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return period_;
}

int64_t VSyncSampler::GetPhase() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return phase_;
}

int64_t VSyncSampler::GetRefrenceTime() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return referenceTime_;
}

VSyncSampler::~VSyncSampler()
{
}
} // namespace impl

sptr<VSyncSampler> CreateVSyncSampler()
{
    return impl::VSyncSampler::GetInstance();
}
}
}
