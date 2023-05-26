/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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


#include "rs_virtual_screen_processor.h"

#include <ctime>

#include "platform/common/rs_log.h"
#include "platform/ohos/backend/rs_surface_frame_ohos_raster.h"
#include "rs_base_render_util.h"
#include "rs_divided_render_util.h"
#include "rs_trace.h"
#include "string_utils.h"

namespace OHOS {
namespace Rosen {
RSVirtualScreenProcessor::RSVirtualScreenProcessor()
{
}

RSVirtualScreenProcessor::~RSVirtualScreenProcessor() noexcept
{
}

bool RSVirtualScreenProcessor::Init(RSDisplayRenderNode& node, int32_t offsetX, int32_t offsetY, ScreenId mirroredId)
{
    if (!RSProcessor::Init(node, offsetX, offsetY, mirroredId)) {
        return false;
    }

    if (mirroredId != INVALID_SCREEN_ID) {
        SetMirrorScreenSwap(node);
    }

    renderFrameConfig_.usage = BUFFER_USAGE_CPU_READ | BUFFER_USAGE_MEM_DMA;

    auto screenManager = CreateOrGetScreenManager();
    producerSurface_ = screenManager->GetProducerSurface(node.GetScreenId());
    if (producerSurface_ == nullptr) {
        RS_LOGE(
            "RSVirtualScreenProcessor::Init for Screen(id %" PRIu64 "): ProducerSurface is null!", node.GetScreenId());
        return false;
    }

    bool forceCPU = false;
    renderFrame_ = renderEngine_->RequestFrame(producerSurface_, renderFrameConfig_, forceCPU, false);
    if (renderFrame_ == nullptr) {
        RS_LOGE("RSVirtualScreenProcessor::Init: renderFrame_ is null!");
        return false;
    }
    canvas_ = renderFrame_->GetCanvas();
    if (canvas_ == nullptr) {
        return false;
    }
    canvas_->concat(screenTransformMatrix_);

    return true;
}

void RSVirtualScreenProcessor::PostProcess()
{
    if (producerSurface_ == nullptr) {
        RS_LOGE("RSVirtualScreenProcessor::PostProcess surface is null!");
        return;
    }
    auto surfaceId = producerSurface_->GetUniqueId();
    renderEngine_->SetUiTimeStamp(renderFrame_, surfaceId);
    
    if (renderFrame_ == nullptr) {
        RS_LOGE("RSVirtualScreenProcessor::PostProcess renderFrame_ is null.");
        return;
    }
    renderFrame_->Flush();
}

void RSVirtualScreenProcessor::ProcessSurface(RSSurfaceRenderNode& node)
{
    if (canvas_ == nullptr) {
        RS_LOGE("RSVirtualScreenProcessor::ProcessSurface: Canvas is null!");
        return;
    }

    std::string traceInfo;
    AppendFormat(traceInfo, "RSVirtualScreenProcessor::ProcessSurface Node:%s ", node.GetName().c_str());
    RS_TRACE_NAME(traceInfo);

    // prepare BufferDrawParam
    // in display's coordinate.
    // clipHole: false.
    // forceCPU: true.
    auto params = RSDividedRenderUtil::CreateBufferDrawParam(node, false, false, false);
    const float adaptiveDstWidth = params.dstRect.width() * mirrorAdaptiveCoefficient_;
    const float adaptiveDstHeight = params.dstRect.height() * mirrorAdaptiveCoefficient_;
    params.dstRect.setWH(adaptiveDstWidth, adaptiveDstHeight);
    renderEngine_->DrawSurfaceNodeWithParams(*canvas_, node, params);
}

void RSVirtualScreenProcessor::ProcessDisplaySurface(RSDisplayRenderNode& node)
{
    RS_LOGI("RSVirtualScreenProcessor::ProcessDisplaySurface() is not supported.");
}
} // namespace Rosen
} // namespace OHOS
