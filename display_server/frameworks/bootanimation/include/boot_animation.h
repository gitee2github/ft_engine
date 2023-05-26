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

#ifndef FRAMEWORKS_BOOTANIMATION_INCLUDE_BOOT_ANIMATION_H
#define FRAMEWORKS_BOOTANIMATION_INCLUDE_BOOT_ANIMATION_H

#include <include/core/SkCanvas.h>
#include <include/core/SkData.h>
#include <include/core/SkImageInfo.h>
#include <include/core/SkImage.h>
#include <include/codec/SkCodec.h>
#include <display.h>
#include <foundation/window/window_manager/interfaces/innerkits/wm/window_manager.h>
#include <ipc_skeleton.h>
#include <iremote_broker.h>
#include <iservice_registry.h>
#include <platform/ohos/rs_irender_service.h>
#include <parameters.h>
#include <render_context/render_context.h>
#include <system_ability_definition.h>
#include <ui/rs_surface_extractor.h>
#include <window.h>
#include <window_option.h>
#include <window_scene.h>
#include "event_handler.h"
#include "player.h"
#include "vsync_receiver.h"
#include "util.h"

namespace OHOS {
class BootAnimation {
public:
    void Init(int32_t width, int32_t height);
    void Draw();
    void CheckExitAnimation();
    void PlaySound();
    bool CheckFrameRateValid(int32_t ratevalue);
    void Run(std::vector<sptr<OHOS::Rosen::Display>>& displays);
    ~BootAnimation();
private:
    void OnVsync();
    void OnDraw(SkCanvas* canvas, int32_t curNo);
    void InitBootWindow();
    void InitRsSurface();
    void InitPicCoordinates();
    int32_t windowWidth_;
    int32_t windowHeight_;
    sptr<OHOS::Rosen::Window> window_;
    sptr<OHOS::Rosen::WindowScene> scene_;
    std::unique_ptr<OHOS::Rosen::RSSurfaceFrame> framePtr_;
    std::shared_ptr<OHOS::Rosen::RSSurface> rsSurface_;
    OHOS::Rosen::RenderContext* rc_;
    int32_t freq_ = 30;
    int32_t realHeight_ = 0;
    int32_t realWidth_ = 0;
    int32_t pointX_ = 0;
    int32_t pointY_ = 0;
    int32_t picCurNo_ = -1;
    int32_t imgVecSize_ = 0;
    std::shared_ptr<OHOS::Rosen::VSyncReceiver> receiver_ = nullptr;
    std::shared_ptr<Media::Player> soundPlayer_ = nullptr;
    ImageStructVec imageVector_;
    std::shared_ptr<OHOS::AppExecFwk::EventHandler> mainHandler_ = nullptr;
    std::shared_ptr<AppExecFwk::EventRunner> runner_ = nullptr;
    bool setBootEvent_ = false;
};
} // namespace OHOS

#endif // FRAMEWORKS_BOOTANIMATION_INCLUDE_BOOT_ANIMATION_H
