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

#ifndef INTERFACES_INNERKITS_WM_WINDOW_MANAGER_H
#define INTERFACES_INNERKITS_WM_WINDOW_MANAGER_H

#include "window_manager_common.h"

#include <cstdio>
#include <memory>

#include <refbase.h>
#include <surface.h>
#include <../wmclient/iscreen_shot_callback.h>
#include <../wmclient/iwindow_shot_callback.h>

namespace OHOS {
/**
 * @brief The IWindowManager class is an abstract definition of windows manager.
 *        Provides a series of client/interfaces for window management, event processing, etc.
 */

class WindowBase {
public:
    WindowBase(int32_t windowid, sptr<Surface>& surface);
    virtual ~WindowBase();

    void GetRequestConfig(BufferRequestConfig &config);
    void SetRequestConfig(BufferRequestConfig &config);
    void RegisterWindowInfoChangeCb(funcWindowInfoChange cb);

    int32_t GetWindowID();
    virtual sptr<Surface> GetSurface();

protected:
    int32_t m_windowid;
    sptr<Surface> surface_;
    BufferRequestConfig config_;
};

class Window : public WindowBase {
public:
    Window(int32_t windowid, sptr<Surface>& surface);
    ~Window();

    void Show();
    void Hide();
    void Move(int32_t x, int32_t y);
    void SwitchTop();
    void ChangeWindowType(WindowType type);
    void ReSize(int32_t width, int32_t height);
    void Rotate(rotateType type);
    void RegisterOnWindowCreateCb(void(* cb)(uint32_t pid));
};

class SubWindow : public WindowBase {
public:
    SubWindow(int32_t windowid, sptr<Surface>& surface);
    virtual ~SubWindow();

    virtual void Move(int32_t x, int32_t y);
    virtual void SetSubWindowSize(int32_t width, int32_t height);
};

class ScreenShotCallback : public RefBase {
public:
    virtual void OnScreenShot(const struct WMImageInfo &info){}
};

class WindowShotCallback : public RefBase {
public:
    virtual void OnWindowShot(const struct WMImageInfo &info){}
};

class WindowManager : public RefBase {
public:
    static sptr<WindowManager> GetInstance();

    std::unique_ptr<Window> CreateWindow(WindowConfig *config);
    std::unique_ptr<SubWindow> CreateSubWindow(int32_t parentid, WindowConfig *config);
    void StartShotScreen(IScreenShotCallback *cb);
    void StartShotWindow(int32_t id, IWindowShotCallback *cb);
    int32_t GetMaxWidth();
    int32_t GetMaxHeight();
    void SwitchTop(int windowId);
    void DestroyWindow(int windowId);
private:
    static sptr<WindowManager> instance;
    WindowManager();
    virtual ~WindowManager();

    void init();
};
}

#endif // INTERFACES_INNERKITS_WM_WINDOW_MANAGER_H
