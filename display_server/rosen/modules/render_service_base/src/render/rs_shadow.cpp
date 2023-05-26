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

#include "render/rs_shadow.h"

#include <algorithm>

namespace OHOS {
namespace Rosen {
RSShadow::RSShadow() {}

RSShadow::~RSShadow() {}

void RSShadow::SetColor(Color color)
{
    color_ = color;
}

void RSShadow::SetOffsetX(float offsetX)
{
    offsetX_ = offsetX;
}

void RSShadow::SetOffsetY(float offsetY)
{
    offsetY_ = offsetY;
}

void RSShadow::SetAlpha(float alpha)
{
    color_.SetAlpha(std::clamp(alpha, 0.0f, 1.0f) * UINT8_MAX);
}

void RSShadow::SetElevation(float elevation)
{
    isHardwareAcceleration_ = true;
    elevation_ = elevation;
}

void RSShadow::SetRadius(float radius)
{
    isHardwareAcceleration_ = false;
    radius_ = radius;
}

void RSShadow::SetPath(std::shared_ptr<RSPath> path)
{
    path_ = path;
}

Color RSShadow::GetColor() const
{
    return color_;
}

float RSShadow::GetOffsetX() const
{
    return offsetX_;
}

float RSShadow::GetOffsetY() const
{
    return offsetY_;
}

float RSShadow::GetAlpha() const
{
    return static_cast<float>(color_.GetAlpha()) / UINT8_MAX;
}

float RSShadow::GetElevation() const
{
    return elevation_;
}

float RSShadow::GetRadius() const
{
    return radius_;
}

std::shared_ptr<RSPath> RSShadow::GetPath() const
{
    return path_;
}

bool RSShadow::IsValid() const
{
    if (isHardwareAcceleration_) {
        return GetAlpha() > 0.f;
    } else {
        return radius_ > 0.f;
    }
}
} // namespace Rosen
} // namespace OHOS
