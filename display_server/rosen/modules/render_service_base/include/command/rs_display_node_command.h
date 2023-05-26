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

#ifndef ROSEN_RENDER_SERVICE_BASE_COMMAND_RS_DISPLAY_NODE_COMMAND_H
#define ROSEN_RENDER_SERVICE_BASE_COMMAND_RS_DISPLAY_NODE_COMMAND_H

#include "command/rs_command_templates.h"
#include "common/rs_macros.h"

namespace OHOS {
namespace Rosen {

enum RSDisplayNodeCommandType : uint16_t {
    DISPLAY_NODE_CREATE,
    DISPLAY_NODE_SET_SCREEN_ID,
    DISPLAY_NODE_SET_DISPLAY_OFFSET,
    DISPLAY_NODE_SET_SECURITY_DISPLAY,
    DISPLAY_NODE_SET_DISPLAY_MODE,
};

class RSB_EXPORT DisplayNodeCommandHelper {
public:
    static void Create(RSContext&, NodeId, const RSDisplayNodeConfig&);
    static void SetScreenId(RSContext&, NodeId, uint64_t);
    static void SetDisplayOffset(RSContext&, NodeId, int32_t, int32_t);
    static void SetSecurityDisplay(RSContext&, NodeId, bool);
    static void SetDisplayMode(RSContext&, NodeId, const RSDisplayNodeConfig&);
};

ADD_COMMAND(RSDisplayNodeCreate,
    ARG(DISPLAY_NODE, DISPLAY_NODE_CREATE, DisplayNodeCommandHelper::Create, NodeId, RSDisplayNodeConfig))
ADD_COMMAND(RSDisplayNodeSetScreenId,
    ARG(DISPLAY_NODE, DISPLAY_NODE_SET_SCREEN_ID, DisplayNodeCommandHelper::SetScreenId, NodeId, uint64_t))
ADD_COMMAND(RSDisplayNodeSetDisplayOffset,
    ARG(DISPLAY_NODE, DISPLAY_NODE_SET_DISPLAY_OFFSET, DisplayNodeCommandHelper::SetDisplayOffset, NodeId,
    int32_t, int32_t))
ADD_COMMAND(RSDisplayNodeSetSecurityDisplay,
    ARG(DISPLAY_NODE, DISPLAY_NODE_SET_SECURITY_DISPLAY, DisplayNodeCommandHelper::SetSecurityDisplay, NodeId, bool))
ADD_COMMAND(RSDisplayNodeSetDisplayMode,
    ARG(DISPLAY_NODE, DISPLAY_NODE_SET_DISPLAY_MODE, DisplayNodeCommandHelper::SetDisplayMode, NodeId,
    RSDisplayNodeConfig))
} // namespace Rosen
} // namespace OHOS

#endif // ROSEN_RENDER_SERVICE_BASE_COMMAND_RS_DISPLAY_NODE_COMMAND_H
