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

#include <iostream>
#include <refbase.h>
#include "screenshot_listener_future.h"

using namespace OHOS;
using namespace OHOS::Rosen;

int main(int argc, char *argv[])
{
    std::cout << "===========================Start===========================" << std::endl;
    std::cout << "Please do screenshot in 20s..." << std::endl;
    sptr<ScreenshotListenerFuture> listener = new ScreenshotListenerFuture();

    auto& dm = DisplayManager::GetInstance();
    dm.RegisterScreenshotListener(listener);

    auto info = listener->future_.GetResult(20000);
    std::cout << "screenshot trigger=" << info.GetTrigger()
        << ", display=" << std::to_string(info.GetDisplayId()) << std::endl;

    dm.UnregisterScreenshotListener(listener);

    std::cout << "============================End============================" << std::endl;
    return 0;
}