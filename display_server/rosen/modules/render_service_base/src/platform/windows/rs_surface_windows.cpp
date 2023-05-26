/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "rs_surface_windows.h"

#include <glfw_render_context.h>
#include <include/core/SkColorSpace.h>
#include <include/gpu/gl/GrGLInterface.h>

#include "platform/common/rs_log.h"
#include "rs_surface_frame_windows.h"

namespace OHOS {
namespace Rosen {
RSSurfaceWindows::RSSurfaceWindows(UseSurfaceToRenderFunc onRender)
    : onRender_(onRender)
{
}

bool RSSurfaceWindows::IsValid() const
{
    return onRender_ != nullptr;
}

void RSSurfaceWindows::SetUiTimeStamp(const std::unique_ptr<RSSurfaceFrame>& frame, uint64_t uiTimestamp)
{
}

std::unique_ptr<RSSurfaceFrame> RSSurfaceWindows::RequestFrame(
    int32_t width, int32_t height, uint64_t uiTimestamp, bool useAFBC)
{
    if (onRender_ == nullptr) {
        ROSEN_LOGE("RSSurfaceWindows::RequestFrame, producer is nullptr");
        return nullptr;
    }

    auto frame = std::make_unique<RSSurfaceFrameWindows>(width, height);
    if (SetupGrContext() == false) {
        return frame;
    }

    constexpr auto colorType = kRGBA_8888_SkColorType;
    SkSurfaceProps surfaceProps = SkSurfaceProps::kLegacyFontHost_InitType;
    constexpr uint32_t format = 0x8058; // GL_RGBA8
    GrBackendRenderTarget backendRenderTarget(
        frame->width_, frame->height_, 0, 8, {.fFBOID = 0, .fFormat = format});
    frame->surface_ = SkSurface::MakeFromBackendRenderTarget(grContext_.get(),
                                                             backendRenderTarget,
                                                             kTopLeft_GrSurfaceOrigin,
                                                             colorType, skColorSpace_, &surfaceProps);
#ifdef USE_GLFW_WINDOW
    if (frame->surface_ != nullptr) {
        const auto &canvas = frame->surface_->getCanvas();
        if (canvas != nullptr) {
            canvas->translate(0, frame->height_);
            canvas->scale(1, -1);
        }
    }
#endif

    return frame;
}

bool RSSurfaceWindows::FlushFrame(std::unique_ptr<RSSurfaceFrame>& frame, uint64_t uiTimestamp)
{
    if (frame == nullptr) {
        ROSEN_LOGE("RSSurfaceFrame::FlushFrame frame is nullptr");
        return false;
    }

    // RSSurfaceWindows is the class for platform Windows,
    // the input pointer should be the pointer to the class RSSurfaceFrameWindows.
    // We use reinterpret_cast instead of RTTI and dynamic_cast which are not permitted.
    auto frameWindows = reinterpret_cast<RSSurfaceFrameWindows *>(frame.get());
    void *addr = nullptr;
    if (frameWindows->addr_ == nullptr) {
        ROSEN_LOGW("RSSurfaceWindows::FlushFrame frame.addr is nullptr");
    } else {
        addr = frameWindows->addr_.get();
    }

    constexpr auto colorType = kRGBA_8888_SkColorType;
    SkBitmap bitmap;
    bitmap.setInfo(SkImageInfo::Make(frameWindows->width_,
                                     frameWindows->height_,
                                     colorType, kPremul_SkAlphaType));
    bitmap.setPixels(addr);
    if (frameWindows->surface_ != nullptr) {
        frameWindows->surface_->readPixels(bitmap, 0, 0);
    }

#ifdef USE_GLFW_WINDOW
    if (frameWindows->surface_ != nullptr) {
        YInvert(addr, frameWindows->width_, frameWindows->height_);
    }
#endif

    int32_t width = frameWindows->width_;
    int32_t height = frameWindows->height_;
    int32_t size = width * height * 0x4;
    onRender_(addr, size, width, height);

#ifdef USE_GLFW_WINDOW
    GlfwRenderContext::GetGlobal()->SwapBuffers();
#endif
    return true;
}

RenderContext* RSSurfaceWindows::GetRenderContext()
{
    return renderContext_;
}

void RSSurfaceWindows::SetRenderContext(RenderContext* context)
{
    renderContext_ = context;
}

void RSSurfaceWindows::YInvert(void *addr, int32_t width, int32_t height)
{
    const auto &pixels = reinterpret_cast<uint32_t *>(addr);
    const auto &halfHeight = height / 0x2;
    const auto &tmpPixels = std::make_unique<uint32_t[]>(width * halfHeight);
    for (int32_t i = 0; i < halfHeight; i++) {
        for (int32_t j = 0; j < width; j++) {
            tmpPixels[i * width + j] = pixels[i * width + j];
        }
    }

    for (int32_t i = 0; i < halfHeight; i++) {
        const auto &r = height - 1 - i;
        for (int32_t j = 0; j < width; j++) {
            pixels[i * width + j] = pixels[r * width + j];
        }
    }

    for (int32_t i = 0; i < halfHeight; i++) {
        const auto &r = height - 1 - i;
        for (int32_t j = 0; j < width; j++) {
            pixels[r * width + j] = tmpPixels[i * width + j];
        }
    }
}

bool RSSurfaceWindows::SetupGrContext()
{
    if (grContext_ != nullptr) {
        return true;
    }

    GlfwRenderContext::GetGlobal()->MakeCurrent();
    sk_sp<const GrGLInterface> glinterface{GrGLCreateNativeInterface()};
    if (glinterface == nullptr) {
        ROSEN_LOGE("glinterface is nullptr");
        return false;
    }

    GrContextOptions options;
    options.fPreferExternalImagesOverES3 = true;
    options.fDisableDistanceFieldPaths = true;
    options.fGpuPathRenderers &= ~GpuPathRenderers::kCoverageCounting;
    const auto &grContext = GrContext::MakeGL(glinterface, options);
    if (grContext == nullptr) {
        ROSEN_LOGE("grContext is nullptr");
        return false;
    }

    grContext_ = grContext;
    return true;
}

uint32_t RSSurfaceWindows::GetQueueSize() const
{
    return 0x3;
}

void RSSurfaceWindows::ClearBuffer()
{
}

void RSSurfaceWindows::ClearAllBuffer()
{
}

void RSSurfaceWindows::ResetBufferAge()
{
    ROSEN_LOGD("RSSurfaceWindows: Reset Buffer Age!");
}
} // namespace Rosen
} // namespace OHOS
