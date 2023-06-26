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

#include "pipeline/rs_display_render_node.h"

#include "common/rs_obj_abs_geometry.h"
#include "platform/common/rs_log.h"
#include "screen_manager/screen_types.h"
#include "visitor/rs_node_visitor.h"
#include "transaction/rs_render_service_client.h"

namespace OHOS {
namespace Rosen {
RSDisplayRenderNode::RSDisplayRenderNode(NodeId id, const RSDisplayNodeConfig& config, std::weak_ptr<RSContext> context)
    : RSRenderNode(id, context), RSSurfaceHandler(id), screenId_(config.screenId), offsetX_(0), offsetY_(0),
      isMirroredDisplay_(config.isMirrored),
      dirtyManager_(std::make_shared<RSDirtyRegionManager>())
{}

RSDisplayRenderNode::~RSDisplayRenderNode() = default;

void RSDisplayRenderNode::CollectSurface(
    const std::shared_ptr<RSBaseRenderNode>& node, std::vector<RSBaseRenderNode::SharedPtr>& vec, bool isUniRender)
{
    ResetSortedChildren();
    for (auto& child : node->GetSortedChildren()) {
        child->CollectSurface(child, vec, isUniRender);
    }
}

void RSDisplayRenderNode::Prepare(const std::shared_ptr<RSNodeVisitor>& visitor)
{
    if (!visitor) {
        return;
    }
    visitor->PrepareDisplayRenderNode(*this);
}

void RSDisplayRenderNode::Process(const std::shared_ptr<RSNodeVisitor>& visitor)
{
    if (!visitor) {
        return;
    }
    RSRenderNode::RenderTraceDebug();
    visitor->ProcessDisplayRenderNode(*this);
}

RSDisplayRenderNode::CompositeType RSDisplayRenderNode::GetCompositeType() const
{
    return compositeType_;
}

void RSDisplayRenderNode::SetCompositeType(RSDisplayRenderNode::CompositeType type)
{
    compositeType_ = type;
}

void RSDisplayRenderNode::SetForceSoftComposite(bool flag)
{
    forceSoftComposite_ = flag;
}

bool RSDisplayRenderNode::IsForceSoftComposite() const
{
    return forceSoftComposite_;
}

void RSDisplayRenderNode::SetMirrorSource(SharedPtr node)
{
    if (!isMirroredDisplay_ || node == nullptr) {
        return;
    }
    mirrorSource_ = node;
}

void RSDisplayRenderNode::ResetMirrorSource()
{
    mirrorSource_.reset();
}

bool RSDisplayRenderNode::IsMirrorDisplay() const
{
    return isMirroredDisplay_;
}

void RSDisplayRenderNode::SetSecurityDisplay(bool isSecurityDisplay)
{
    isSecurityDisplay_ = isSecurityDisplay;
}

bool RSDisplayRenderNode::GetSecurityDisplay() const
{
    return isSecurityDisplay_;
}

void RSDisplayRenderNode::SetIsMirrorDisplay(bool isMirror)
{
    isMirroredDisplay_ = isMirror;
    RS_LOGD("RSDisplayRenderNode::SetIsMirrorDisplay, node id:[%" PRIu64 "], isMirrorDisplay: [%s]", GetId(),
        IsMirrorDisplay() ? "true" : "false");
}

#ifndef ROSEN_CROSS_PLATFORM
bool RSDisplayRenderNode::CreateSurface(sptr<IBufferConsumerListener> listener)
{
    if (consumer_ != nullptr && surface_ != nullptr) {
        RS_LOGI("RSDisplayRenderNode::CreateSurface already created, return");
        return true;
    }
    consumer_ = Surface::CreateSurfaceAsConsumer("DisplayNode");
    if (consumer_ == nullptr) {
        RS_LOGE("RSDisplayRenderNode::CreateSurface get consumer surface fail");
        return false;
    }
    SurfaceError ret = consumer_->RegisterConsumerListener(listener);
    if (ret != SURFACE_ERROR_OK) {
        RS_LOGE("RSDisplayRenderNode::CreateSurface RegisterConsumerListener fail");
        return false;
    }
    consumerListener_ = listener;
    auto producer = consumer_->GetProducer();
    sptr<Surface> surface = Surface::CreateSurfaceAsProducer(producer);
    auto client = std::static_pointer_cast<RSRenderServiceClient>(RSIRenderClient::CreateRenderServiceClient());
    surface_ = client->CreateRSSurface(surface);
    RS_LOGI("RSDisplayRenderNode::CreateSurface end");
    surfaceCreated_ = true;
    return true;
}
#endif

bool RSDisplayRenderNode::SkipFrame(uint32_t skipFrameInterval)
{
    frameCount_++;
    // ensure skipFrameInterval is not 0
    if (skipFrameInterval == 0) {
        return false;
    }
    if ((frameCount_ - 1) % skipFrameInterval == 0) {
        return false;
    }
    return true;
}

ScreenRotation RSDisplayRenderNode::GetRotation() const
{
    auto boundsGeoPtr = std::static_pointer_cast<RSObjAbsGeometry>(GetRenderProperties().GetBoundsGeometry());
    if (boundsGeoPtr == nullptr) {
        return ScreenRotation::ROTATION_0;
    }
    // -90.0f: convert rotation degree to 4 enum values
    return static_cast<ScreenRotation>(static_cast<int32_t>(std::roundf(boundsGeoPtr->GetRotation() / -90.0f)) % 4);
}

bool RSDisplayRenderNode::IsRotationChanged() const
{
    auto boundsGeoPtr = std::static_pointer_cast<RSObjAbsGeometry>(GetRenderProperties().GetBoundsGeometry());
    if (boundsGeoPtr == nullptr) {
        return false;
    }
    // boundsGeoPtr->IsNeedClientCompose() return false if rotation degree is times of 90
    // which means rotation is end.
    bool isRotationEnd = !boundsGeoPtr->IsNeedClientCompose();
    return !(ROSEN_EQ(boundsGeoPtr->GetRotation(), lastRotation_) && isRotationEnd);
}

void RSDisplayRenderNode::UpdateRotation()
{
    auto boundsGeoPtr = std::static_pointer_cast<RSObjAbsGeometry>(GetRenderProperties().GetBoundsGeometry());
    if (boundsGeoPtr == nullptr) {
        return;
    }
    lastRotation_ = boundsGeoPtr->GetRotation();
}

void RSDisplayRenderNode::UpdateDisplayDirtyManager(int32_t bufferage)
{
    dirtyManager_->SetBufferAge(bufferage);
    dirtyManager_->UpdateDirty();
}

void RSDisplayRenderNode::ClearCurrentSurfacePos()
{
    lastFrameSurfacePos_.clear();
    lastFrameSurfacePos_.swap(currentFrameSurfacePos_);
}

} // namespace Rosen
} // namespace OHOS
