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

#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include <memory>
#include "common/rs_rect.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#include "GLES3/gl32.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkSurface.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrContext.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "memory_handler.h"
#ifndef ROSEN_CROSS_PLATFORM
#include "surface_type.h"
#endif

#define GLES_VERSION 2
#define GET_MAX_EGL_CONFIG 5

namespace OHOS {
namespace Rosen {
class RenderContext {
public:
    RenderContext();
    virtual ~RenderContext();
    void CreateCanvas(int width, int height);
    sk_sp<SkSurface> AcquireSurface(int width, int height);

#ifndef ROSEN_CROSS_PLATFORM
    void SetColorSpace(ColorGamut colorSpace);
#endif
    void InitializeEglContext();

    GrContext* GetGrContext() const
    {
        return grContext_.get();
    }

    sk_sp<SkSurface> GetSurface() const
    {
        return skSurface_;
    }

    bool SetUpGrContext();

    EGLSurface CreateEGLSurface(EGLNativeWindowType eglNativeWindow);
    void DestroyEGLSurface(EGLSurface surface);
    void MakeCurrent(EGLSurface surface, EGLContext context = EGL_NO_CONTEXT);
    void SwapBuffers(EGLSurface surface) const;
    void RenderFrame();
    EGLint QueryEglBufferAge();
    void DamageFrame(int32_t left, int32_t top, int32_t width, int32_t height);
    void DamageFrame(const std::vector<RectI> &rects);
    void ClearRedundantResources();
    void CreatePbufferSurface();

    EGLSurface GetEGLSurface() const
    {
        return eglSurface_;
    }

    EGLContext GetEGLContext() const
    {
        return eglContext_;
    }

    EGLDisplay GetEGLDisplay() const
    {
        return eglDisplay_;
    }

    bool IsEglContextReady() const
    {
        return eglContext_ != EGL_NO_DISPLAY;
    }

    void SetCacheDir(const std::string& filePath)
    {
        cacheDir_ = filePath;
    }

    void SetUniRenderMode(bool isUni)
    {
        isUniRenderMode_ = isUni;
    }

private:
    sk_sp<GrContext> grContext_;
    sk_sp<SkSurface> skSurface_;

    EGLNativeWindowType nativeWindow_;

    EGLDisplay eglDisplay_ = EGL_NO_DISPLAY;
    EGLContext eglContext_ = EGL_NO_CONTEXT;
    EGLSurface eglSurface_ = EGL_NO_SURFACE;
    EGLSurface pbufferSurface_= EGL_NO_SURFACE;
    EGLConfig config_ = nullptr;
#ifndef ROSEN_CROSS_PLATFORM
    ColorGamut colorSpace_ = ColorGamut::COLOR_GAMUT_SRGB;
#endif

    bool isUniRenderMode_ = false;
    const std::string UNIRENDER_CACHE_DIR = "/data/service/el0/render_service";
    std::string cacheDir_;
    std::shared_ptr<MemoryHandler> mHandler_;
};

class RenderContextFactory {
public:
    static RenderContextFactory& GetInstance();

    ~RenderContextFactory()
    {
        if (context_ != nullptr) {
            delete context_;
        }
        context_ = nullptr;
    }

    RenderContext* CreateEngine()
    {
        if (context_ == nullptr) {
            context_ = new RenderContext();
        }

        return context_;
    }

private:
    RenderContextFactory() : context_(nullptr) {}
    RenderContextFactory(const RenderContextFactory&) = delete;
    RenderContextFactory& operator=(const RenderContextFactory&) = delete;

    RenderContext* context_;
};
} // namespace Rosen
} // namespace OHOS
#endif
