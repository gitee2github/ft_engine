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

#include "skia_shader_effect.h"

#include <vector>

#include "include/core/SkMatrix.h"
#include "include/core/SkTileMode.h"
#include "include/effects/SkGradientShader.h"
#if defined(USE_CANVASKIT0310_SKIA)
#include "include/core/SkSamplingOptions.h"
#endif

#include "skia_image.h"
#include "skia_matrix.h"
#include "skia_picture.h"

#include "effect/shader_effect.h"
#include "image/image.h"
#include "image/picture.h"
#include "utils/matrix.h"

namespace OHOS {
namespace Rosen {
namespace Drawing {
SkiaShaderEffect::SkiaShaderEffect() noexcept : shader_(nullptr) {}

void SkiaShaderEffect::InitWithColor(ColorQuad color)
{
    shader_ = SkShaders::Color(color);
}

void SkiaShaderEffect::InitWithBlend(const ShaderEffect& s1, const ShaderEffect& s2, BlendMode mode)
{
    auto dst = s1.GetImpl<SkiaShaderEffect>();
    auto src = s2.GetImpl<SkiaShaderEffect>();
    if (dst != nullptr && src != nullptr) {
        shader_ = SkShaders::Blend(static_cast<SkBlendMode>(mode), dst->GetShader(), src->GetShader());
    }
}

void SkiaShaderEffect::InitWithImage(
    const Image& image, TileMode tileX, TileMode tileY, const SamplingOptions& sampling, const Matrix& matrix)
{
    SkTileMode modeX = static_cast<SkTileMode>(tileX);
    SkTileMode modeY = static_cast<SkTileMode>(tileY);

    auto m = matrix.GetImpl<SkiaMatrix>();
    auto i = image.GetImpl<SkiaImage>();
    SkMatrix skiaMatrix;
    sk_sp<SkImage> skiaImage;
    if (m != nullptr && i != nullptr) {
        skiaMatrix = m->ExportSkiaMatrix();
        skiaImage = i->GetImage();
        if (skiaImage != nullptr) {
#if defined(USE_CANVASKIT0310_SKIA)
            SkSamplingOptions samplingOptions;
            if (sampling.GetUseCubic()) {
                samplingOptions = SkSamplingOptions({ sampling.GetCubicCoffB(), sampling.GetCubicCoffC() });
            } else {
                samplingOptions = SkSamplingOptions(static_cast<SkFilterMode>(sampling.GetFilterMode()),
                    static_cast<SkMipmapMode>(sampling.GetMipmapMode()));
            }
            shader_ = skiaImage->makeShader(modeX, modeY, samplingOptions, &skiaMatrix);
#else
            shader_ = skiaImage->makeShader(modeX, modeY, &skiaMatrix);
#endif
        }
    }
}

void SkiaShaderEffect::InitWithPicture(
    const Picture& picture, TileMode tileX, TileMode tileY, FilterMode mode, const Matrix& matrix, const Rect& rect)
{
    SkTileMode modeX = static_cast<SkTileMode>(tileX);
    SkTileMode modeY = static_cast<SkTileMode>(tileY);
    SkRect r = SkRect::MakeLTRB(rect.GetLeft(), rect.GetTop(), rect.GetRight(), rect.GetBottom());

    auto p = picture.GetImpl<SkiaPicture>();
    auto m = matrix.GetImpl<SkiaMatrix>();
    sk_sp<SkPicture> skiaPicture;
    SkMatrix skiaMatrix;
    if (p != nullptr && m != nullptr) {
        skiaPicture = p->GetPicture();
        skiaMatrix = m->ExportSkiaMatrix();
        if (skiaPicture != nullptr) {
#if defined(USE_CANVASKIT0310_SKIA)
            SkFilterMode m = static_cast<SkFilterMode>(mode);
            shader_ = skiaPicture->makeShader(modeX, modeY, m, &skiaMatrix, &r);
#else
            shader_ = skiaPicture->makeShader(modeX, modeY, &skiaMatrix, &r);
#endif
        }
    }
}

void SkiaShaderEffect::InitWithLinearGradient(const Point& startPt, const Point& endPt,
    const std::vector<ColorQuad>& colors, const std::vector<scalar>& pos, TileMode mode)
{
    SkPoint pts[2];
    pts[0].set(startPt.GetX(), startPt.GetY());
    pts[1].set(endPt.GetX(), endPt.GetY());

    size_t count = (colors.size() == pos.size()) ? colors.size() : 0;
    if (count == 0) {
        return;
    }
    std::vector<SkColor> c;
    std::vector<SkScalar> p;
    for (size_t i = 0; i < count; ++i) {
        c.emplace_back(colors[i]);
        p.emplace_back(pos[i]);
    }
    shader_ = SkGradientShader::MakeLinear(pts, &c[0], &p[0], count, static_cast<SkTileMode>(mode));
}

void SkiaShaderEffect::InitWithRadialGradient(const Point& centerPt, scalar radius,
    const std::vector<ColorQuad>& colors, const std::vector<scalar>& pos, TileMode mode)
{
    SkPoint center;
    center.set(centerPt.GetX(), centerPt.GetY());

    size_t count = (colors.size() == pos.size()) ? colors.size() : 0;
    if (count == 0) {
        return;
    }
    std::vector<SkColor> c;
    std::vector<SkScalar> p;
    for (size_t i = 0; i < count; ++i) {
        c.emplace_back(colors[i]);
        p.emplace_back(pos[i]);
    }
    shader_ = SkGradientShader::MakeRadial(center, radius, &c[0], &p[0], count, static_cast<SkTileMode>(mode));
}

void SkiaShaderEffect::InitWithTwoPointConical(const Point& startPt, scalar startRadius, const Point& endPt,
    scalar endRadius, const std::vector<ColorQuad>& colors, const std::vector<scalar>& pos, TileMode mode)
{
    SkPoint start;
    SkPoint end;
    start.set(startPt.GetX(), startPt.GetY());
    end.set(endPt.GetX(), endPt.GetY());

    size_t count = (colors.size() == pos.size()) ? colors.size() : 0;
    if (count == 0) {
        return;
    }
    std::vector<SkColor> c;
    std::vector<SkScalar> p;
    for (size_t i = 0; i < count; ++i) {
        c.emplace_back(colors[i]);
        p.emplace_back(pos[i]);
    }
    shader_ = SkGradientShader::MakeTwoPointConical(
        start, startRadius, end, endRadius, &c[0], &p[0], count, static_cast<SkTileMode>(mode));
}

void SkiaShaderEffect::InitWithSweepGradient(const Point& centerPt, const std::vector<ColorQuad>& colors,
    const std::vector<scalar>& pos, TileMode mode, scalar startAngle, scalar endAngle)
{
    size_t count = (colors.size() == pos.size()) ? colors.size() : 0;
    if (count == 0) {
        return;
    }
    std::vector<SkColor> c;
    std::vector<SkScalar> p;
    for (size_t i = 0; i < count; ++i) {
        c.emplace_back(colors[i]);
        p.emplace_back(pos[i]);
    }
    shader_ = SkGradientShader::MakeSweep(centerPt.GetX(), centerPt.GetY(), &c[0], &p[0], count,
        static_cast<SkTileMode>(mode), startAngle, endAngle, 0, nullptr);
}

sk_sp<SkShader> SkiaShaderEffect::GetShader() const
{
    return shader_;
}
} // namespace Drawing
} // namespace Rosen
} // namespace OHOS