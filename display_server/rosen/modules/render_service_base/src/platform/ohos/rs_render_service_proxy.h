/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef ROSEN_RENDER_SERVICE_BASE_TRANSACTION_RS_RENDER_SERVICE_PROXY_H
#define ROSEN_RENDER_SERVICE_BASE_TRANSACTION_RS_RENDER_SERVICE_PROXY_H

#include <iremote_proxy.h>
#include <platform/ohos/rs_irender_service.h>

namespace OHOS {
namespace Rosen {

class RSRenderServiceProxy : public IRemoteProxy<RSIRenderService> {
public:
    explicit RSRenderServiceProxy(const sptr<IRemoteObject>& impl);
    virtual ~RSRenderServiceProxy() noexcept = default;

    sptr<RSIRenderServiceConnection> CreateConnection(const sptr<RSIConnectionToken>& token) override;

private:
    static inline BrokerDelegator<RSRenderServiceProxy> delegator_;
};

} // namespace Rosen
} // namespace OHOS

#endif // ROSEN_RENDER_SERVICE_BASE_TRANSACTION_RS_RENDER_SERVICE_PROXY_H
