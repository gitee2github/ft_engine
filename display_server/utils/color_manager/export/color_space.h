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

#ifndef COLORSPACE
#define COLORSPACE

#include <array>
#include <cfloat>
#include <cmath>
#include <map>
#include <utility>

#include "include/core/SkColorSpace.h"
#include "include/third_party/skcms/skcms.h"

namespace OHOS {
namespace ColorManager {
#define DIMES_3 3
#define DIMES_2 2

using Vector3 = std::array<float, DIMES_3>;
using Matrix3x3 = std::array<Vector3, DIMES_3>;
static constexpr float COLOR_EPSILON = 0.0018f; // 1/255/2 = 0.00196
static const std::array<float, DIMES_2> ILLUMINANT_D50_XY = {0.34567f, 0.35850f};
static const Vector3 ILLUMINANT_D50_XYZ = {0.964212f, 1.0f, 0.825188f};
static const Matrix3x3 BRADFORD = {{
    {0.8951f, 0.2664f, -0.1614f},
    {-0.7502f, 1.7135f, 0.0367f},
    {0.0389f, -0.0685f, 1.0296f}}};
static const Matrix3x3 BRADFORD_INV = {{
    {0.9869929f, -0.1470543f, 0.1599627f},
    {0.4323053f,  0.5183603f, 0.0492912f},
    {-0.0085287f, 0.0400428f, 0.9684867f},
}};

enum ColorSpaceName : uint32_t {
    NONE = 0,
    ACES,
    ACESCG,
    ADOBE_RGB,
    BT2020,
    BT709,
    CIE_LAB,
    CIE_XYZ,
    DCI_P3,
    DISPLAY_P3,
    EXTENDED_SRGB,
    LINEAR_EXTENDED_SRGB,
    LINEAR_SRGB,
    NTSC_1953,
    PRO_PHOTO_RGB,
    SMPTE_C,
    SRGB,
    CUSTOM,
};

enum GamutMappingMode : uint32_t {
    GAMUT_MAP_CONSTANT = 0,
    GAMUT_MAP_EXPENSION,
    GAMUT_MAP_HDR_CONSTANT,
    GAMUT_MAP_HDR_EXPENSION,
};

struct ColorSpacePrimaries {
    float rX;
    float rY;
    float gX;
    float gY;
    float bX;
    float bY;
    float wX;
    float wY;
};

struct TransferFunc {
    float g;
    float a;
    float b;
    float c;
    float d;
    float e;
    float f;
};

Matrix3x3 operator*(const Matrix3x3& a, const Matrix3x3& b);

Vector3 operator*(const Vector3& x, const Matrix3x3& a);

Vector3 operator*(const Matrix3x3& a, const Vector3& x);

Matrix3x3 operator/(const Vector3& a, const Vector3& b);

Matrix3x3 Invert(const Matrix3x3& src);

inline Vector3 XYZ(const Vector3& xyY)
{
    return Vector3 {(xyY[0] * xyY[2]) / xyY[1], xyY[2],
        ((1 - xyY[0] - xyY[1]) * xyY[2]) / xyY[1]};
}

inline bool FloatEqual(const float src, const float dst)
{
    return fabs(src - dst) < COLOR_EPSILON;
}

inline std::array<float, DIMES_2> ComputeWhitePoint(Matrix3x3 &toXYZ)
{
    Vector3 w = toXYZ * Vector3 {1.0f};
    float sumW = w[0] + w[1] + w[2];
    return {{w[0] / sumW, w[1] / sumW}};
}

inline Matrix3x3 SkToXYZToMatrix3(const skcms_Matrix3x3 &skToXYZ)
{
    return {{{skToXYZ.vals[0][0], skToXYZ.vals[0][1], skToXYZ.vals[0][2]},
        {skToXYZ.vals[1][0], skToXYZ.vals[1][1], skToXYZ.vals[1][2]},
        {skToXYZ.vals[2][0], skToXYZ.vals[2][1], skToXYZ.vals[2][2]}}};
}

// Compute a toXYZD50 matrix from a given rgb and white point
Matrix3x3 ComputeXYZD50(const ColorSpacePrimaries& primaries);

// Convert toXYZ to toXYZD50 matrix with its white point
Matrix3x3 DXToD50(const Matrix3x3 &toXYZ, const std::array<float, DIMES_2> &wp);

class ColorSpace {
public:
    ColorSpace(ColorSpaceName name);

    ColorSpace(const ColorSpacePrimaries &primaries, const TransferFunc &transferFunc);

    ColorSpace(const ColorSpacePrimaries &primaries, float gamma);

    ColorSpace(const Matrix3x3& toXYZ, const std::array<float, 2> &whitePoint, const TransferFunc& transferFunc);

    ColorSpace(const Matrix3x3& toXYZ, const std::array<float, 2>& whitePoint, float gamma);

    // convert SKColorSpace to OHOS ColorSpce
    ColorSpace(const sk_sp<SkColorSpace> src, ColorSpaceName name = ColorSpaceName::CUSTOM);

    ColorSpace(const skcms_ICCProfile& srcIcc, ColorSpaceName name = ColorSpaceName::CUSTOM);

    ColorSpaceName GetColorSpaceName() const
    {
        return colorSpaceName;
    }

    Matrix3x3 GetRGBToXYZ() const
    {
        return toXYZ;
    }

    Matrix3x3 GetXYZToRGB() const
    {
        auto toRGB = Invert(toXYZ);
        return toRGB;
    }

    std::array<float, DIMES_2> GetWhitePoint() const
    {
        return whitePoint;
    }

    float GetGamma() const
    {
        return transferFunc.g;
    }

    Vector3 ToLinear(Vector3 color) const;
    Vector3 ToNonLinear(Vector3 color) const;
    
    // convert OHOS ColorSpce to SKColorSpace
    sk_sp<SkColorSpace> ToSkColorSpace() const;

    float clampMin = 0.0f;
    float clampMax = 1.0f;

private:
    skcms_Matrix3x3 ToSkiaXYZ() const;

    ColorSpaceName colorSpaceName = ColorSpaceName::SRGB;
    Matrix3x3 toXYZ;
    std::array<float, DIMES_2> whitePoint;
    TransferFunc transferFunc = {};
};
} // namespace ColorSpace
} // namespace OHOS
#endif  // COLORSPACE
