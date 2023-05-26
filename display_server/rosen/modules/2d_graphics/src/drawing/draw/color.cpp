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

#include "draw/color.h"
#include <algorithm>

namespace OHOS {
namespace Rosen {
namespace Drawing {
Color::Color() noexcept : alpha_(RGB_MAX), red_(0), green_(0), blue_(0) {}

Color::Color(const Color& c) noexcept : alpha_(c.GetAlpha()), red_(c.GetRed()), green_(c.GetGreen()), blue_(c.GetBlue())
{}

Color::Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept : alpha_(a), red_(r), green_(g), blue_(b) {}

// Return alpha byte, red component, green component and blue component of color rgba.
Color::Color(ColorQuad rgba) noexcept
{
    alpha_ = rgba >> 24;
    red_ = (rgba >> 16) & 0xff;
    green_ = (rgba >> 8) & 0xff;
    blue_ = (rgba >> 0) & 0xff;
}

uint32_t Color::GetRed() const
{
    return red_;
}

uint32_t Color::GetGreen() const
{
    return green_;
}

uint32_t Color::GetBlue() const
{
    return blue_;
}

uint32_t Color::GetAlpha() const
{
    return alpha_;
}

void Color::SetRed(uint32_t r)
{
    red_ = r;
}

void Color::SetGreen(uint32_t g)
{
    green_ = g;
}

void Color::SetBlue(uint32_t b)
{
    blue_ = b;
}

void Color::SetAlpha(uint32_t a)
{
    alpha_ = a;
}

scalar Color::GetRedF() const
{
    return static_cast<scalar>(red_) / RGB_MAX;
}

scalar Color::GetGreenF() const
{
    return static_cast<scalar>(green_) / RGB_MAX;
}

scalar Color::GetBlueF() const
{
    return static_cast<scalar>(blue_) / RGB_MAX;
}

scalar Color::GetAlphaF() const
{
    return static_cast<scalar>(alpha_) / RGB_MAX;
}

Color4f Color::GetColor4f()
{
    color4f_.redF_ = GetRedF();
    color4f_.greenF_ = GetGreenF();
    color4f_.blueF_ = GetBlueF();
    color4f_.alphaF_ = GetAlphaF();
    return color4f_;
}

void Color::SetRedF(scalar r)
{
    red_ = static_cast<uint8_t>(std::clamp(r, 0.0f, 1.0f) * RGB_MAX);
}

void Color::SetGreenF(scalar g)
{
    green_ = static_cast<uint8_t>(std::clamp(g, 0.0f, 1.0f) * RGB_MAX);
}

void Color::SetBlueF(scalar b)
{
    blue_ = static_cast<uint8_t>(std::clamp(b, 0.0f, 1.0f) * RGB_MAX);
}

void Color::SetAlphaF(scalar a)
{
    alpha_ = static_cast<uint8_t>(std::clamp(a, 0.0f, 1.0f) * RGB_MAX);
}

void Color::SetRgb(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    alpha_ = a;
    red_ = r;
    green_ = g;
    blue_ = b;
}

void Color::SetRgbF(scalar r, scalar g, scalar b, scalar a)
{
    alpha_ = static_cast<uint32_t>(round(std::clamp(a, 0.0f, 1.0f) * RGB_MAX));
    red_ = static_cast<uint32_t>(round(std::clamp(r, 0.0f, 1.0f) * RGB_MAX));
    green_ = static_cast<uint32_t>(round(std::clamp(g, 0.0f, 1.0f) * RGB_MAX));
    blue_ = static_cast<uint32_t>(round(std::clamp(b, 0.0f, 1.0f) * RGB_MAX));
}

void Color::SetColorQuad(uint32_t c)
{
    alpha_ = Color::ColorQuadGetA(c);
    red_ = Color::ColorQuadGetR(c);
    green_ = Color::ColorQuadGetG(c);
    blue_ = Color::ColorQuadGetB(c);
}

ColorQuad Color::CastToColorQuad() const
{
    return Color::ColorQuadSetARGB(red_, green_, blue_, alpha_);
}

bool operator==(const Color& c1, const Color& c2)
{
    return c1.alpha_ == c2.alpha_ && c1.red_ == c2.red_ && c1.green_ == c2.green_ && c1.blue_ == c2.blue_;
}
bool operator!=(const Color& c1, const Color& c2)
{
    return c1.alpha_ != c2.alpha_ || c1.red_ != c2.red_ || c1.green_ != c2.green_ || c1.blue_ != c2.blue_;
}
} // namespace Drawing
} // namespace Rosen
} // namespace OHOS
