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

#ifndef ROSENRENDER_ROSEN_WEBGL_RENDERING_CONTEXT_BASE
#define ROSENRENDER_ROSEN_WEBGL_RENDERING_CONTEXT_BASE

#include <GLES2/gl2.h>
#include <GLES3/gl31.h>
#include <GLES2/gl2ext.h>
#include "securec.h"
#include "common/napi/n_exporter.h"
#include "webgl_rendering_context_basic_base.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace OHOS {
namespace Rosen {
class WebGLRenderingContextBase : public WebGLRenderingContextBasicBase {
public:
    static const GLenum DEPTH_BUFFER_BIT = 0x00000100;
    static const GLenum STENCIL_BUFFER_BIT = 0x00000400;
    static const GLenum COLOR_BUFFER_BIT = 0x00004000;
    static const GLenum POINTS = 0x0000;
    static const GLenum LINES = 0x0001;
    static const GLenum LINE_LOOP = 0x0002;
    static const GLenum LINE_STRIP = 0x0003;
    static const GLenum TRIANGLES = 0x0004;
    static const GLenum TRIANGLE_STRIP = 0x0005;
    static const GLenum TRIANGLE_FAN = 0x0006;
    static const GLenum ZERO = 0;
    static const GLenum ONE = 1;
    static const GLenum SRC_COLOR = 0x0300;
    static const GLenum ONE_MINUS_SRC_COLOR = 0x0301;
    static const GLenum SRC_ALPHA = 0x0302;
    static const GLenum ONE_MINUS_SRC_ALPHA = 0x0303;
    static const GLenum DST_ALPHA = 0x0304;
    static const GLenum ONE_MINUS_DST_ALPHA = 0x0305;
    static const GLenum DST_COLOR = 0x0306;
    static const GLenum ONE_MINUS_DST_COLOR = 0x0307;
    static const GLenum SRC_ALPHA_SATURATE = 0x0308;
    static const GLenum FUNC_ADD = 0x8006;
    static const GLenum BLEND_EQUATION = 0x8009;
    static const GLenum BLEND_EQUATION_RGB = 0x8009;
    static const GLenum BLEND_EQUATION_ALPHA = 0x883D;
    static const GLenum FUNC_SUBTRACT = 0x800A;
    static const GLenum FUNC_REVERSE_SUBTRACT = 0x800B;
    static const GLenum BLEND_DST_RGB = 0x80C8;
    static const GLenum BLEND_SRC_RGB = 0x80C9;
    static const GLenum BLEND_DST_ALPHA = 0x80CA;
    static const GLenum BLEND_SRC_ALPHA = 0x80CB;
    static const GLenum CONSTANT_COLOR = 0x8001;
    static const GLenum ONE_MINUS_CONSTANT_COLOR = 0x8002;
    static const GLenum CONSTANT_ALPHA = 0x8003;
    static const GLenum ONE_MINUS_CONSTANT_ALPHA = 0x8004;
    static const GLenum BLEND_COLOR = 0x8005;
    static const GLenum ARRAY_BUFFER = 0x8892;
    static const GLenum ELEMENT_ARRAY_BUFFER = 0x8893;
    static const GLenum ARRAY_BUFFER_BINDING = 0x8894;
    static const GLenum ELEMENT_ARRAY_BUFFER_BINDING = 0x8895;
    static const GLenum STREAM_DRAW = 0x88E0;
    static const GLenum STATIC_DRAW = 0x88E4;
    static const GLenum DYNAMIC_DRAW = 0x88E8;
    static const GLenum BUFFER_SIZE = 0x8764;
    static const GLenum BUFFER_USAGE = 0x8765;
    static const GLenum CURRENT_VERTEX_ATTRIB = 0x8626;
    static const GLenum FRONT = 0x0404;
    static const GLenum BACK = 0x0405;
    static const GLenum FRONT_AND_BACK = 0x0408;
    static const GLenum CULL_FACE = 0x0B44;
    static const GLenum BLEND = 0x0BE2;
    static const GLenum DITHER = 0x0BD0;
    static const GLenum STENCIL_TEST = 0x0B90;
    static const GLenum DEPTH_TEST = 0x0B71;
    static const GLenum SCISSOR_TEST = 0x0C11;
    static const GLenum POLYGON_OFFSET_FILL = 0x8037;
    static const GLenum SAMPLE_ALPHA_TO_COVERAGE = 0x809E;
    static const GLenum SAMPLE_COVERAGE = 0x80A0;
    static const GLenum NO_ERROR = 0;
    static const GLenum INVALID_ENUM = 0x0500;
    static const GLenum INVALID_VALUE = 0x0501;
    static const GLenum INVALID_OPERATION = 0x0502;
    static const GLenum OUT_OF_MEMORY = 0x0505;
    static const GLenum CW = 0x0900;
    static const GLenum CCW = 0x0901;
    static const GLenum LINE_WIDTH = 0x0B21;
    static const GLenum ALIASED_POINT_SIZE_RANGE = 0x846D;
    static const GLenum ALIASED_LINE_WIDTH_RANGE = 0x846E;
    static const GLenum CULL_FACE_MODE = 0x0B45;
    static const GLenum FRONT_FACE = 0x0B46;
    static const GLenum DEPTH_RANGE = 0x0B70;
    static const GLenum DEPTH_WRITEMASK = 0x0B72;
    static const GLenum DEPTH_CLEAR_VALUE = 0x0B73;
    static const GLenum DEPTH_FUNC = 0x0B74;
    static const GLenum STENCIL_CLEAR_VALUE = 0x0B91;
    static const GLenum STENCIL_FUNC = 0x0B92;
    static const GLenum STENCIL_FAIL = 0x0B94;
    static const GLenum STENCIL_PASS_DEPTH_FAIL = 0x0B95;
    static const GLenum STENCIL_PASS_DEPTH_PASS = 0x0B96;
    static const GLenum STENCIL_REF = 0x0B97;
    static const GLenum STENCIL_VALUE_MASK = 0x0B93;
    static const GLenum STENCIL_WRITEMASK = 0x0B98;
    static const GLenum STENCIL_BACK_FUNC = 0x8800;
    static const GLenum STENCIL_BACK_FAIL = 0x8801;
    static const GLenum STENCIL_BACK_PASS_DEPTH_FAIL = 0x8802;
    static const GLenum STENCIL_BACK_PASS_DEPTH_PASS = 0x8803;
    static const GLenum STENCIL_BACK_REF = 0x8CA3;
    static const GLenum STENCIL_BACK_VALUE_MASK = 0x8CA4;
    static const GLenum STENCIL_BACK_WRITEMASK = 0x8CA5;
    static const GLenum VIEWPORT = 0x0BA2;
    static const GLenum SCISSOR_BOX = 0x0C10;
    static const GLenum COLOR_CLEAR_VALUE = 0x0C22;
    static const GLenum COLOR_WRITEMASK = 0x0C23;
    static const GLenum UNPACK_ALIGNMENT = 0x0CF5;
    static const GLenum PACK_ALIGNMENT = 0x0D05;
    static const GLenum MAX_TEXTURE_SIZE = 0x0D33;
    static const GLenum MAX_VIEWPORT_DIMS = 0x0D3A;
    static const GLenum SUBPIXEL_BITS = 0x0D50;
    static const GLenum RED_BITS = 0x0D52;
    static const GLenum GREEN_BITS = 0x0D53;
    static const GLenum BLUE_BITS = 0x0D54;
    static const GLenum ALPHA_BITS = 0x0D55;
    static const GLenum DEPTH_BITS = 0x0D56;
    static const GLenum STENCIL_BITS = 0x0D57;
    static const GLenum POLYGON_OFFSET_UNITS = 0x2A00;
    static const GLenum POLYGON_OFFSET_FACTOR = 0x8038;
    static const GLenum TEXTURE_BINDING_2D = 0x8069;
    static const GLenum SAMPLE_BUFFERS = 0x80A8;
    static const GLenum SAMPLES = 0x80A9;
    static const GLenum SAMPLE_COVERAGE_VALUE = 0x80AA;
    static const GLenum SAMPLE_COVERAGE_INVERT = 0x80AB;
    static const GLenum COMPRESSED_TEXTURE_FORMATS = 0x86A3;
    static const GLenum DONT_CARE = 0x1100;
    static const GLenum FASTEST = 0x1101;
    static const GLenum NICEST = 0x1102;
    static const GLenum GENERATE_MIPMAP_HINT = 0x8192;
    static const GLenum BYTE = 0x1400;
    static const GLenum UNSIGNED_BYTE = 0x1401;
    static const GLenum SHORT = 0x1402;
    static const GLenum UNSIGNED_SHORT = 0x1403;
    static const GLenum INT = 0x1404;
    static const GLenum UNSIGNED_INT = 0x1405;
    static const GLenum FLOAT = 0x1406;
    static const GLenum DEPTH_COMPONENT = 0x1902;
    static const GLenum ALPHA = 0x1906;
    static const GLenum RGB = 0x1907;
    static const GLenum RGBA = 0x1908;
    static const GLenum LUMINANCE = 0x1909;
    static const GLenum LUMINANCE_ALPHA = 0x190A;
    static const GLenum UNSIGNED_SHORT_4_4_4_4 = 0x8033;
    static const GLenum UNSIGNED_SHORT_5_5_5_1 = 0x8034;
    static const GLenum UNSIGNED_SHORT_5_6_5 = 0x8363;
    static const GLenum FRAGMENT_SHADER = 0x8B30;
    static const GLenum VERTEX_SHADER = 0x8B31;
    static const GLenum MAX_VERTEX_ATTRIBS = 0x8869;
    static const GLenum MAX_VERTEX_UNIFORM_VECTORS = 0x8DFB;
    static const GLenum MAX_VARYING_VECTORS = 0x8DFC;
    static const GLenum MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D;
    static const GLenum MAX_VERTEX_TEXTURE_IMAGE_UNITS = 0x8B4C;
    static const GLenum MAX_TEXTURE_IMAGE_UNITS = 0x8872;
    static const GLenum MAX_FRAGMENT_UNIFORM_VECTORS = 0x8DFD;
    static const GLenum SHADER_TYPE = 0x8B4F;
    static const GLenum DELETE_STATUS = 0x8B80;
    static const GLenum LINK_STATUS = 0x8B82;
    static const GLenum VALIDATE_STATUS = 0x8B83;
    static const GLenum ATTACHED_SHADERS = 0x8B85;
    static const GLenum ACTIVE_UNIFORMS = 0x8B86;
    static const GLenum ACTIVE_ATTRIBUTES = 0x8B89;
    static const GLenum SHADING_LANGUAGE_VERSION = 0x8B8C;
    static const GLenum CURRENT_PROGRAM = 0x8B8D;
    static const GLenum NEVER = 0x0200;
    static const GLenum LESS = 0x0201;
    static const GLenum EQUAL = 0x0202;
    static const GLenum LEQUAL = 0x0203;
    static const GLenum GREATER = 0x0204;
    static const GLenum NOTEQUAL = 0x0205;
    static const GLenum GEQUAL = 0x0206;
    static const GLenum ALWAYS = 0x0207;
    static const GLenum KEEP = 0x1E00;
    static const GLenum REPLACE = 0x1E01;
    static const GLenum INCR = 0x1E02;
    static const GLenum DECR = 0x1E03;
    static const GLenum INVERT = 0x150A;
    static const GLenum INCR_WRAP = 0x8507;
    static const GLenum DECR_WRAP = 0x8508;
    static const GLenum VENDOR = 0x1F00;
    static const GLenum RENDERER = 0x1F01;
    static const GLenum VERSION = 0x1F02;
    static const GLenum NEAREST = 0x2600;
    static const GLenum LINEAR = 0x2601;
    static const GLenum NEAREST_MIPMAP_NEAREST = 0x2700;
    static const GLenum LINEAR_MIPMAP_NEAREST = 0x2701;
    static const GLenum NEAREST_MIPMAP_LINEAR = 0x2702;
    static const GLenum LINEAR_MIPMAP_LINEAR = 0x2703;
    static const GLenum TEXTURE_MAG_FILTER = 0x2800;
    static const GLenum TEXTURE_MIN_FILTER = 0x2801;
    static const GLenum TEXTURE_WRAP_S = 0x2802;
    static const GLenum TEXTURE_WRAP_T = 0x2803;
    static const GLenum TEXTURE_2D = 0x0DE1;
    static const GLenum TEXTURE = 0x1702;
    static const GLenum TEXTURE_CUBE_MAP = 0x8513;
    static const GLenum TEXTURE_BINDING_CUBE_MAP = 0x8514;
    static const GLenum TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515;
    static const GLenum TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516;
    static const GLenum TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517;
    static const GLenum TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518;
    static const GLenum TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519;
    static const GLenum TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A;
    static const GLenum MAX_CUBE_MAP_TEXTURE_SIZE = 0x851C;
    static const GLenum TEXTURE0 = 0x84C0;
    static const GLenum TEXTURE1 = 0x84C1;
    static const GLenum TEXTURE2 = 0x84C2;
    static const GLenum TEXTURE3 = 0x84C3;
    static const GLenum TEXTURE4 = 0x84C4;
    static const GLenum TEXTURE5 = 0x84C5;
    static const GLenum TEXTURE6 = 0x84C6;
    static const GLenum TEXTURE7 = 0x84C7;
    static const GLenum TEXTURE8 = 0x84C8;
    static const GLenum TEXTURE9 = 0x84C9;
    static const GLenum TEXTURE10 = 0x84CA;
    static const GLenum TEXTURE11 = 0x84CB;
    static const GLenum TEXTURE12 = 0x84CC;
    static const GLenum TEXTURE13 = 0x84CD;
    static const GLenum TEXTURE14 = 0x84CE;
    static const GLenum TEXTURE15 = 0x84CF;
    static const GLenum TEXTURE16 = 0x84D0;
    static const GLenum TEXTURE17 = 0x84D1;
    static const GLenum TEXTURE18 = 0x84D2;
    static const GLenum TEXTURE19 = 0x84D3;
    static const GLenum TEXTURE20 = 0x84D4;
    static const GLenum TEXTURE21 = 0x84D5;
    static const GLenum TEXTURE22 = 0x84D6;
    static const GLenum TEXTURE23 = 0x84D7;
    static const GLenum TEXTURE24 = 0x84D8;
    static const GLenum TEXTURE25 = 0x84D9;
    static const GLenum TEXTURE26 = 0x84DA;
    static const GLenum TEXTURE27 = 0x84DB;
    static const GLenum TEXTURE28 = 0x84DC;
    static const GLenum TEXTURE29 = 0x84DD;
    static const GLenum TEXTURE30 = 0x84DE;
    static const GLenum TEXTURE31 = 0x84DF;
    static const GLenum ACTIVE_TEXTURE = 0x84E0;
    static const GLenum REPEAT = 0x2901;
    static const GLenum CLAMP_TO_EDGE = 0x812F;
    static const GLenum MIRRORED_REPEAT = 0x8370;
    static const GLenum FLOAT_VEC2 = 0x8B50;
    static const GLenum FLOAT_VEC3 = 0x8B51;
    static const GLenum FLOAT_VEC4 = 0x8B52;
    static const GLenum INT_VEC2 = 0x8B53;
    static const GLenum INT_VEC3 = 0x8B54;
    static const GLenum INT_VEC4 = 0x8B55;
    static const GLenum BOOL = 0x8B56;
    static const GLenum BOOL_VEC2 = 0x8B57;
    static const GLenum BOOL_VEC3 = 0x8B58;
    static const GLenum BOOL_VEC4 = 0x8B59;
    static const GLenum FLOAT_MAT2 = 0x8B5A;
    static const GLenum FLOAT_MAT3 = 0x8B5B;
    static const GLenum FLOAT_MAT4 = 0x8B5C;
    static const GLenum SAMPLER_2D = 0x8B5E;
    static const GLenum SAMPLER_CUBE = 0x8B60;
    static const GLenum VERTEX_ATTRIB_ARRAY_ENABLED = 0x8622;
    static const GLenum VERTEX_ATTRIB_ARRAY_SIZE = 0x8623;
    static const GLenum VERTEX_ATTRIB_ARRAY_STRIDE = 0x8624;
    static const GLenum VERTEX_ATTRIB_ARRAY_TYPE = 0x8625;
    static const GLenum VERTEX_ATTRIB_ARRAY_NORMALIZED = 0x886A;
    static const GLenum VERTEX_ATTRIB_ARRAY_POINTER = 0x8645;
    static const GLenum VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F;
    static const GLenum IMPLEMENTATION_COLOR_READ_TYPE = 0x8B9A;
    static const GLenum IMPLEMENTATION_COLOR_READ_FORMAT = 0x8B9B;
    static const GLenum COMPILE_STATUS = 0x8B81;
    static const GLenum LOW_FLOAT = 0x8DF0;
    static const GLenum MEDIUM_FLOAT = 0x8DF1;
    static const GLenum HIGH_FLOAT = 0x8DF2;
    static const GLenum LOW_INT = 0x8DF3;
    static const GLenum MEDIUM_INT = 0x8DF4;
    static const GLenum HIGH_INT = 0x8DF5;
    static const GLenum FRAMEBUFFER = 0x8D40;
    static const GLenum RENDERBUFFER = 0x8D41;
    static const GLenum RGBA4 = 0x8056;
    static const GLenum RGB5_A1 = 0x8057;
    static const GLenum RGB565 = 0x8D62;
    static const GLenum DEPTH_COMPONENT16 = 0x81A5;
    static const GLenum STENCIL_INDEX8 = 0x8D48;
    static const GLenum DEPTH_STENCIL = 0x84F9;
    static const GLenum RENDERBUFFER_WIDTH = 0x8D42;
    static const GLenum RENDERBUFFER_HEIGHT = 0x8D43;
    static const GLenum RENDERBUFFER_INTERNAL_FORMAT = 0x8D44;
    static const GLenum RENDERBUFFER_RED_SIZE = 0x8D50;
    static const GLenum RENDERBUFFER_GREEN_SIZE = 0x8D51;
    static const GLenum RENDERBUFFER_BLUE_SIZE = 0x8D52;
    static const GLenum RENDERBUFFER_ALPHA_SIZE = 0x8D53;
    static const GLenum RENDERBUFFER_DEPTH_SIZE = 0x8D54;
    static const GLenum RENDERBUFFER_STENCIL_SIZE = 0x8D55;
    static const GLenum FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = 0x8CD0;
    static const GLenum FRAMEBUFFER_ATTACHMENT_OBJECT_NAME = 0x8CD1;
    static const GLenum FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = 0x8CD2;
    static const GLenum FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3;
    static const GLenum COLOR_ATTACHMENT0 = 0x8CE0;
    static const GLenum DEPTH_ATTACHMENT = 0x8D00;
    static const GLenum STENCIL_ATTACHMENT = 0x8D20;
    static const GLenum DEPTH_STENCIL_ATTACHMENT = 0x821A;
    static const GLenum NONE = 0;
    static const GLenum FRAMEBUFFER_COMPLETE = 0x8CD5;
    static const GLenum FRAMEBUFFER_INCOMPLETE_ATTACHMENT = 0x8CD6;
    static const GLenum FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7;
    static const GLenum FRAMEBUFFER_INCOMPLETE_DIMENSIONS = 0x8CD9;
    static const GLenum FRAMEBUFFER_UNSUPPORTED = 0x8CDD;
    static const GLenum FRAMEBUFFER_BINDING = 0x8CA6;
    static const GLenum RENDERBUFFER_BINDING = 0x8CA7;
    static const GLenum MAX_RENDERBUFFER_SIZE = 0x84E8;
    static const GLenum INVALID_FRAMEBUFFER_OPERATION = 0x0506;
    static const GLenum UNPACK_FLIP_Y_WEBGL = 0x9240;
    static const GLenum UNPACK_PREMULTIPLY_ALPHA_WEBGL = 0x9241;
    static const GLenum CONTEXT_LOST_WEBGL = 0x9242;
    static const GLenum UNPACK_COLORSPACE_CONVERSION_WEBGL = 0x9243;
    static const GLenum BROWSER_DEFAULT_WEBGL = 0x9244;
    static const uint32_t INPUTFLOAT_LENGTH = 128;
    // readonly canvas: HTMLCanvasElement | OffscreenCanvas;
    int drawingBufferWidth() const;
    int drawingBufferHeight() const;

