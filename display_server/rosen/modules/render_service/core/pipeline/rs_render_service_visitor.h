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

#ifndef RENDER_SERVICE_CLIENT_CORE_RENDER_RS_RENDER_SERVICE_VISITOR_H
#define RENDER_SERVICE_CLIENT_CORE_RENDER_RS_RENDER_SERVICE_VISITOR_H

#include <memory>

#include "include/core/SkCanvas.h"
#include "pipeline/rs_paint_filter_canvas.h"
#include "pipeline/rs_processor.h"
#include "visitor/rs_node_visitor.h"

namespace OHOS {
namespace Rosen {

class RSRenderServiceVisitor : public RSNodeVisitor {
public:
    RSRenderServiceVisitor(bool parallel = false);
    ~RSRenderServiceVisitor();

    void PrepareBaseRenderNode(RSBaseRenderNode &node) override;
    void PrepareCanvasRenderNode(RSCanvasRenderNode &node) override {}
    void PrepareDisplayRenderNode(RSDisplayRenderNode &node) override;
    void PrepareProxyRenderNode(RSProxyRenderNode& node) override {}
    void PrepareRootRenderNode(RSRootRenderNode& node) override {}
    void PrepareSurfaceRenderNode(RSSurfaceRenderNode &node) override;

    void ProcessBaseRenderNode(RSBaseRenderNode &node) override;
    void ProcessCanvasRenderNode(RSCanvasRenderNode& node) override {}
    void ProcessDisplayRenderNode(RSDisplayRenderNode &node) override;
    void ProcessProxyRenderNode(RSProxyRenderNode& node) override {}
    void ProcessRootRenderNode(RSRootRenderNode& node) override {}
    void ProcessSurfaceRenderNode(RSSurfaceRenderNode &node) override;

    void SetAnimateState(bool doAnimate)
    {
        doAnimate_ = doAnimate;
    }

    bool ShouldForceSerial()
    {
        return mForceSerial;
    }

private:
    std::unique_ptr<SkCanvas> skCanvas_;
    std::shared_ptr<RSPaintFilterCanvas> canvas_;
    float globalZOrder_ = 0.0f;
    int32_t offsetX_ = 0;
    int32_t offsetY_ = 0;
    bool isSecurityDisplay_ = false;
    bool mParallelEnable = false;
    bool mForceSerial = false;
    std::shared_ptr<RSProcessor> processor_ = nullptr;
    bool doAnimate_ = false;
};
} // namespace Rosen
} // namespace OHOS

#endif // RENDER_SERVICE_CLIENT_CORE_RENDER_RS_RENDER_SERVICE_VISITOR_H
