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

#include "rs_window_animation_finished_callback.h"

#include "rs_window_animation_log.h"

namespace OHOS {
namespace Rosen {
RSWindowAnimationFinishedCallback::RSWindowAnimationFinishedCallback(const std::function<void(void)>& callback)
    : callback_(callback)
{
}

void RSWindowAnimationFinishedCallback::OnAnimationFinished()
{
    WALOGD("On animation finished!");
    if (callback_ == nullptr) {
        WALOGE("Callback is null!");
        return;
    }
    callback_();
}
} // namespace Rosen
} // namespace OHOS