    static napi_value GetContextAttributes(napi_env env, napi_callback_info info);

    static napi_value IsContextLost(napi_env env, napi_callback_info info);

    static napi_value GetSupportedExtensions(napi_env env, napi_callback_info info);

    static napi_value GetExtension(napi_env env, napi_callback_info info);

    static napi_value ActiveTexture(napi_env env, napi_callback_info info);

    static napi_value AttachShader(napi_env env, napi_callback_info info);

    static napi_value BindAttribLocation(napi_env env, napi_callback_info info);

    static napi_value BindBuffer(napi_env env, napi_callback_info info);

    static napi_value BindFramebuffer(napi_env env, napi_callback_info info);

    static napi_value BindRenderbuffer(napi_env env, napi_callback_info info);

    static napi_value BindTexture(napi_env env, napi_callback_info info);

    static napi_value BlendColor(napi_env env, napi_callback_info info);

    static napi_value BlendEquation(napi_env env, napi_callback_info info);

    static napi_value BlendEquationSeparate(napi_env env, napi_callback_info info);

    static napi_value BlendFunc(napi_env env, napi_callback_info info);

    static napi_value BlendFuncSeparate(napi_env env, napi_callback_info info);

    static napi_value CheckFramebufferStatus(napi_env env, napi_callback_info info);

