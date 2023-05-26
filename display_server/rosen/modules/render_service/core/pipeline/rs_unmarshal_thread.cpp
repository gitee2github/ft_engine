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

#include "pipeline/rs_unmarshal_thread.h"

#include "pipeline/rs_base_render_util.h"
#include "pipeline/rs_main_thread.h"
#include "platform/common/rs_log.h"
#include "transaction/rs_transaction_data.h"

namespace OHOS::Rosen {
RSUnmarshalThread& RSUnmarshalThread::Instance()
{
    static RSUnmarshalThread instance;
    return instance;
}

void RSUnmarshalThread::Start()
{
    runner_ = AppExecFwk::EventRunner::Create("RSUnmarshalThread");
    handler_ = std::make_shared<AppExecFwk::EventHandler>(runner_);
}

void RSUnmarshalThread::PostTask(const std::function<void()>& task)
{
    if (handler_) {
        handler_->PostTask(task, AppExecFwk::EventQueue::Priority::IMMEDIATE);
    }
}

void RSUnmarshalThread::RecvParcel(std::shared_ptr<MessageParcel>& parcel)
{
    if (!handler_) {
        RS_LOGE("RSUnmarshalThread::RecvParcel handler_ is nullptr");
        return;
    }
    RSTaskMessage::RSTask task = [this, parcel = parcel]() {
        auto transData = RSBaseRenderUtil::ParseTransactionData(*parcel);
        if (!transData) {
            return;
        }
        if (!transData->GetUniRender()) {
            RSMainThread::Instance()->RecvRSTransactionData(transData);
            return;
        }
        std::lock_guard<std::mutex> lock(transactionDataMutex_);
        cachedTransactionDataMap_[transData->GetSendingPid()].emplace_back(std::move(transData));
    };
    PostTask(task);
    RSMainThread::Instance()->RequestNextVSync();
}

TransactionDataMap RSUnmarshalThread::GetCachedTransactionData()
{
    TransactionDataMap transactionData;
    {
        std::lock_guard<std::mutex> lock(transactionDataMutex_);
        std::swap(transactionData, cachedTransactionDataMap_);
    }
    return transactionData;
}
}
