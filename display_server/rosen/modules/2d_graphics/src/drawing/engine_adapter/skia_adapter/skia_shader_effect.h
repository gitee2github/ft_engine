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

#ifndef SKIA_SHADER_EFFECT_H
#define SKIA_SHADER_EFFECT_H

#include "include/core/SkShader.h"

#include "impl_interface/shader_effect_impl.h"

namespace OHOS {
namespace Rosen {
namespace Drawing {
class SkiaShaderEffect : public ShaderEffectImpl {
public:
    static inline constexpr AdapterType TYPE = AdapterType::SKIA_ADAPTER;
    SkiaShaderEffect() noexcept;
    ~SkiaShaderEffect() override {};
    AdapterType GetType() const override
    {
        return AdapterType::SKIA_ADAPTER;
    }

    void InitWithColor(ColorQuad color) override;
    void InitWithBlend(const ShaderEffect& s1, const ShaderEffect& s2, BlendMode mode) override;
    void InitWithImage(const Image& image, TileMode tileX, TileMode tileY, const SamplingOptions& sampling,
        const Matrix& matrix) override;
    void InitWithPicture(const Picture& picture, TileMode tileX, TileMode tileY, FilterMode mode, const Matrix& matrix,
        const Rect& rect) override;
    void InitWithLinearGradient(const Point& startPt, const Point& endPt, const std::vector<ColorQuad>& colors,
        const std::vector<scalar>& pos, TileMode mode) override;
    void InitWithRadialGradient(const Point& centerPt, scalar radius, const std::vector<ColorQuad>& colors,
        const std::vector<scalar>& pos, TileMode mode) override;
    void InitWithTwoPointConical(const Point& startPt, scalar startRadius, const Point& endPt, scalar endRadius,
        const std::vector<ColorQuad>& colors, const std::vector<scalar>& pos, TileMode mode) override;
    void InitWithSweepGradient(const Point& centerPt, const std::vector<ColorQuad>& colors,
        const std::vector<scalar>& pos, TileMode mode, scalar startAngle, scalar endAngle) override;
    sk_sp<SkShader> GetShader() const;

private:
    sk_sp<SkShader> shader_;
};
} // namespace Drawing
} // namespace Rosen
} // namespace OHOS
#endif