    static napi_value Clear(napi_env env, napi_callback_info info);

    static napi_value ClearColor(napi_env env, napi_callback_info info);

    static napi_value ClearDepth(napi_env env, napi_callback_info info);

    static napi_value ClearStencil(napi_env env, napi_callback_info info);

    static napi_value ColorMask(napi_env env, napi_callback_info info);

    static napi_value CompileShader(napi_env env, napi_callback_info info);

    static napi_value CopyTexImage2D(napi_env env, napi_callback_info info);

    static napi_value CopyTexSubImage2D(napi_env env, napi_callback_info info);

    static napi_value CreateBuffer(napi_env env, napi_callback_info info);

    static napi_value CreateFramebuffer(napi_env env, napi_callback_info info);

    static napi_value CreateProgram(napi_env env, napi_callback_info info);

    static napi_value CreateRenderbuffer(napi_env env, napi_callback_info info);

    static napi_value CreateShader(napi_env env, napi_callback_info info);

    static napi_value CreateTexture(napi_env env, napi_callback_info info);

    static napi_value CullFace(napi_env env, napi_callback_info info);

    static napi_value DeleteBuffer(napi_env env, napi_callback_info info);

    static napi_value DeleteFramebuffer(napi_env env, napi_callback_info info);

    static napi_value DeleteProgram(napi_env env, napi_callback_info info);

