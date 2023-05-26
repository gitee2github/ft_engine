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

#include "skia_path_effect.h"

#include <memory>

#include "include/core/SkPathEffect.h"
#include "include/effects/Sk1DPathEffect.h"
#include "include/effects/SkCornerPathEffect.h"
#include "include/effects/SkDashPathEffect.h"
#include "skia_path.h"

#include "effect/path_effect.h"

namespace OHOS {
namespace Rosen {
namespace Drawing {
SkiaPathEffect::SkiaPathEffect() noexcept : pathEffect_(nullptr) {}

void SkiaPathEffect::InitWithDash(const scalar intervals[], int count, scalar phase)
{
    pathEffect_ = SkDashPathEffect::Make(intervals, count, phase);
}

void SkiaPathEffect::InitWithPathDash(const Path& path, scalar advance, scalar phase, PathDashStyle style)
{
    auto p = path.GetImpl<SkiaPath>();
    if (p != nullptr) {
        pathEffect_ =
            SkPath1DPathEffect::Make(p->GetPath(), advance, phase, static_cast<SkPath1DPathEffect::Style>(style));
    }
}

void SkiaPathEffect::InitWithCorner(scalar radius)
{
    pathEffect_ = SkCornerPathEffect::Make(radius);
}

void SkiaPathEffect::InitWithSum(const PathEffect& e1, const PathEffect& e2)
{
    auto first = e1.GetImpl<SkiaPathEffect>();
    auto second = e2.GetImpl<SkiaPathEffect>();
    if (first != nullptr && second != nullptr) {
        pathEffect_ = SkPathEffect::MakeSum(first->GetPathEffect(), second->GetPathEffect());
    }
}

void SkiaPathEffect::InitWithCompose(const PathEffect& e1, const PathEffect& e2)
{
    auto outer = e1.GetImpl<SkiaPathEffect>();
    auto inner = e2.GetImpl<SkiaPathEffect>();
    if (outer != nullptr && inner != nullptr) {
        pathEffect_ = SkPathEffect::MakeCompose(outer->GetPathEffect(), inner->GetPathEffect());
    }
}

sk_sp<SkPathEffect> SkiaPathEffect::GetPathEffect() const
{
    return pathEffect_;
}
} // namespace Drawing
} // namespace Rosen
} // namespace OHOS