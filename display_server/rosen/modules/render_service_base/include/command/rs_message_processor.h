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
#ifndef RENDER_SERVICE_CLIENT_CORE_MESSAGE_RS_MESSAGE_PROCESSOR_H
#define RENDER_SERVICE_CLIENT_CORE_MESSAGE_RS_MESSAGE_PROCESSOR_H

#include <memory>
#include <mutex>
#include <unordered_map>
#include "transaction/rs_transaction_data.h"

#include "common/rs_macros.h"

namespace OHOS {
namespace Rosen {
class RSCommand;
class RSB_EXPORT RSMessageProcessor final {
public:
    static RSB_EXPORT RSMessageProcessor& Instance();

    void AddUIMessage(uint32_t pid, std::unique_ptr<RSCommand>& command);
    void AddUIMessage(uint32_t pid, std::unique_ptr<RSCommand>&& command);

    bool HasTransaction() const;
    bool HasTransaction(uint32_t pid) const;

    RSTransactionData&& GetTransaction(uint32_t pid);
    std::unordered_map<uint32_t, RSTransactionData>&& GetAllTransactions();
private:
    RSMessageProcessor();
    ~RSMessageProcessor();
    RSMessageProcessor(const RSMessageProcessor&) = delete;
    RSMessageProcessor(const RSMessageProcessor&&) = delete;
    RSMessageProcessor& operator=(const RSMessageProcessor&) = delete;
    RSMessageProcessor& operator=(const RSMessageProcessor&&) = delete;

private:
    mutable std::mutex transactionMapMutex_;
    std::unordered_map<uint32_t, RSTransactionData> transactionMap_;
};
} // namespace Rosen
} // namespace OHOS
#endif // RENDER_SERVICE_CLIENT_CORE_MESSAGE_RS_MESSAGE_PROCESSOR_H
