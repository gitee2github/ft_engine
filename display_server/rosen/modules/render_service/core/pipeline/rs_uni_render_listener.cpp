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

#include "pipeline/rs_uni_render_listener.h"

#include "pipeline/rs_main_thread.h"
#include "platform/common/rs_log.h"

namespace OHOS {
namespace Rosen {
RSUniRenderListener::~RSUniRenderListener() {}

RSUniRenderListener::RSUniRenderListener(std::weak_ptr<RSDisplayRenderNode> displayRenderNode)
    : displayRenderNode_(displayRenderNode) {}

void RSUniRenderListener::OnBufferAvailable()
{
    auto node = displayRenderNode_.lock();
    if (node == nullptr) {
        RS_LOGE("RSUniRenderListener::OnBufferAvailable node is nullptr");
        return;
    }
    RS_LOGD("RSUniRenderListener::OnBufferAvailable node id:%" PRIu64, node->GetId());
    node->IncreaseAvailableBuffer();

    RSMainThread::Instance()->NotifyUniRenderFinish();
}
}
}
