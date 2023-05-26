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

#include "image_test.h"

namespace OHOS {
namespace Rosen {
namespace Drawing {
void ImageTest::TestDrawImage(Canvas& canvas, uint32_t width, uint32_t height)
{
    LOGI("+++++++ TestDrawImage");
    Bitmap bmp;
    BitmapFormat format { COLORTYPE_RGBA_8888, ALPHATYPE_OPAQUE };
    bmp.Build(300, 300, format); // bitmap width and height
    bmp.ClearWithColor(Drawing::Color::COLOR_BLUE);

    Image image;
    image.BuildFromBitmap(bmp);
    int imageWidth = image.GetWidth();
    int imageHeight = image.GetHeight();
    LOGI("image width = %{public}d, image height = %{public}d", imageWidth, imageHeight);
    Matrix matrix;
    // Set matrix to rotate by degrees 45 about a pivot point at (0, 0).
    matrix.Rotate(45, 0, 0);
    SamplingOptions sampling = SamplingOptions(Drawing::FilterMode::NEAREST, Drawing::MipmapMode::NEAREST);
    auto e = ShaderEffect::CreateImageShader(image, TileMode::REPEAT, TileMode::MIRROR, sampling, matrix);
    LOGI("sampling useCubic = %{public}d, filter = %{public}d, mipmap = %{public}d", sampling.GetUseCubic(),
        sampling.GetFilterMode(), sampling.GetMipmapMode());
    auto c = Drawing::ColorSpace::CreateRefImage(image);

    Pen pen;
    pen.SetAntiAlias(true);
    pen.SetColor(Drawing::Color::COLOR_BLUE);
    pen.SetColor(pen.GetColor4f(), c);
    pen.SetWidth(10); // The thickness of the pen is 10
    pen.SetShaderEffect(e);
    canvas.AttachPen(pen);
    canvas.DrawImage(image, 500, 500, sampling); // Draw image at (500,500)

    LOGI("------- TestDrawImage");
}

void ImageTest::TestDrawImageRect(Canvas& canvas, uint32_t width, uint32_t height)
{
    LOGI("+++++++ TestDrawImageRect");
    Bitmap bmp;
    BitmapFormat format { COLORTYPE_RGBA_8888, ALPHATYPE_OPAQUE };
    bmp.Build(300, 300, format); // bitmap width and height
    bmp.ClearWithColor(Drawing::Color::COLOR_BLUE);

    Image image;
    image.BuildFromBitmap(bmp);
    Drawing::Rect r1(100, 100, 200, 200); // rect is set to (fLeft, fTop, fRight, fBottom)
    Drawing::Rect r2(300, 300, 500, 500);
    SamplingOptions sampling = SamplingOptions(Drawing::FilterMode::NEAREST, Drawing::MipmapMode::NEAREST);

    Brush brush;
    brush.SetColor(Drawing::Color::COLOR_RED);
    canvas.AttachBrush(brush);
    canvas.DrawImageRect(image, r1, r2, sampling, SrcRectConstraint::STRICT_SRC_RECT_CONSTRAINT);

    LOGI("------- TestDrawImageRect");
}

std::vector<ImageTest::TestFunc> ImageTest::ImageTestCase()
{
    std::vector<TestFunc> testFuncVec;
    testFuncVec.push_back(TestDrawImage);
    testFuncVec.push_back(TestDrawImageRect);
    return testFuncVec;
}
} // namespace Drawing
} // namespace Rosen
} // namespace OHOS