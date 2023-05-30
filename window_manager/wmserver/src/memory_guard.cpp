/*
* Copyright (C) 2023 Huawei Device Co., Ltd.
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
#include "memory_guard.h"

#include "window_manager_hilog.h"
#include "malloc.h"
namespace OHOS {
namespace Rosen {
#ifdef CONFIG_USE_JEMALLOC_DFX_INTF
namespace {
    constexpr HiviewDFX::HiLogLabel LABEL = {LOG_CORE, HILOG_DOMAIN_WINDOW, "MemoryGuard"};
}
#endif
MemoryGuard::MemoryGuard()
{
#ifdef CONFIG_USE_JEMALLOC_DFX_INTF
    int ret1 = mallopt(M_SET_THREAD_CACHE, M_THREAD_CACHE_DISABLE); // 0 indicates success
    int ret2 = mallopt(M_DELAYED_FREE, M_DELAYED_FREE_DISABLE);
    WLOGFD("disable tcache and delay free, result[%{public}d, %{public}d]", ret1, ret2);
#endif
}
MemoryGuard::~MemoryGuard()
{
#ifdef CONFIG_USE_JEMALLOC_DFX_INTF
    int err = mallopt(M_FLUSH_THREAD_CACHE, 0);
    WLOGFD("flush cache, result: %{public}d", err);
#endif
}
} // namespace Rosen
} // namespace OHOS