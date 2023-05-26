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

#ifndef SKIABITMAP_H
#define SKIABITMAP_H

#include "include/core/SkBitmap.h"

#include "impl_interface/bitmap_impl.h"

namespace OHOS {
namespace Rosen {
namespace Drawing {
class SkiaBitmap : public BitmapImpl {
public:
    static inline constexpr AdapterType TYPE = AdapterType::SKIA_ADAPTER;
    SkiaBitmap();
    ~SkiaBitmap() override {}
    AdapterType GetType() const override
    {
        return AdapterType::SKIA_ADAPTER;
    }

    void Build(const int width, const int height, const BitmapFormat& format) override;
    int GetWidth() override;
    int GetHeight() override;
    void* GetPixels() override;
    void SetPixels(void* pixels) override;
    const SkBitmap& ExportSkiaBitmap() const;
    void CopyPixels(Bitmap& dst, int srcLeft, int srcTop, int width, int height) const override;
    void ClearWithColor(const ColorQuad& color) const override;
    ColorQuad GetColor(int x, int y) const override;
    void Free() override;
    bool IsValid() const override;

private:
    SkBitmap skiaBitmap_;
};
} // namespace Drawing
} // namespace Rosen
} // namespace OHOS
#endif
