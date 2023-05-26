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

#include "pipeline/rs_render_service_listener.h"

#include "platform/common/rs_log.h"
#include "pipeline/rs_main_thread.h"
#include "sync_fence.h"

namespace OHOS {
namespace Rosen {

RSRenderServiceListener::~RSRenderServiceListener() {}

RSRenderServiceListener::RSRenderServiceListener(std::weak_ptr<RSSurfaceRenderNode> surfaceRenderNode)
    : surfaceRenderNode_(surfaceRenderNode)
{}

void RSRenderServiceListener::OnBufferAvailable()
{
    auto node = surfaceRenderNode_.lock();
    if (node == nullptr) {
        RS_LOGE("RSRenderServiceListener::OnBufferAvailable node is nullptr");
        return;
    }
    RS_LOGD("RsDebug RSRenderServiceListener::OnBufferAvailable node id:%" PRIu64, node->GetId());
    node->IncreaseAvailableBuffer();
    if (!node->IsNotifyUIBufferAvailable()) {
        // Only ipc for one time.
        RS_LOGD("RsDebug RSRenderServiceListener::OnBufferAvailable id = %" PRIu64 " Notify UI buffer available",
            node->GetId());
        node->NotifyUIBufferAvailable();
    }
    RSMainThread::Instance()->RequestNextVSync();
}

void RSRenderServiceListener::OnTunnelHandleChange()
{
    auto node = surfaceRenderNode_.lock();
    if (node == nullptr) {
        RS_LOGE("RSRenderServiceListener::OnTunnelHandleChange node is nullptr");
        return;
    }
    node->SetTunnelHandleChange(true);
    if (!node->IsNotifyUIBufferAvailable()) {
        // Only ipc for one time.
        RS_LOGD("RsDebug RSRenderServiceListener::OnTunnelHandleChange id = %" PRIu64 " Notify UI buffer available",
            node->GetId());
        node->NotifyUIBufferAvailable();
    }
    RSMainThread::Instance()->RequestNextVSync();
}

void RSRenderServiceListener::OnCleanCache()
{
    std::weak_ptr<RSSurfaceRenderNode> surfaceNode = surfaceRenderNode_;
    RSMainThread::Instance()->PostTask([surfaceNode]() {
        auto node = surfaceNode.lock();
        if (node == nullptr) {
            RS_LOGW("RSRenderServiceListener::OnBufferAvailable node is nullptr");
            return;
        }
        RS_LOGD("RsDebug RSRenderServiceListener::OnCleanCache node id:%" PRIu64, node->GetId());
        node->ResetBufferAvailableCount();
    });
}

void RSRenderServiceListener::OnGoBackground()
{
    std::weak_ptr<RSSurfaceRenderNode> surfaceNode = surfaceRenderNode_;
    RSMainThread::Instance()->PostTask([surfaceNode]() {
        auto node = surfaceNode.lock();
        if (node == nullptr) {
            RS_LOGW("RSRenderServiceListener::OnBufferAvailable node is nullptr");
            return;
        }
        RS_LOGD("RsDebug RSRenderServiceListener::OnGoBackground node id:%" PRIu64, node->GetId());
        node->ResetBufferAvailableCount();
        node->CleanCache();
    });
}
} // namespace Rosen
} // namespace OHOS
