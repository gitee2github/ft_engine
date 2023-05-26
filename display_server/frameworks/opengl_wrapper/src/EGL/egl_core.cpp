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
#include "egl_core.h"

#include <mutex>

#include "egl_defs.h"
#include "egl_pre_initializer.h"
#include "egl_wrapper_layer.h"
#include "egl_wrapper_loader.h"
#include "../wrapper_log.h"

namespace OHOS {
EglWrapperDispatchTable gWrapperHook;
GlHookTable gGlHookNoContext;

#undef CALL_HOOK_API
#define CALL_HOOK_API(...)

#undef CALL_HOOK_API_RET
#define CALL_HOOK_API_RET CALL_HOOK_API

#undef HOOK_API_ENTRY
#define HOOK_API_ENTRY(r, api, ...) #api,

char const * const gWrapperApiNames[EGL_API_NUM] = {
#include "../wrapper_hook_entries.in"
    nullptr
};

char const * const gEglApiNames[EGL_API_NUM] = {
#include "../egl_hook_entries.in"
    nullptr
};

char const * const gGlApiNames1[GL_API_NUM] = {
#include "../gl1_hook_entries.in"
    nullptr
};

char const * const gGlApiNames2[GL_API_NUM] = {
#include "../gl2_hook_entries.in"
    nullptr
};

char const * const gGlApiNames3[GL_API_NUM] = {
#include "../gl3_hook_entries.in"
    nullptr
};

using namespace OHOS;

static std::mutex gInitMutex;
static EglPreInitializer preInitializer;

void WrapperHookTableInit() noexcept
{
    WLOGD("");
    char const * const *apiName = gWrapperApiNames;
    EglWrapperFuncPointer *curr = reinterpret_cast<EglWrapperFuncPointer*>(&gWrapperHook.wrapper);
    while (*apiName) {
        std::string name = *apiName;
        EglWrapperFuncPointer addr = FindEglWrapperApi(name);
        if (addr == nullptr) {
            WLOGW("No addr found in wrapper entries lookup table for %{public}s", *apiName);
        }
        *curr++ = addr;
        apiName++;
    }
}

bool EglCoreInit()
{
    std::lock_guard<std::mutex> lock(gInitMutex);

    if (gWrapperHook.isLoad) {
        return true;
    }

    if (!preInitializer.InitStat()) {
        WLOGE("preInit Error.");
        return false;
    }

    WrapperHookTableInit();
    EglWrapperLoader& loader(EglWrapperLoader::GetInstance());
    if (!loader.Load(&gWrapperHook)) {
        WLOGE("EglWrapperLoader Load Failed.");
        return false;
    }

    EglWrapperLayer& layer(EglWrapperLayer::GetInstance());
    if (!layer.Init(&gWrapperHook)) {
        WLOGE("EglWrapperLayer Init Failed.");
    }

    return true;
}
}; // namespace OHOS
