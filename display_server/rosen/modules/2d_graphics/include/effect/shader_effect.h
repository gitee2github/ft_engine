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

#ifndef SHADER_EFFECT_H
#define SHADER_EFFECT_H

#include "drawing/engine_adapter/impl_interface/shader_effect_impl.h"

namespace OHOS {
namespace Rosen {
namespace Drawing {
enum class TileMode {
    CLAMP,
    REPEAT,
    MIRROR,
};

class ShaderEffect {
public:
    enum class ShaderEffectType {
        NO_TYPE,
        COLOR,
        BLEND,
        IMAGE,
        PICTURE,
        LINEAR_GRADIENT,
        RADIAL_GRADIENT,
        CONICAL_GRADIENT,
        SWEEP_GRADIENT,
    };

    static std::shared_ptr<ShaderEffect> CreateColorShader(ColorQuad color);
    static std::shared_ptr<ShaderEffect> CreateBlendShader(ShaderEffect& dst, ShaderEffect& src, BlendMode mode);
    static std::shared_ptr<ShaderEffect> CreateImageShader(
        const Image& image, TileMode tileX, TileMode tileY, const SamplingOptions& sampling, const Matrix& matrix);
    static std::shared_ptr<ShaderEffect> CreatePictureShader(const Picture& picture, TileMode tileX, TileMode tileY,
        FilterMode mode, const Matrix& matrix, const Rect& rect);
    static std::shared_ptr<ShaderEffect> CreateLinearGradient(const Point& startPt, const Point& endPt,
        const std::vector<ColorQuad>& colors, const std::vector<scalar>& pos, TileMode mode);
    static std::shared_ptr<ShaderEffect> CreateRadialGradient(const Point& centerPt, scalar radius,
        const std::vector<ColorQuad>& colors, const std::vector<scalar>& pos, TileMode mode);
    static std::shared_ptr<ShaderEffect> CreateTwoPointConical(const Point& startPt, scalar startRadius,
        const Point& endPt, scalar endRadius, const std::vector<ColorQuad>& colors, const std::vector<scalar>& pos,
        TileMode mode);
    static std::shared_ptr<ShaderEffect> CreateSweepGradient(const Point& centerPt,
        const std::vector<ColorQuad>& colors, const std::vector<scalar>& pos, TileMode mode, scalar startAngle,
        scalar endAngle);

    ~ShaderEffect() {}
    ShaderEffectType GetType() const;
    template<typename T>
    const std::shared_ptr<T> GetImpl() const
    {
        return impl_->DowncastingTo<T>();
    }

    ShaderEffect(ShaderEffectType t, ColorQuad color) noexcept;
    ShaderEffect(ShaderEffectType t, ShaderEffect& dst, ShaderEffect& src, BlendMode mode) noexcept;
    ShaderEffect(ShaderEffectType t, const Image& image, TileMode tileX, TileMode tileY,
        const SamplingOptions& sampling, const Matrix& matrix) noexcept;
    ShaderEffect(ShaderEffectType t, const Picture& picture, TileMode tileX, TileMode tileY, FilterMode mode,
        const Matrix& matrix, const Rect& rect) noexcept;
    ShaderEffect(ShaderEffectType t, const Point& startPt, const Point& endPt, const std::vector<ColorQuad>& colors,
        const std::vector<scalar>& pos, TileMode mode) noexcept;
    ShaderEffect(ShaderEffectType t, const Point& centerPt, scalar radius, const std::vector<ColorQuad>& colors,
        const std::vector<scalar>& pos, TileMode mode) noexcept;
    ShaderEffect(ShaderEffectType t, const Point& startPt, scalar startRadius, const Point& endPt, scalar endRadius,
        const std::vector<ColorQuad>& colors, const std::vector<scalar>& pos, TileMode mode) noexcept;
    ShaderEffect(ShaderEffectType t, const Point& centerPt, const std::vector<ColorQuad>& colors,
        const std::vector<scalar>& pos, TileMode mode, scalar startAngle, scalar endAngle) noexcept;

protected:
    ShaderEffect() noexcept;

private:
    ShaderEffectType type_;
    std::shared_ptr<ShaderEffectImpl> impl_;
};
} // namespace Drawing
} // namespace Rosen
} // namespace OHOS
#endif
