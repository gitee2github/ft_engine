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

#ifndef RS_SCREEN_MODE_INFO
#define RS_SCREEN_MODE_INFO

#include <cstdint>
#include <parcel.h>

#include "common/rs_macros.h"

namespace OHOS {
namespace Rosen {
class RSB_EXPORT RSScreenModeInfo : public Parcelable {
public:
    RSScreenModeInfo() = default;
    RSScreenModeInfo(int32_t width, int32_t height, uint32_t refreshRate, int32_t id);
    ~RSScreenModeInfo() noexcept = default;

    RSScreenModeInfo(const RSScreenModeInfo& other);
    RSScreenModeInfo& operator=(const RSScreenModeInfo& other);

    bool Marshalling(Parcel &parcel) const override;
    static RSScreenModeInfo *Unmarshalling(Parcel &parcel);

    int32_t GetScreenWidth() const;
    int32_t GetScreenHeight() const;
    uint32_t GetScreenRefreshRate() const;
    int32_t GetScreenModeId() const;
    void SetScreenWidth(int32_t width);
    void SetScreenHeight(int32_t height);
    void SetScreenRefreshRate(uint32_t refreshRate);
    void SetScreenModeId(int32_t id);

private:
    int32_t width_ = -1;
    int32_t height_ = -1;
    uint32_t refreshRate_ = 0;
    int32_t modeId_ = -1;
};
} // namespace Rosen
} // namespace OHOS

#endif // RS_SCREEN_MODE_INFO