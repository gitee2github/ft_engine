/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#ifndef RENDER_SERVICE_CLIENT_CORE_ANIMATION_RS_VALUE_ESTIMATOR_H
#define RENDER_SERVICE_CLIENT_CORE_ANIMATION_RS_VALUE_ESTIMATOR_H

#include <memory>

#include "animation/rs_animation_common.h"
#include "animation/rs_interpolator.h"
#include "common/rs_color.h"
#include "common/rs_macros.h"
#include "common/rs_matrix3.h"
#include "common/rs_vector2.h"
#include "common/rs_vector4.h"
#include "modifier/rs_modifier_type.h"
#include "render/rs_filter.h"

namespace OHOS {
namespace Rosen {
class RSRenderPropertyBase;
template<typename T>
class RSRenderAnimatableProperty;

class RSB_EXPORT RSValueEstimator {
public:
    template<typename T>
    T Estimate(float fraction, const T& startValue, const T& endValue)
    {
        return startValue * (1.0f - fraction) + endValue * fraction;
    }

    Quaternion Estimate(float fraction, const Quaternion& startValue, const Quaternion& endValue);

    std::shared_ptr<RSFilter> Estimate(
        float fraction, const std::shared_ptr<RSFilter>& startValue, const std::shared_ptr<RSFilter>& endValue);

    virtual float EstimateFraction(const std::shared_ptr<RSInterpolator>& interpolator)
    {
        return 0.0f;
    }

    virtual void InitCurveAnimationValue(const std::shared_ptr<RSRenderPropertyBase>& property,
        const std::shared_ptr<RSRenderPropertyBase>& startValue,
        const std::shared_ptr<RSRenderPropertyBase>& endValue,
        const std::shared_ptr<RSRenderPropertyBase>& lastValue) {}

    virtual void InitKeyframeAnimationValue(const std::shared_ptr<RSRenderPropertyBase>& property,
        std::vector<std::tuple<float, std::shared_ptr<RSRenderPropertyBase>,
        std::shared_ptr<RSInterpolator>>>& keyframes,
        const std::shared_ptr<RSRenderPropertyBase>& lastValue) {}

    virtual void UpdateAnimationValue(const float fraction, const bool isAdditive) = 0;
};

template<typename T>
class RSB_EXPORT_TMP RSCurveValueEstimator : public RSValueEstimator {
public:
    RSCurveValueEstimator() = default;
    virtual ~RSCurveValueEstimator() = default;

    void InitCurveAnimationValue(const std::shared_ptr<RSRenderPropertyBase>& property,
        const std::shared_ptr<RSRenderPropertyBase>& startValue,
        const std::shared_ptr<RSRenderPropertyBase>& endValue,
        const std::shared_ptr<RSRenderPropertyBase>& lastValue) override
    {
        auto animatableProperty = std::static_pointer_cast<RSRenderAnimatableProperty<T>>(property);
        auto animatableStartValue = std::static_pointer_cast<RSRenderAnimatableProperty<T>>(startValue);
        auto animatableEndValue = std::static_pointer_cast<RSRenderAnimatableProperty<T>>(endValue);
        auto animatableLastValue = std::static_pointer_cast<RSRenderAnimatableProperty<T>>(lastValue);
        if (animatableProperty && animatableStartValue && animatableEndValue && animatableLastValue) {
            property_ = animatableProperty;
            startValue_ = animatableStartValue->Get();
            endValue_ = animatableEndValue->Get();
            lastValue_ = animatableLastValue->Get();
        }
    }

    void UpdateAnimationValue(const float fraction, const bool isAdditive) override
    {
        auto animationValue = GetAnimationValue(fraction, isAdditive);
        if (property_ != nullptr) {
            property_->Set(animationValue);
        }
    }

    T GetAnimationValue(const float fraction, const bool isAdditive)
    {
        auto interpolationValue = RSValueEstimator::Estimate(fraction, startValue_, endValue_);
        auto animationValue = interpolationValue;
        if (isAdditive && property_ != nullptr) {
            animationValue = property_->Get() + interpolationValue - lastValue_;
        }
        lastValue_ = interpolationValue;
        return animationValue;
    }