    static napi_value DeleteRenderbuffer(napi_env env, napi_callback_info info);

    static napi_value DeleteShader(napi_env env, napi_callback_info info);

    static napi_value DeleteTexture(napi_env env, napi_callback_info info);

    static napi_value DepthFunc(napi_env env, napi_callback_info info);

    static napi_value DepthMask(napi_env env, napi_callback_info info);

    static napi_value DepthRange(napi_env env, napi_callback_info info);

    static napi_value DetachShader(napi_env env, napi_callback_info info);

    static napi_value Disable(napi_env env, napi_callback_info info);

    static napi_value DisableVertexAttribArray(napi_env env, napi_callback_info info);

    static napi_value DrawArrays(napi_env env, napi_callback_info info);

    static napi_value DrawElements(napi_env env, napi_callback_info info);

    static napi_value Enable(napi_env env, napi_callback_info info);

    static napi_value EnableVertexAttribArray(napi_env env, napi_callback_info info);

    static napi_value Finish(napi_env env, napi_callback_info info);

    static napi_value Flush(napi_env env, napi_callback_info info);

    static napi_value FramebufferRenderbuffer(napi_env env, napi_callback_info info);

    static napi_value FramebufferTexture2D(napi_env env, napi_callback_info info);

    static napi_value Viewport(napi_env env, napi_callback_info info);

