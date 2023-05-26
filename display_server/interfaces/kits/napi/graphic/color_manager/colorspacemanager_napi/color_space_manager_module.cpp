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
#include "js_color_space_manager.h"
#include "native_engine/native_engine.h"

extern "C" __attribute__((constructor)) void NAPI_application_ColorSpaceManager_AotoRegister(void)
{
    auto moduleManager = NativeModuleManager::GetInstance();
    // name and fine name should be formated as api file name
    NativeModule colorSpaceManagerModule = {
        .name = "graphics.colorSpaceManager",
        .fileName = "graphics/libgraphics_napi.so/colorSpaceManager.js",
        .registerCallback = OHOS::ColorManager::JsColorSpaceManagerInit,
    };

    moduleManager->Register(&colorSpaceManagerModule);
}
