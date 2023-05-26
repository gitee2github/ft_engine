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

#ifndef DRAWING_LOG_H
#define DRAWING_LOG_H

#include <chrono>
#include <cstdint>
#include <functional>
#ifdef OHOS_PLATFORM
#include <hilog/log.h>
#endif

namespace OHOS {
namespace Rosen {

#ifndef LOGD
#ifdef OHOS_PLATFORM
#define LOGD(fmt, ...)               \
    ::OHOS::HiviewDFX::HiLog::Debug(::OHOS::HiviewDFX::HiLogLabel \
        { LOG_CORE, 0xD001402, "2DGraphics" }, "%{public}s: " fmt, __func__, ##__VA_ARGS__)
#define LOGI(fmt, ...)              \
    ::OHOS::HiviewDFX::HiLog::Info(::OHOS::HiviewDFX::HiLogLabel \
        { LOG_CORE, 0xD001402, "2DGraphics" }, "%{public}s: " fmt, __func__, ##__VA_ARGS__)
#define LOGW(fmt, ...)              \
    ::OHOS::HiviewDFX::HiLog::Warn(::OHOS::HiviewDFX::HiLogLabel \
        { LOG_CORE, 0xD001402, "2DGraphics" }, "%{public}s: " fmt, __func__, ##__VA_ARGS__)
#define LOGE(fmt, ...)               \
    ::OHOS::HiviewDFX::HiLog::Error(::OHOS::HiviewDFX::HiLogLabel \
        { LOG_CORE, 0xD001402, "2DGraphics" }, "%{public}s: " fmt, __func__, ##__VA_ARGS__)
#else
#define LOGD(fmt, ...)
#define LOGI(fmt, ...)
#define LOGW(fmt, ...)
#define LOGE(fmt, ...)
#endif
#endif
} // namespace Rosen
} // namespace OHOS
#endif
