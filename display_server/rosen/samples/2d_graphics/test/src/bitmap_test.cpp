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

#include "bitmap_test.h"

using namespace OHOS::Media;

namespace OHOS {
namespace Rosen {
namespace Drawing {
using TestFunc = std::function<void(Canvas&, uint32_t, uint32_t)>;
void BitmapTest::TestDrawBitmap(Canvas& canvas, uint32_t width, uint32_t height)
{
    LOGI("+++++++ TestDrawBitmap");
    Bitmap bmp;
    BitmapFormat format { COLORTYPE_RGBA_8888, ALPHATYPE_OPAQUE };
    bmp.Build(200, 200, format); // bitmap width and height
    bmp.ClearWithColor(Drawing::Color::COLOR_BLUE);

    Pen pen;
    pen.SetAntiAlias(true);
    pen.SetColor(Drawing::Color::COLOR_BLUE);
    pen.SetWidth(10); // The thickness of the pen is 10
    canvas.AttachPen(pen);
    canvas.DrawBitmap(bmp, 500, 500); // draw bitmap at (fx, fy)

    LOGI("------- TestDrawBitmap");
}

std::vector<BitmapTest::TestFunc> BitmapTest::BitmapTestCase()
{
    std::vector<TestFunc> testFuncVec;
    testFuncVec.push_back(TestDrawBitmap);
    return testFuncVec;
}
} // namespace Drawing
} // namespace Rosen
} // namespace OHOS
