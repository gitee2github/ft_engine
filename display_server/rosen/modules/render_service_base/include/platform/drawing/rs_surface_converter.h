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
#ifndef RENDER_SERVICE_CLIENT_CORE_UI_RS_SURFACE_CONVERTER_H
#define RENDER_SERVICE_CLIENT_CORE_UI_RS_SURFACE_CONVERTER_H

#include "platform/drawing/rs_surface.h"
#include "surface.h"

namespace OHOS {
namespace Rosen {

class RSB_EXPORT RSSurfaceConverter {
public:
    static RSB_EXPORT sptr<Surface> ConvertToOhosSurface(std::shared_ptr<RSSurface> surface);
};
} // namespace Rosen
} // namespace OHOS

#endif // RENDER_SERVICE_CLIENT_CORE_UI_RS_SURFACE_CONVERTER_H