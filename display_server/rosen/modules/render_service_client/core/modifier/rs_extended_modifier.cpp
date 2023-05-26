/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "modifier/rs_extended_modifier.h"

#include <memory>

#include "modifier/rs_render_modifier.h"
#include "modifier/rs_modifier_type.h"
#include "pipeline/rs_node_map.h"
#include "pipeline/rs_recording_canvas.h"
#include "ui/rs_canvas_node.h"

namespace OHOS {
namespace Rosen {
RSDrawingContext RSExtendedModifierHelper::CreateDrawingContext(NodeId nodeId)
{
    auto node = RSNodeMap::Instance().GetNode<RSCanvasNode>(nodeId);
    if (!node) {
        return { nullptr };
    }
    auto recordingCanvas = new RSRecordingCanvas(node->GetPaintWidth(), node->GetPaintHeight());
    return { recordingCanvas, node->GetPaintWidth(), node->GetPaintHeight() };
}

std::shared_ptr<RSRenderModifier> RSExtendedModifierHelper::CreateRenderModifier(
    RSDrawingContext& ctx, PropertyId id, RSModifierType type)
{
    auto renderProperty = std::make_shared<RSRenderProperty<DrawCmdListPtr>>(
        RSExtendedModifierHelper::FinishDrawing(ctx), id);
    auto renderModifier =  std::make_shared<RSDrawCmdListRenderModifier>(renderProperty);
    renderModifier->SetType(type);
    return renderModifier;
}

std::shared_ptr<DrawCmdList> RSExtendedModifierHelper::FinishDrawing(RSDrawingContext& ctx)
{
    auto recording = static_cast<RSRecordingCanvas*>(ctx.canvas)->GetDrawCmdList();
    delete ctx.canvas;
    ctx.canvas = nullptr;
    return recording;
}
} // namespace Rosen
} // namespace OHOS