    float EstimateFraction(const std::shared_ptr<RSInterpolator>& interpolator) override
    {
        return 0.0f;
    }

private:
    T startValue_ {};
    T endValue_ {};
    T lastValue_ {};
    std::shared_ptr<RSRenderAnimatableProperty<T>> property_;
};

template<>
float RSCurveValueEstimator<float>::EstimateFraction(const std::shared_ptr<RSInterpolator>& interpolator);

extern template class RSCurveValueEstimator<float>;

template<typename T>
class RSKeyframeValueEstimator : public RSValueEstimator {
public:
    RSKeyframeValueEstimator() = default;
    virtual ~RSKeyframeValueEstimator() = default;

    void InitKeyframeAnimationValue(const std::shared_ptr<RSRenderPropertyBase>& property,
        std::vector<std::tuple<float, std::shared_ptr<RSRenderPropertyBase>,
        std::shared_ptr<RSInterpolator>>>& keyframes,
        const std::shared_ptr<RSRenderPropertyBase>& lastValue) override
    {
        auto animatableProperty = std::static_pointer_cast<RSRenderAnimatableProperty<T>>(property);
        auto animatableLastValue = std::static_pointer_cast<RSRenderAnimatableProperty<T>>(lastValue);
        if (animatableProperty && animatableLastValue) {
            property_ = animatableProperty;
            lastValue_ = animatableLastValue->Get();
        }
        for (const auto& keyframe : keyframes) {
            auto keyframeValue = std::static_pointer_cast<RSRenderAnimatableProperty<T>>(std::get<1>(keyframe));
            if (keyframeValue != nullptr) {
                keyframes_.push_back({ std::get<0>(keyframe), keyframeValue->Get(), std::get<2>(keyframe) });
            }
        }
    }

    void UpdateAnimationValue(const float fraction, const bool isAdditive) override
    {
        auto animationValue = GetAnimationValue(fraction, isAdditive);
        if (property_ != nullptr) {
            property_->Set(animationValue);
        }
    }

    T GetAnimationValue(const float fraction, const bool isAdditive)
    {
        float preKeyframeFraction = std::get<0>(keyframes_.front());
        auto preKeyframeValue = std::get<1>(keyframes_.front());
        for (const auto& keyframe : keyframes_) {
            // the index of tuple
            float keyframeFraction = std::get<0>(keyframe);
            auto keyframeValue = std::get<1>(keyframe);
            auto keyframeInterpolator = std::get<2>(keyframe);
            if (fraction <= keyframeFraction) {
                if (ROSEN_EQ(keyframeFraction, preKeyframeFraction)) {
                    continue;
                }

                float intervalFraction = (fraction - preKeyframeFraction) / (keyframeFraction - preKeyframeFraction);
                auto interpolationValue = RSValueEstimator::Estimate(
                    keyframeInterpolator->Interpolate(intervalFraction), preKeyframeValue, keyframeValue);
                auto animationValue = interpolationValue;
                if (isAdditive && property_ != nullptr) {
                    animationValue = property_->Get() + interpolationValue - lastValue_;
                }
                lastValue_ = interpolationValue;
                return animationValue;
            }

            preKeyframeFraction = keyframeFraction;
            preKeyframeValue = keyframeValue;
        }
        return preKeyframeValue;
    }

private:
    std::vector<std::tuple<float, T, std::shared_ptr<RSInterpolator>>> keyframes_;
    T lastValue_ {};
    std::shared_ptr<RSRenderAnimatableProperty<T>> property_;
};

enum class RSValueEstimatorType : int16_t {
    INVALID = 0,
    CURVE_VALUE_ESTIMATOR,
    KEYFRAME_VALUE_ESTIMATOR,
    PATH_VALUE_ESTIMATOR,
};
} // namespace Rosen
} // namespace OHOS

#endif // RENDER_SERVICE_CLIENT_CORE_ANIMATION_RS_VALUE_ESTIMATOR_H
