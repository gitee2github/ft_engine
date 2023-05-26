/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef C_INCLUDE_DRAWING_PEN_H
#define C_INCLUDE_DRAWING_PEN_H

/**
 * @addtogroup Drawing
 * @{
 *
 * @brief Provides functions such as 2D graphics rendering, text drawing, and image display.
 * 
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 *
 * @since 8
 * @version 1.0
 */

/**
 * @file drawing_pen.h
 *
 * @brief Declares functions related to the <b>pen</b> object in the drawing module.
 *
 * @since 8
 * @version 1.0
 */

#include "drawing_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates an <b>OH_Drawing_Pen</b> object.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @return Returns the pointer to the <b>OH_Drawing_Pen</b> object created.
 * @since 8
 * @version 1.0
 */
OH_Drawing_Pen* OH_Drawing_PenCreate(void);

/**
 * @brief Destroys an <b>OH_Drawing_Pen</b> object and reclaims the memory occupied by the object.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @since 8
 * @version 1.0
 */
void OH_Drawing_PenDestroy(OH_Drawing_Pen*);

/**
 * @brief Checks whether anti-aliasing is enabled for a pen. If anti-aliasing is enabled,
 * edges will be drawn with partial transparency.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @return Returns <b>true</b> if anti-aliasing is enabled; returns <b>false</b> otherwise.
 * @since 8
 * @version 1.0
 */
bool OH_Drawing_PenIsAntiAlias(const OH_Drawing_Pen*);

/**
 * @brief Enables or disables anti-aliasing for a pen. If anti-aliasing is enabled,
 * edges will be drawn with partial transparency.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @param bool Specifies whether to enable anti-aliasing. The value <b>true</b> means
 *             to enable anti-aliasing, and <b>false</b> means the opposite.
 * @since 8
 * @version 1.0
 */
void OH_Drawing_PenSetAntiAlias(OH_Drawing_Pen*, bool);

/**
 * @brief Obtains the color of a pen. The color is used by the pen to outline a shape.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @return Returns a 32-bit (ARGB) variable that describes the color.
 * @since 8
 * @version 1.0
 */
uint32_t OH_Drawing_PenGetColor(const OH_Drawing_Pen*);

/**
 * @brief Sets the color for a pen. The color is used by the pen to outline a shape.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @param color Indicates the color to set, which is a 32-bit (ARGB) variable.
 * @since 8
 * @version 1.0
 */
void OH_Drawing_PenSetColor(OH_Drawing_Pen*, uint32_t color);

/**
 * @brief Obtains the thickness of a pen. This thickness determines the width of the outline of a shape.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @return Returns the thickness.
 * @since 8
 * @version 1.0
 */
float OH_Drawing_PenGetWidth(const OH_Drawing_Pen*);

/**
 * @brief Sets the thickness for a pen. This thickness determines the width of the outline of a shape.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @param width Indicates the thickness to set, which is a variable.
 * @since 8
 * @version 1.0
 */
void OH_Drawing_PenSetWidth(OH_Drawing_Pen*, float width);

/**
 * @brief Obtains the stroke miter limit of a polyline drawn by a pen.
 *
 * When the corner type is bevel, a beveled corner is displayed if the miter limit is exceeded,
 * and a mitered corner is displayed if the miter limit is not exceeded.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @return Returns the miter limit.
 * @since 8
 * @version 1.0
 */
float OH_Drawing_PenGetMiterLimit(const OH_Drawing_Pen*);

/**
 * @brief Sets the stroke miter limit for a polyline drawn by a pen.
 *
 * When the corner type is bevel, a beveled corner is displayed if the miter limit is exceeded,
 * and a mitered corner is displayed if the miter limit is not exceeded.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @param miter Indicates a variable that describes the miter limit.
 * @since 8
 * @version 1.0
 */
void OH_Drawing_PenSetMiterLimit(OH_Drawing_Pen*, float miter);

/**
 * @brief Enumerates line cap styles of a pen. The line cap style defines 
 * the style of both ends of a line segment drawn by the pen.
 * 
 * @since 8
 * @version 1.0
 */
typedef enum {
    /**
     * There is no cap style. Both ends of the line segment are cut off square.
     */
    LINE_FLAT_CAP,
    /**
     * Square cap style. Both ends have a square, the height of which
     * is half of the width of the line segment, with the same width.
     */
    LINE_SQUARE_CAP,
    /**
     * Round cap style. Both ends have a semicircle centered, the diameter of which
     * is the same as the width of the line segment.
     */
    LINE_ROUND_CAP
} OH_Drawing_PenLineCapStyle;

/**
 * @brief Obtains the line cap style of a pen.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @return Returns the line cap style.
 * @since 8
 * @version 1.0
 */
OH_Drawing_PenLineCapStyle OH_Drawing_PenGetCap(const OH_Drawing_Pen*);

/**
 * @brief Sets the line cap style for a pen.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @param OH_Drawing_PenLineCapStyle Indicates a variable that describes the line cap style.
 * @since 8
 * @version 1.0
 */
void OH_Drawing_PenSetCap(OH_Drawing_Pen*, OH_Drawing_PenLineCapStyle);

/**
 * @brief Enumerates pen line join styles. The line join style defines
 * the shape of the joints of a polyline segment drawn by the pen.
 * 
 * @since 8
 * @version 1.0
 */
typedef enum {
    /**
     * Mitered corner. If the angle of a polyline is small, its miter length may be inappropriate.
     * In this case, you need to use the miter limit to limit the miter length.
     */
    LINE_MITER_JOIN,
    /** Round corner. */
    LINE_ROUND_JOIN,
    /** Beveled corner. */
    LINE_BEVEL_JOIN
} OH_Drawing_PenLineJoinStyle;

/**
 * @brief Obtains the line join style of a pen.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @return Returns the line join style.
 * @since 8
 * @version 1.0
 */
OH_Drawing_PenLineJoinStyle OH_Drawing_PenGetJoin(const OH_Drawing_Pen*);

/**
 * @brief Sets the line join style for a pen.
 *
 * @syscap SystemCapability.Graphic.Graphic2D.NativeDrawing
 * @param OH_Drawing_Pen Indicates the pointer to an <b>OH_Drawing_Pen</b> object.
 * @param OH_Drawing_PenLineJoinStyle Indicates a variable that describes the line join style.
 * @since 8
 * @version 1.0
 */
void OH_Drawing_PenSetJoin(OH_Drawing_Pen*, OH_Drawing_PenLineJoinStyle);

#ifdef __cplusplus
}
#endif
/** @} */
#endif
