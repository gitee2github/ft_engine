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

#include "c/drawing_bitmap.h"

#include "draw/color.h"
#include "image/bitmap.h"

using namespace OHOS;
using namespace Rosen;
using namespace Drawing;

static Bitmap* CastToBitmap(OH_Drawing_Bitmap* cBitmap)
{
    return reinterpret_cast<Bitmap*>(cBitmap);
}

static ColorType CColorFormatCastToColorType(OH_Drawing_ColorFormat cColorFormat)
{
    ColorType colorType = COLORTYPE_UNKNOWN;
    switch (cColorFormat) {
        case COLOR_FORMAT_UNKNOWN:
            colorType = COLORTYPE_UNKNOWN;
            break;
        case COLOR_FORMAT_ALPHA_8:
            colorType = COLORTYPE_ALPHA_8;
            break;
        case COLOR_FORMAT_RGB_565:
            colorType = COLORTYPE_RGB_565;
            break;
        case COLOR_FORMAT_ARGB_4444:
            colorType = COLORTYPE_ARGB_4444;
            break;
        case COLOR_FORMAT_RGBA_8888:
            colorType = COLORTYPE_RGBA_8888;
            break;
        case COLOR_FORMAT_BGRA_8888:
            colorType = COLORTYPE_BGRA_8888;
            break;
        default:
            break;
    }
    return colorType;
}

static AlphaType CAlphaFormatCastToAlphaType(OH_Drawing_AlphaFormat cAlphaFormat)
{
    AlphaType alphaType = ALPHATYPE_UNKNOWN;

    switch (cAlphaFormat) {
        case ALPHA_FORMAT_UNKNOWN:
            alphaType = ALPHATYPE_UNKNOWN;
            break;
        case ALPHA_FORMAT_OPAQUE:
            alphaType = ALPHATYPE_OPAQUE;
            break;
        case ALPHA_FORMAT_PREMUL:
            alphaType = ALPHATYPE_PREMUL;
            break;
        case ALPHA_FORMAT_UNPREMUL:
            alphaType = ALPHATYPE_UNPREMUL;
            break;
        default:
            break;
    }
    return alphaType;
}

OH_Drawing_Bitmap* OH_Drawing_BitmapCreate()
{
    return (OH_Drawing_Bitmap*)new Bitmap;
}

void OH_Drawing_BitmapDestroy(OH_Drawing_Bitmap* cBitmap)
{
    delete CastToBitmap(cBitmap);
}

void OH_Drawing_BitmapBuild(OH_Drawing_Bitmap* cBitmap, const uint32_t width, const uint32_t height,
    const OH_Drawing_BitmapFormat* cBitmapFormat)
{
    ColorType colorType = CColorFormatCastToColorType(cBitmapFormat->colorFormat);
    AlphaType alphaType = CAlphaFormatCastToAlphaType(cBitmapFormat->alphaFormat);

    BitmapFormat format { colorType, alphaType };
    CastToBitmap(cBitmap)->Build(width, height, format);
}

uint32_t OH_Drawing_BitmapGetWidth(OH_Drawing_Bitmap* cBitmap)
{
    return CastToBitmap(cBitmap)->GetWidth();
}

uint32_t OH_Drawing_BitmapGetHeight(OH_Drawing_Bitmap* cBitmap)
{
    return CastToBitmap(cBitmap)->GetHeight();
}

void* OH_Drawing_BitmapGetPixels(OH_Drawing_Bitmap* cBitmap)
{
    return CastToBitmap(cBitmap)->GetPixels();
}