    static napi_value GetUniformLocation(napi_env env, napi_callback_info info);

    static napi_value GetVertexAttribOffset(napi_env env, napi_callback_info info);

    static napi_value Hint(napi_env env, napi_callback_info info);

    static napi_value IsBuffer(napi_env env, napi_callback_info info);

    static napi_value IsEnabled(napi_env env, napi_callback_info info);

    static napi_value RenderbufferStorage(napi_env env, napi_callback_info info);

    static napi_value SampleCoverage(napi_env env, napi_callback_info info);

    static napi_value Scissor(napi_env env, napi_callback_info info);

    static napi_value ShaderSource(napi_env env, napi_callback_info info);

    static napi_value StencilFunc(napi_env env, napi_callback_info info);

    static napi_value StencilFuncSeparate(napi_env env, napi_callback_info info);

    static napi_value StencilMask(napi_env env, napi_callback_info info);

    static napi_value StencilMaskSeparate(napi_env env, napi_callback_info info);

    static napi_value StencilOp(napi_env env, napi_callback_info info);

    static napi_value StencilOpSeparate(napi_env env, napi_callback_info info);

    static napi_value TexParameterf(napi_env env, napi_callback_info info);

    static napi_value TexParameteri(napi_env env, napi_callback_info info);

    static napi_value Uniform1f(napi_env env, napi_callback_info info);

