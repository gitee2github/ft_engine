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

#include "rs_surface_frame_windows.h"

#include "platform/common/rs_log.h"

namespace OHOS {
namespace Rosen {
RSSurfaceFrameWindows::RSSurfaceFrameWindows(int32_t width, int32_t height)
    : width_(width), height_(height)
{
    addr_ = std::make_unique<uint32_t[]>(width_ * height_);
}

void RSSurfaceFrameWindows::SetDamageRegion(int32_t left, int32_t top, int32_t width, int32_t height)
{
}

int32_t RSSurfaceFrameWindows::GetBufferAge() const
{
    return -1;
}

SkCanvas* RSSurfaceFrameWindows::GetCanvas()
{
    if (surface_ == nullptr) {
        CreateSurface();
    }

    if (surface_ != nullptr) {
        return surface_->getCanvas();
    }
    return nullptr;
}

sk_sp<SkSurface> RSSurfaceFrameWindows::GetSurface()
{
    return surface_;
}

void RSSurfaceFrameWindows::SetRenderContext(RenderContext* context)
{
    renderContext_ = context;
}

void RSSurfaceFrameWindows::CreateSurface()
{
    if (addr_ == nullptr) {
        ROSEN_LOGW("RSSurfaceFrameWindows::GetCanvas buffer is nullptr");
        return;
    }

    const auto &info = SkImageInfo::Make(width_, height_, kRGBA_8888_SkColorType, kPremul_SkAlphaType);
    surface_ = SkSurface::MakeRasterDirect(info, addr_.get(), width_);
}
} // namespace Rosen
} // namespace OHOS
