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

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <string>
#include "pixel_map.h"

namespace OHOS::Rosen {
class CommonTestUtils {
public:
    static void InjectTokenInfoByHapName(int userID, const std::string& bundleName, int instIndex);
    static std::shared_ptr<Media::PixelMap> CreatePixelMap();
    static constexpr int32_t TEST_IMAGE_HEIGHT = 1080;
    static constexpr int32_t TEST_IMAGE_WIDTH = 1920;
    static void SetAceessTokenPermission(const std::string processName);
    static void SetAceessTokenPermission(const std::string processName,
                                        const char** perms, const int permCount);
};
} // namespace OHOS::Rosen
#endif // TEST_UTILS_H