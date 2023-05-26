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

#ifndef SKIA_PATH_H
#define SKIA_PATH_H

#include "include/core/SkPath.h"

#include "impl_interface/path_impl.h"

namespace OHOS {
namespace Rosen {
namespace Drawing {
class SkiaPath : public PathImpl {
public:
    static inline constexpr AdapterType TYPE = AdapterType::SKIA_ADAPTER;
    SkiaPath() noexcept;
    ~SkiaPath() override {};
    SkiaPath(const SkiaPath& p) noexcept;
    SkiaPath &operator=(const SkiaPath& p) noexcept;
    AdapterType GetType() const override
    {
        return AdapterType::SKIA_ADAPTER;
    }
    PathImpl* Clone() override;
    void MoveTo(scalar x, scalar y) override;
    void LineTo(scalar x, scalar y) override;
    void ArcTo(scalar pt1X, scalar pt1Y, scalar pt2X, scalar pt2Y, scalar startAngle, scalar sweepAngle) override;
    void ArcTo(scalar rx, scalar ry, scalar angle, PathDirection direction, scalar endX, scalar endY) override;
    void CubicTo(
        scalar ctrlPt1X, scalar ctrlPt1Y, scalar ctrlPt2X, scalar ctrlPt2Y, scalar endPtX, scalar endPtY) override;
    void QuadTo(scalar ctrlPtX, scalar ctrlPtY, scalar endPtX, scalar endPtY) override;

    void AddRect(scalar left, scalar top, scalar right, scalar bottom, PathDirection dir) override;
    void AddOval(scalar left, scalar top, scalar right, scalar bottom, PathDirection dir) override;
    void AddArc(scalar left, scalar top, scalar right, scalar bottom, scalar startAngle, scalar sweepAngle) override;
    void AddPoly(const std::vector<Point>& points, int count, bool close) override;
    void AddCircle(scalar x, scalar y, scalar radius, PathDirection dir) override;
    void AddRoundRect(scalar left, scalar top, scalar right, scalar bottom, scalar xRadius, scalar yRadius,
        PathDirection dir) override;

    void AddPath(const Path& src, scalar dx, scalar dy) override;
    void AddPath(const Path& src) override;
    void AddPathWithMatrix(const Path& src, const Matrix& matrix) override;

    Rect GetBounds() const override;
    void SetFillStyle(PathFillType fillstyle) override;

    bool Interpolate(const Path& ending, scalar weight, Path& out) override;
    void Transform(const Matrix& matrix) override;
    void Offset(scalar dx, scalar dy) override;
    bool OpWith(const Path& path1, const Path& path2, PathOp op) override;

    void Reset() override;

    void Close() override;

    void SetPath(const SkPath& path);

    const SkPath& GetPath() const;

private:
    SkPath path_;
};
} // namespace Drawing
} // namespace Rosen
} // namespace OHOS
#endif