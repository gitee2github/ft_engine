/*
 * Copyright (c) 2021-2023 Huawei Technologies Co., Ltd.
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
#include "platform/common/rs_log.h"
#include "platform/drawing/rs_surface_converter.h"
#include "rs_surface.h"
#include "platform/fangtian/backend/rs_surface_raster.h"
#include "platform/fangtian/backend/rs_surface_gl.h"

namespace OHOS {
namespace Rosen {
sptr<Surface> RSSurfaceConverter::ConvertToOhosSurface(std::shared_ptr<RSSurface> surface)
{
    if (surface == nullptr) {
        ROSEN_LOGE("nullptr input");
        return nullptr;
    }
#ifdef ACE_ENABLE_GL
    auto derivedPtr = std::static_pointer_cast<RSSurfaceOhosGl>(surface); // gpu render
#else
    auto derivedPtr = std::static_pointer_cast<RSSurfaceOhosRaster>(surface); // cpu render
#endif
    return derivedPtr->GetSurface();
}

} // namespace Rosen
} // namespace OHOS
