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

#include "../include/util/egl_manager.h"  // for EglManager

#include <cstddef>                       // for NULL

#include "EGL/egl.h"                      // for eglCreatePbufferSurface
#include "EGL/eglplatform.h"              // for EGLint, NativeWindow
#include "__config"                       // for std

#include "../include/util/log.h"          // for LOGE, LOGI

#ifdef __cplusplus
extern "C" {
#endif

namespace OHOS {
namespace Rosen {
namespace {
    const int OPENGL_ES2_VERSION = 2;
    const int OPENGL_ES3_VERSION = 3;
}
using namespace std;

EGLConfig EglManager::GetConfig(int version, EGLDisplay eglDisplay)
{
    EGLint renderableType = EGL_OPENGL_ES2_BIT;
    if (version == OPENGL_ES3_VERSION) {
        renderableType = EGL_OPENGL_ES3_BIT;
    }
    int attribList[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT | EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, renderableType,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_NONE
    };
    EGLConfig configs = NULL;
    int numConfigs;
    if (!eglChooseConfig(eglDisplay, attribList, &configs, 1, &numConfigs)) {
        LOGE("eglChooseConfig ERROR");
        return NULL;
    }
    return configs;
}

void EglManager::MakeCurrentIfNeeded(EGLSurface newEGLSurface)
{
    if (mCurrentSurface != newEGLSurface) {
        mCurrentSurface = newEGLSurface;
        eglMakeCurrent(mEGLDisplay, mCurrentSurface, mCurrentSurface, mEGLContext);
    }
}

EGLSurface EglManager::CreateSurface(NativeWindow* window)
{
    EGLSurface eglSurface = nullptr;
    if (window) {
        LOGI("EglManager CreateSurface from eglWindow");
        eglSurface = eglCreateWindowSurface(mEGLDisplay, mEGLConfig, window, NULL);
    } else {
        LOGI("EglManager CreateSurface from PBuffer width = %{public}d, height = %{public}d",
            mEglWidth, mEglHeight);
        int surfaceAttributes[] = {
            EGL_WIDTH, mEglWidth,
            EGL_HEIGHT, mEglHeight,
            EGL_NONE
        };
        eglSurface = eglCreatePbufferSurface(mEGLDisplay, mEGLConfig, surfaceAttributes);
    }
    return eglSurface;
}

void EglManager::Init()
{
    if (initialized) {
        return;
    }
    initialized = true;
    LOGI("EglManager ----- Init.\n");
    if (mEGLContext != nullptr) {
        LOGE("EglManager Init mEGLContext is already init.\n");
        return;
    }

    mEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mEGLDisplay == EGL_NO_DISPLAY) {
        LOGE("EglManager Init unable to get EGL display.\n");
        return;
    }

    EGLint eglMajVers, eglMinVers;
    if (!eglInitialize(mEGLDisplay, &eglMajVers, &eglMinVers)) {
        mEGLDisplay = EGL_NO_DISPLAY;
        LOGE("EglManager Init unable to initialize display");
        return;
    }

    int version = OPENGL_ES3_VERSION;
    mEGLConfig = EglManager::GetConfig(version, mEGLDisplay);
    if (mEGLConfig == NULL) {
        LOGE("EglManager Init config ERROR, try again");
        version = OPENGL_ES2_VERSION;
        mEGLConfig = EglManager::GetConfig(version, mEGLDisplay);
        if (mEGLConfig == NULL) {
            LOGE("EglManager Init config ERROR again");
            return;
        }
    }

    if (mEglWindow) {
        LOGI("EglManager Init eglSurface from eglWindow");
        mCurrentSurface = eglCreateWindowSurface(mEGLDisplay, mEGLConfig, mEglWindow, NULL);
        if (mCurrentSurface == NULL) {
            LOGE("EglManager Init eglSurface = null");
            return;
        }
    } else {
        LOGI("EglManager Init eglSurface from PBuffer width = %{public}d, height = %{public}d",
            mEglWidth, mEglHeight);
        int surfaceAttributes[] = {
            EGL_WIDTH, mEglWidth,
            EGL_HEIGHT, mEglHeight,
            EGL_NONE
        };
        mCurrentSurface = eglCreatePbufferSurface(mEGLDisplay, mEGLConfig, surfaceAttributes);
        if (mCurrentSurface == NULL) {
            LOGE("EglManager Init eglCreateContext eglSurface = null");
            return;
        }
    }

    int attrib3List[] = {
        EGL_CONTEXT_CLIENT_VERSION, version,
        EGL_NONE
    };
    mEGLContext = eglCreateContext(mEGLDisplay, mEGLConfig, nullptr, attrib3List);
    int error = eglGetError();
    if (error == EGL_SUCCESS) {
        LOGI("EglManager Init Create mEGLContext ok");
    } else {
        LOGE("EglManager Init eglCreateContext error %{public}x", error);
    }
    eglMakeCurrent(mEGLDisplay, mCurrentSurface, mCurrentSurface, mEGLContext);
}
} // namespace Rosen
} // namespace OHOS

#ifdef __cplusplus
}
#endif
