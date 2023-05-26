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

#ifndef ROSEN_MODULE_FRAME_ANALYZER_SRC_FRAME_SAVER_H
#define ROSEN_MODULE_FRAME_ANALYZER_SRC_FRAME_SAVER_H

#include <cstdint>
#include <fstream>

namespace OHOS {
namespace Rosen {
enum class FrameEventType;

class FrameSaver {
public:
    FrameSaver();
    ~FrameSaver();

    void SaveFrameEvent(const FrameEventType &type, int64_t timeNs);

private:
    std::ofstream ofs_;
};
} // namespace Rosen
} // namespace OHOS

#endif // ROSEN_MODULE_FRAME_ANALYZER_SRC_FRAME_SAVER_H