    static napi_value Uniform2f(napi_env env, napi_callback_info info);

    static napi_value Uniform3f(napi_env env, napi_callback_info info);

    static napi_value Uniform4f(napi_env env, napi_callback_info info);

    static napi_value UseProgram(napi_env env, napi_callback_info info);

    static napi_value Uniform1i(napi_env env, napi_callback_info info);

    static napi_value Uniform2i(napi_env env, napi_callback_info info);

    static napi_value Uniform3i(napi_env env, napi_callback_info info);

    static napi_value Uniform4i(napi_env env, napi_callback_info info);

    static napi_value ValidateProgram(napi_env env, napi_callback_info info);

    static napi_value VertexAttrib1f(napi_env env, napi_callback_info info);

    static napi_value VertexAttrib2f(napi_env env, napi_callback_info info);

    static napi_value VertexAttrib3f(napi_env env, napi_callback_info info);

    static napi_value VertexAttrib4f(napi_env env, napi_callback_info info);

    static napi_value VertexAttribPointer(napi_env env, napi_callback_info info);

    static napi_value IsFramebuffer(napi_env env, napi_callback_info info);

    static napi_value IsProgram(napi_env env, napi_callback_info info);

    static napi_value IsRenderbuffer(napi_env env, napi_callback_info info);

