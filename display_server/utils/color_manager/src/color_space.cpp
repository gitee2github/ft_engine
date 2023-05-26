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

#include "color_space.h"

namespace OHOS {
namespace ColorManager {
const ColorSpace CS_SRGB = {
    ColorSpacePrimaries {0.640f, 0.330f, 0.300f, 0.600f, 0.150f, 0.060f, 0.3127f, 0.3290f},
    {2.4f, 1 / 1.055f, 0.055f / 1.055f, 1 / 12.92f, 0.04045f, 0.0f, 0.0f}};

const ColorSpace CS_LINEAR_SRGB = {
    ColorSpacePrimaries {0.640f, 0.330f, 0.300f, 0.600f, 0.150f, 0.060f, 0.3127f, 0.3290f},
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};

const ColorSpace CS_NTSC_1953 = {
    ColorSpacePrimaries {0.67f, 0.33f, 0.21f, 0.71f, 0.14f, 0.08f, 0.310f, 0.316f},
    {1 / 0.45f, 1 / 1.099f, 0.099f / 1.099f, 1 / 4.5f, 0.081f, 0.0f, 0.0f}};

const ColorSpace CS_BT709 = {
    ColorSpacePrimaries {0.640f, 0.330f, 0.300f, 0.600f, 0.150f, 0.060f, 0.3127f, 0.3290f},
    {1 / 0.45f, 1 / 1.099f, 0.099f / 1.099f, 1 / 4.5f, 0.081f, 0.0f, 0.0f}};

const ColorSpace CS_BT2020 = {
    ColorSpacePrimaries {0.708f, 0.292f, 0.170f, 0.797f, 0.131f, 0.046f, 0.3127f, 0.3290f},
    {1 / 0.45f, 1 / 1.099f, 0.099f / 1.099f, 1 / 4.5f, 0.081f, 0.0f, 0.0f}};

const ColorSpace CS_ADOBE_RGB = {
    ColorSpacePrimaries {0.640f, 0.330f, 0.21f, 0.71f, 0.150f, 0.060f, 0.3127f, 0.3290f},
    {2.2f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};

const ColorSpace CS_PRO_PHOTO_RGB = {
    ColorSpacePrimaries {0.7347f, 0.2653f, 0.1596f, 0.8404f, 0.0366f, 0.0001f, 0.34567f, 0.35850f},
    {1.8f, 1.0f, 0.0f, 1 / 16.0f, 0.031248f, 0.0f, 0.0f}};

const ColorSpace CS_DISPLAY_P3 = {
    ColorSpacePrimaries {0.680f, 0.320f, 0.265f, 0.690f, 0.150f, 0.060f, 0.3127f, 0.3290f},
    {2.4f, 1 / 1.055f, 0.055f / 1.055f, 1 / 12.92f, 0.039f, 0.0f, 0.0f}};

const ColorSpace CS_DCI_P3 = {
    ColorSpacePrimaries {0.680f, 0.320f, 0.265f, 0.690f, 0.150f, 0.060f, 0.314f, 0.351f},
    {2.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};

const std::map<ColorSpaceName, ColorSpace> NamedColorSpace = {
    {ColorSpaceName::SRGB, CS_SRGB},
    {ColorSpaceName::LINEAR_SRGB, CS_LINEAR_SRGB},
    {ColorSpaceName::NTSC_1953, CS_NTSC_1953},
    {ColorSpaceName::BT709, CS_BT709},
    {ColorSpaceName::BT2020, CS_BT2020},
    {ColorSpaceName::ADOBE_RGB, CS_ADOBE_RGB},
    {ColorSpaceName::PRO_PHOTO_RGB, CS_PRO_PHOTO_RGB},
    {ColorSpaceName::DISPLAY_P3, CS_DISPLAY_P3},
    {ColorSpaceName::DCI_P3, CS_DCI_P3},
};

ColorSpace::ColorSpace(ColorSpaceName name)
{
    if (NamedColorSpace.find(name) != NamedColorSpace.end()) {
        const ColorSpace &colorSpace = NamedColorSpace.at(name);
        colorSpaceName = name;
        whitePoint = colorSpace.whitePoint;
        toXYZ = colorSpace.toXYZ;
        transferFunc = colorSpace.transferFunc;
    } else {
        colorSpaceName = ColorSpaceName::NONE;
    }
}

ColorSpace::ColorSpace(const ColorSpacePrimaries &primaries, const TransferFunc &transferFunc)
    : colorSpaceName(ColorSpaceName::CUSTOM),
      toXYZ(ComputeXYZD50(primaries)),
      transferFunc(transferFunc)
{
    std::array<float, 2> whiteP = {primaries.wX, primaries.wY};
    whitePoint = whiteP;
}

ColorSpace::ColorSpace(const ColorSpacePrimaries &primaries, float gamma)
    : colorSpaceName(ColorSpaceName::CUSTOM),
      toXYZ(ComputeXYZD50(primaries))
{
    std::array<float, 2> whiteP = {primaries.wX, primaries.wY};
    whitePoint = whiteP;
    transferFunc = {};
    transferFunc.g = gamma;
    transferFunc.a = 1.0f;
}

ColorSpace::ColorSpace(const Matrix3x3& toXYZ, const std::array<float, 2>& whitePoint, const TransferFunc &transferFunc)
    : colorSpaceName(ColorSpaceName::CUSTOM),
      toXYZ(DXToD50(toXYZ, whitePoint)),
      whitePoint(whitePoint),
      transferFunc(transferFunc)
{
}

ColorSpace::ColorSpace(const Matrix3x3 &toXYZ, const std::array<float, 2>& whitePoint, float gamma)
    : colorSpaceName(ColorSpaceName::CUSTOM), toXYZ(DXToD50(toXYZ, whitePoint)), whitePoint(whitePoint)
{
    transferFunc = {};
    transferFunc.g = gamma;
    transferFunc.a = 1.0f;
}

ColorSpace::ColorSpace(const sk_sp<SkColorSpace> src, ColorSpaceName name)
    : colorSpaceName(name)
{
    if (src) {
        float func[7];
        src->transferFn(func);
        transferFunc = {func[0], func[1], func[2], func[3], func[4], func[5], func[6]};
        skcms_Matrix3x3 toXYZD50;
        src->toXYZD50(&toXYZD50);
        toXYZ = SkToXYZToMatrix3(toXYZD50);
        whitePoint = ComputeWhitePoint(toXYZ);
    }
}

ColorSpace::ColorSpace(const skcms_ICCProfile& srcIcc, ColorSpaceName name)
    : colorSpaceName(name)
{
    if (srcIcc.has_toXYZD50 && srcIcc.has_trc) {
        skcms_TransferFunction func = srcIcc.trc[0].parametric;
        transferFunc = {func.g, func.a, func.b, func.c, func.d, func.e, func.f};
        toXYZ = SkToXYZToMatrix3(srcIcc.toXYZD50);
        whitePoint = ComputeWhitePoint(toXYZ);
    }
}

Matrix3x3 operator*(const Matrix3x3& a, const Matrix3x3& b)
{
    Matrix3x3 c = {};
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < b.size(); ++j) {
            for (size_t k = 0; k < b[0].size(); ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

Vector3 operator*(const Vector3& x, const Matrix3x3& a)
{
    Vector3 b = {};
    for (size_t i = 0; i < x.size(); ++i) {
        for (size_t j = 0; j < a.size(); ++j) {
            b[i] += x[j] * a[j][i];
        }
    }
    return b;
}

Vector3 operator*(const Matrix3x3& a, const Vector3& x)
{
    Vector3 b = {};
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < x.size(); ++j) {
            b[i] += a[i][j] * x[j];
        }
    }
    return b;
}

Matrix3x3 operator/(const Vector3& a, const Vector3& b)
{
    Matrix3x3 diag = {};
    for (size_t i = 0; i < a.size(); ++i) {
        diag[i][i] += a[i] / b[i];
    }
    return diag;
}

Matrix3x3 ComputeXYZD50(const ColorSpacePrimaries& primaries)
{
    float oneRxRy = (1 - primaries.rX) / primaries.rY;
    float oneGxGy = (1 - primaries.gX) / primaries.gY;
    float oneBxBy = (1 - primaries.bX) / primaries.bY;
    float oneWxWy = (1 - primaries.wX) / primaries.wY;

    float RxRy = primaries.rX / primaries.rY;
    float GxGy = primaries.gX / primaries.gY;
    float BxBy = primaries.bX / primaries.bY;
    float WxWy = primaries.wX / primaries.wY;

    float BY = ((oneWxWy - oneRxRy) * (GxGy - RxRy) - (WxWy - RxRy) * (oneGxGy - oneRxRy)) /
        ((oneBxBy - oneRxRy) * (GxGy - RxRy) - (BxBy - RxRy) * (oneGxGy - oneRxRy));
    float GY = (WxWy - RxRy - BY * (BxBy - RxRy)) / (GxGy - RxRy);
    float RY = 1 - GY - BY;

    float RYRy = RY / primaries.rY;
    float GYGy = GY / primaries.gY;
    float BYBy = BY / primaries.bY;

    Matrix3x3 toXYZ = {{{RYRy * primaries.rX, GYGy * primaries.gX, BYBy * primaries.bX},
        {RY, GY, BY},
        {RYRy * (1 - primaries.rX - primaries.rY),
         GYGy * (1 - primaries.gX - primaries.gY),
         BYBy * (1 - primaries.bX - primaries.bY)}}};
    std::array<float, DIMES_2> wp = {primaries.wX, primaries.wY};

    return DXToD50(toXYZ, wp);
}

static Matrix3x3 AdaptationToD50(const Vector3& srcWhitePoint)
{
    Vector3 srcLMS = BRADFORD * srcWhitePoint;
    Vector3 dstLMS = BRADFORD * ILLUMINANT_D50_XYZ;
    return BRADFORD_INV * (dstLMS / srcLMS) * BRADFORD;
}

Matrix3x3 DXToD50(const Matrix3x3 &toXYZ, const std::array<float, DIMES_2> &wp)
{
    if ((wp[0] == ILLUMINANT_D50_XY[0]) && (wp[1] == ILLUMINANT_D50_XY[1])) {
        return toXYZ;
    }
    Vector3 srcXYZ = XYZ(Vector3 {wp[0], wp[1], 1.0f});
    return AdaptationToD50(srcXYZ) * toXYZ;
}

static bool IsFinite(float x)
{
    return x * 0 == 0;
}

// inverse src Matrix to dst Matrix
Matrix3x3 Invert(const Matrix3x3& src)
{
    double a00 = src[0][0];
    double a01 = src[1][0];
    double a02 = src[2][0];
    double a10 = src[0][1];
    double a11 = src[1][1];
    double a12 = src[2][1];
    double a20 = src[0][2];
    double a21 = src[1][2];
    double a22 = src[2][2];

    double b0 = a00 * a11 - a01 * a10;
    double b1 = a00 * a12 - a02 * a10;
    double b2 = a01 * a12 - a02 * a11;
    double b3 = a20, b4 = a21, b5 = a22;

    double determinant = b0 * b5 - b1 * b4 + b2 * b3;

    if (determinant == 0) {
        return {};
    }

    double invdet = 1.0 / determinant;
    if (invdet > +FLT_MAX || invdet < -FLT_MAX || !IsFinite((float)invdet)) {
        return {};
    }

    Matrix3x3 dst {};

    b0 *= invdet;
    b1 *= invdet;
    b2 *= invdet;
    b3 *= invdet;
    b4 *= invdet;
    b5 *= invdet;

    dst[0][0] = static_cast<float>(a11 * b5 - a12 * b4); // compute dst[0][0] value
    dst[1][0] = static_cast<float>(a02 * b4 - a01 * b5); // compute dst[1][0] value
    dst[2][0] = static_cast<float>(+b2); // compute dst[2][0] value
    dst[0][1] = static_cast<float>(a12 * b3 - a10 * b5); // compute dst[0][1] value
    dst[1][1] = static_cast<float>(a00 * b5 - a02 * b3); // compute dst[1][1] value
    dst[2][1] = static_cast<float>(-b1); // compute dst[2][1] value
    dst[0][2] = static_cast<float>(a10 * b4 - a11 * b3); // compute dst[0][2] value
    dst[1][2] = static_cast<float>(a01 * b3 - a00 * b4); // compute dst[1][2] value
    dst[2][2] = static_cast<float>(+b0); // compute dst[2][2] value

    for (size_t r = 0; r < dst.size(); ++r) {
        for (size_t c = 0; c < dst[0].size(); ++c) {
            if (!IsFinite(dst[r][c])) {
                return {};
            }
        }
    }
    return dst;
}

Vector3 ColorSpace::ToLinear(Vector3 v) const
{
    auto &p = transferFunc;
    Vector3 res = v;
    for (auto& n : res) {
        n = n >= p.d ? std::pow(p.a * n + p.b, p.g) + p.e : p.c * n + p.f;
    }
    return res;
}

Vector3 ColorSpace::ToNonLinear(Vector3 v) const
{
    auto &p = transferFunc;
    Vector3 res = v;
    for (auto& n : res) {
        n = n >= p.d * p.c ? (std::pow(n - p.e, 1.0f / p.g) - p.b) / p.a : (n - p.f) / p.c;
    }
    return res;
}

sk_sp<SkColorSpace> ColorSpace::ToSkColorSpace() const
{
    skcms_Matrix3x3 toXYZ = ToSkiaXYZ();
    skcms_TransferFunction skTransferFun = {
        transferFunc.g,
        transferFunc.a,
        transferFunc.b,
        transferFunc.c,
        transferFunc.d,
        transferFunc.e,
        transferFunc.f,
    };
    return SkColorSpace::MakeRGB(skTransferFun, toXYZ);
}

skcms_Matrix3x3 ColorSpace::ToSkiaXYZ() const
{
    skcms_Matrix3x3 skToXYZ;
    for (int i = 0; i < DIMES_3; ++i) {
        for (int j = 0; j < DIMES_3; ++j) {
            skToXYZ.vals[i][j] = toXYZ[i][j];
        }
    }
    return skToXYZ;
}
} //! namespace ColorManager
} //! namespace OHOS
