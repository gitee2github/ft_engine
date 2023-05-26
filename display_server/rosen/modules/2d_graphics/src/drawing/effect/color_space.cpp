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

#include "effect/color_space.h"

#include "impl_factory.h"

namespace OHOS {
namespace Rosen {
namespace Drawing {
ColorSpace::ColorSpace(ColorSpaceType t) noexcept : ColorSpace()
{
    type_ = t;
    switch (type_) {
        case ColorSpace::ColorSpaceType::SRGB:
            impl_->InitWithSRGB();
            break;
        case ColorSpace::ColorSpaceType::SRGB_LINEAR:
            impl_->InitWithSRGBLinear();
            break;
        default:
            break;
    }
}

ColorSpace::ColorSpace(ColorSpaceType t, const Image& image) noexcept : ColorSpace()
{
    type_ = t;
    impl_->InitWithImage(image);
}

ColorSpace::ColorSpace() noexcept
    : type_(ColorSpace::ColorSpaceType::NO_TYPE), impl_(ImplFactory::CreateColorSpaceImpl())
{}

ColorSpace::ColorSpaceType ColorSpace::GetType() const
{
    return type_;
}

std::shared_ptr<ColorSpace> ColorSpace::CreateSRGB()
{
    return std::make_shared<ColorSpace>(ColorSpace::ColorSpaceType::SRGB);
}

std::shared_ptr<ColorSpace> ColorSpace::CreateSRGBLinear()
{
    return std::make_shared<ColorSpace>(ColorSpace::ColorSpaceType::SRGB_LINEAR);
}

std::shared_ptr<ColorSpace> ColorSpace::CreateRefImage(const Image& image)
{
    return std::make_shared<ColorSpace>(ColorSpace::ColorSpaceType::REF_IMAGE, image);
}
} // namespace Drawing
} // namespace Rosen
} // namespace OHOS