    static napi_value IsShader(napi_env env, napi_callback_info info);

    static napi_value IsTexture(napi_env env, napi_callback_info info);

    static napi_value LineWidth(napi_env env, napi_callback_info info);

    static napi_value LinkProgram(napi_env env, napi_callback_info info);

    static napi_value PixelStorei(napi_env env, napi_callback_info info);

    static napi_value PolygonOffset(napi_env env, napi_callback_info info);

    static napi_value FrontFace(napi_env env, napi_callback_info info);

    static napi_value GenerateMipmap(napi_env env, napi_callback_info info);

    static napi_value GetActiveAttrib(napi_env env, napi_callback_info info);

    static napi_value GetActiveUniform(napi_env env, napi_callback_info info);

    static napi_value GetAttribLocation(napi_env env, napi_callback_info info);

    static napi_value GetBufferParameter(napi_env env, napi_callback_info info);

    static napi_value GetParameter(napi_env env, napi_callback_info info);

    static napi_value GetError(napi_env env, napi_callback_info info);

    static napi_value GetFramebufferAttachmentParameter(napi_env env, napi_callback_info info);

    static napi_value GetProgramParameter(napi_env env, napi_callback_info info);

    static napi_value GetProgramInfoLog(napi_env env, napi_callback_info info);

    static napi_value GetRenderbufferParameter(napi_env env, napi_callback_info info);

    static napi_value GetShaderParameter(napi_env env, napi_callback_info info);

    static napi_value GetShaderPrecisionFormat(napi_env env, napi_callback_info info);

    static napi_value GetShaderInfoLog(napi_env env, napi_callback_info info);

    static napi_value GetShaderSource(napi_env env, napi_callback_info info);

    static napi_value GetTexParameter(napi_env env, napi_callback_info info);

    static napi_value GetUniform(napi_env env, napi_callback_info info);

    static napi_value GetAttachedShaders(napi_env env, napi_callback_info info);

    static napi_value VertexAttrib1fv(napi_env env, napi_callback_info info);

    static napi_value VertexAttrib2fv(napi_env env, napi_callback_info info);

    static napi_value VertexAttrib3fv(napi_env env, napi_callback_info info);

    static napi_value VertexAttrib4fv(napi_env env, napi_callback_info info);

    static napi_value GetVertexAttrib(napi_env env, napi_callback_info info);
};
} // namespace Rosen
} // namespace OHOS

#ifdef __cplusplus
}
#endif

#endif // ROSENRENDER_ROSEN_WEBGL_RENDERING_CONTEXT_BASE