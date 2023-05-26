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

#include "egl_consumer_surface.h"

#include "buffer_log.h"

#include "buffer_queue_producer.h"
namespace OHOS {
EglConsumerSurface::EglConsumerSurface(const std::string &name, bool isShared)
    : ConsumerSurface(name, isShared)
{
    BLOGD("ctor");
}

EglConsumerSurface::~EglConsumerSurface()
{
    BLOGD("dtor");
}

GSError EglConsumerSurface::Init()
{
    auto ret = ConsumerSurface::Init();
    if (ret) {
        BLOGE("ConsumerSurface::Init failed with %{public}d", ret);
        return ret;
    }

    return GSERROR_OK;
}

GSError EglConsumerSurface::AcquireBuffer(sptr<SurfaceBuffer>& buffer, int32_t &fence,
    int64_t &timestamp, Rect &damage)
{
    auto ret = ConsumerSurface::AcquireBuffer(buffer, fence, timestamp, damage);
    if (ret) {
        return ret;
    }

    auto eglData = buffer->GetEglData();
    if (eglData == nullptr) {
        auto eglDataImpl = new EglDataImpl();
        auto res = eglDataImpl->CreateEglData(buffer);
        if (res) {
            BLOGE("EglDataImpl::CreateEglData failed with %{public}d", res);
            return GSERROR_INTERNAL;
        } else {
            eglData = eglDataImpl;
            buffer->SetEglData(eglData);
        }
    }
    EglManager::GetInstance()->EglMakeCurrent();
    return GSERROR_OK;
}
} // namespace OHOS
