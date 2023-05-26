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

#include "surface_capture_callback_stub.h"
#include "platform/common/rs_log.h"

namespace OHOS {
namespace Rosen {
int RSSurfaceCaptureCallbackStub::OnRemoteRequest(
    uint32_t code, MessageParcel& data, MessageParcel& reply, MessageOption& option)
{
    auto token = data.ReadInterfaceToken();
    if (token != RSISurfaceCaptureCallback::GetDescriptor()) {
        ROSEN_LOGE("RSSurfaceCaptureCallbackStub: token ERR_INVALID_STATE");
        return ERR_INVALID_STATE;
    }
    int ret = ERR_NONE;
    switch (code) {
        case ON_SURFACE_CAPTURE: {
            NodeId id = data.ReadUint64();
            auto pixelmap = data.ReadParcelable<OHOS::Media::PixelMap>();
            OnSurfaceCapture(id, pixelmap);
            break;
        }
        default: {
            ret = ERR_UNKNOWN_TRANSACTION;
            break;
        }
    }

    return ret;
}
} // namespace Rosen
} // namespace OHOS
