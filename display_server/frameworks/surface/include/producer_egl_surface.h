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

#ifndef FRAMEWORKS_SURFACE_INCLUDE_PRODUCER_EGL_SURFACE_H
#define FRAMEWORKS_SURFACE_INCLUDE_PRODUCER_EGL_SURFACE_H

#include <map>
#include <string>

#include <egl_surface.h>
#include <ibuffer_producer.h>

#include "surface_buffer.h"

namespace OHOS {
class ProducerEglSurface : public EglSurface {
public:
    ProducerEglSurface(sptr<IBufferProducer>& producer);
    virtual ~ProducerEglSurface();

    GSError InitContext(EGLContext context = EGL_NO_CONTEXT) override;
    EGLDisplay GetEglDisplay() const override;
    EGLContext GetEglContext() const override;
    EGLSurface GetEglSurface() const override;
    GLuint GetEglFbo() const override;
    GSError SwapBuffers() override;
    GSError SetWidthAndHeight(int32_t width, int32_t height) override;

private:
    bool IsRemote();
    GSError RequestBuffer(sptr<SurfaceBuffer> &buffer, int32_t &fence, BufferRequestConfig &config);

    GSError RequestBufferProc();
    GSError FlushBufferProc();
    GSError FlushBuffer(sptr<SurfaceBuffer> &buffer, int32_t fence, BufferFlushConfig &config);
    GSError AddEglData(sptr<SurfaceBuffer> &buffer);
    GSError CreateEglFenceFd(int32_t &fd);
    GSError WaitForReleaseFence(int32_t fd);

    std::map<int32_t, sptr<SurfaceBuffer>> bufferProducerCache_;
    sptr<IBufferProducer> producer_ = nullptr;
    std::string name_ = "not init";
    bool initFlag_ = false;
    sptr<EglManager> sEglManager_ = nullptr;
    sptr<SurfaceBuffer> currentBuffer_ = nullptr;
    std::mutex mutex_;
    int32_t width_;
    int32_t height_;
};
} // namespace OHOS

#endif // FRAMEWORKS_SURFACE_INCLUDE_PRODUCER_EGL_SURFACE_H
