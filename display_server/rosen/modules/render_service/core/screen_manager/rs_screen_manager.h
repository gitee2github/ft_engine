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

#ifndef RS_SCREEN_MANAGER
#define RS_SCREEN_MANAGER

#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <unordered_map>

#include <hdi_backend.h>
#include <ipc_callbacks/screen_change_callback.h>
#include <refbase.h>
#include <screen_manager/rs_screen_props.h>
#include <screen_manager/rs_screen_mode_info.h>
#include <screen_manager/rs_screen_capability.h>
#include <screen_manager/rs_screen_data.h>
#include <screen_manager/rs_screen_hdr_capability.h>
#include <screen_manager/screen_types.h>
#include <screen_manager/rs_virtual_screen_resolution.h>
#include <surface.h>

#include "rs_screen.h"

namespace OHOS {
namespace Rosen {
enum class ScreenState : uint8_t {
    HDI_OUTPUT_ENABLE,
    PRODUCER_SURFACE_ENABLE,
    DISABLED,
    NOT_EXISTED,
    UNKNOWN,
};

struct ScreenInfo {
    ScreenId id = INVALID_SCREEN_ID;
    uint32_t width = 0;
    uint32_t height = 0;
    ScreenColorGamut colorGamut = ScreenColorGamut::COLOR_GAMUT_SRGB;
    ScreenState state = ScreenState::UNKNOWN;
    ScreenRotation rotation = ScreenRotation::ROTATION_0;

    uint32_t skipFrameInterval = DEFAULT_SKIP_FRAME_INTERVAL;  // skip frame interval for change screen refresh rate

    uint32_t GetRotatedWidth() const
    {
        return (rotation == ScreenRotation::ROTATION_0 || rotation == ScreenRotation::ROTATION_180) ? width : height;
    }

    uint32_t GetRotatedHeight() const
    {
        return (rotation == ScreenRotation::ROTATION_0 || rotation == ScreenRotation::ROTATION_180) ? height : width;
    }
};

class RSScreenManager : public RefBase {
public:
    RSScreenManager() = default;
    virtual ~RSScreenManager() noexcept = default;

    virtual bool Init() noexcept = 0;

    // get default/primary screen id.
    virtual ScreenId GetDefaultScreenId() const = 0;

    virtual std::vector<ScreenId> GetAllScreenIds() = 0;

    virtual void SetDefaultScreenId(ScreenId id) = 0;

    virtual void SetScreenMirror(ScreenId id, ScreenId toMirror) = 0;

    virtual ScreenId CreateVirtualScreen(
        const std::string &name,
        uint32_t width,
        uint32_t height,
        sptr<Surface> surface,
        ScreenId mirrorId = 0,
        int flags = 0) = 0;

    virtual int32_t SetVirtualScreenSurface(ScreenId id, sptr<Surface> surface) = 0;

    virtual void RemoveVirtualScreen(ScreenId id) = 0;

    virtual void SetScreenActiveMode(ScreenId id, uint32_t modeId) = 0;

    virtual int32_t SetVirtualScreenResolution(ScreenId id, uint32_t width, uint32_t height) = 0;

    virtual void SetScreenPowerStatus(ScreenId id, ScreenPowerStatus status) = 0;

    virtual void GetVirtualScreenResolution(ScreenId id, RSVirtualScreenResolution& virtualScreenResolution) const = 0;

    virtual void GetScreenActiveMode(ScreenId id, RSScreenModeInfo& screenModeInfo) const = 0;

    virtual std::vector<RSScreenModeInfo> GetScreenSupportedModes(ScreenId id) const = 0;

    virtual RSScreenCapability GetScreenCapability(ScreenId id) const = 0;

    virtual ScreenPowerStatus GetScreenPowerStatus(ScreenId id) const = 0;

    virtual RSScreenData GetScreenData(ScreenId id) const = 0;

    virtual ScreenInfo QueryScreenInfo(ScreenId id) const = 0;

    // Can only be called after QueryScreenState and the state is ScreenState::PRODUCER_SURFACE_ENABLE;
    virtual sptr<Surface> GetProducerSurface(ScreenId id) const = 0;

    // Can only be called after QueryScreenState and the state is ScreenState::HDI_OUTPUT_ENABLE;
    virtual std::shared_ptr<HdiOutput> GetOutput(ScreenId id) const = 0;

    virtual int32_t AddScreenChangeCallback(const sptr<RSIScreenChangeCallback> &callback) = 0;

    virtual void RemoveScreenChangeCallback(const sptr<RSIScreenChangeCallback> &callback) = 0;

    virtual void ProcessScreenHotPlugEvents() = 0;

    virtual void DisplayDump(std::string& dumpString) = 0;

    virtual void SurfaceDump(std::string& dumpString) = 0;

    virtual void FpsDump(std::string& dumpString, std::string& arg) = 0;

    virtual void ClearFpsDump(std::string& dumpString, std::string& arg) = 0;

    virtual int32_t GetScreenBacklight(ScreenId id) = 0;

    virtual void SetScreenBacklight(ScreenId id, uint32_t level) = 0;

    virtual int32_t GetScreenSupportedColorGamuts(ScreenId id, std::vector<ScreenColorGamut>& mode) const = 0;

    virtual int32_t GetScreenSupportedMetaDataKeys(ScreenId id, std::vector<ScreenHDRMetadataKey>& keys) const = 0;

    virtual int32_t GetScreenColorGamut(ScreenId id, ScreenColorGamut& mode) const = 0;

    virtual int32_t SetScreenColorGamut(ScreenId id, int32_t modeIdx) = 0;

    virtual int32_t SetScreenGamutMap(ScreenId id, ScreenGamutMap mode) = 0;

    virtual int32_t GetScreenGamutMap(ScreenId id, ScreenGamutMap& mode) const = 0;

    virtual int32_t GetScreenHDRCapability(ScreenId id, RSScreenHDRCapability& screenHdrCapability) const = 0;

    virtual int32_t GetScreenType(ScreenId id, RSScreenType& type) const = 0;

    virtual int32_t SetScreenSkipFrameInterval(ScreenId id, uint32_t skipFrameInterval) = 0;

    /* only used for mock tests */
    virtual void MockHdiScreenConnected(std::unique_ptr<impl::RSScreen>& rsScreen) = 0;
};

sptr<RSScreenManager> CreateOrGetScreenManager();

namespace impl {
struct ScreenHotPlugEvent {
    std::shared_ptr<HdiOutput> output;
    bool connected = false;
};

class RSScreenManager : public OHOS::Rosen::RSScreenManager {
public:
    static sptr<OHOS::Rosen::RSScreenManager> GetInstance() noexcept;

    // noncopyable
    RSScreenManager(const RSScreenManager &) = delete;
    RSScreenManager &operator=(const RSScreenManager &) = delete;

    bool Init() noexcept override;

    ScreenId GetDefaultScreenId() const override
    {
        return defaultScreenId_;
    }

    std::vector<ScreenId> GetAllScreenIds() override;

    void SetDefaultScreenId(ScreenId id) override;

    void SetScreenMirror(ScreenId id, ScreenId toMirror) override;

    ScreenId CreateVirtualScreen(
        const std::string &name,
        uint32_t width,
        uint32_t height,
        sptr<Surface> surface,
        ScreenId mirrorId,
        int32_t flags) override;

    int32_t SetVirtualScreenSurface(ScreenId id, sptr<Surface> surface) override;

    void RemoveVirtualScreen(ScreenId id) override;

    void SetScreenActiveMode(ScreenId id, uint32_t modeId) override;

    int32_t SetVirtualScreenResolution(ScreenId id, uint32_t width, uint32_t height) override;

    void SetScreenPowerStatus(ScreenId id, ScreenPowerStatus status) override;

    void GetVirtualScreenResolution(ScreenId id, RSVirtualScreenResolution& virtualScreenResolution) const override;

    void GetScreenActiveMode(ScreenId id, RSScreenModeInfo& screenModeInfo) const override;

    std::vector<RSScreenModeInfo> GetScreenSupportedModes(ScreenId id) const override;

    RSScreenCapability GetScreenCapability(ScreenId id) const override;

    ScreenPowerStatus GetScreenPowerStatus(ScreenId id) const override;

    RSScreenData GetScreenData(ScreenId id) const  override;

    ScreenInfo QueryScreenInfo(ScreenId id) const override;

    sptr<Surface> GetProducerSurface(ScreenId id) const override;

    std::shared_ptr<HdiOutput> GetOutput(ScreenId id) const override;

    int32_t AddScreenChangeCallback(const sptr<RSIScreenChangeCallback> &callback) override;

    void RemoveScreenChangeCallback(const sptr<RSIScreenChangeCallback> &callback) override;

    void ProcessScreenHotPlugEvents() override;

    void DisplayDump(std::string& dumpString) override;

    void SurfaceDump(std::string& dumpString) override;

    void FpsDump(std::string& dumpString, std::string& arg) override;

    void ClearFpsDump(std::string& dumpString, std::string& arg) override;

    int32_t GetScreenBacklight(ScreenId id) override;

    void SetScreenBacklight(ScreenId id, uint32_t level) override;

    int32_t GetScreenSupportedColorGamuts(ScreenId id, std::vector<ScreenColorGamut>& mode) const override;

    int32_t GetScreenSupportedMetaDataKeys(ScreenId id, std::vector<ScreenHDRMetadataKey>& keys) const override;

    int32_t GetScreenColorGamut(ScreenId id, ScreenColorGamut& mode) const override;

    int32_t SetScreenColorGamut(ScreenId id, int32_t modeIdx) override;

    int32_t SetScreenGamutMap(ScreenId id, ScreenGamutMap mode) override;

    int32_t GetScreenGamutMap(ScreenId id, ScreenGamutMap& mode) const override;

    int32_t GetScreenHDRCapability(ScreenId id, RSScreenHDRCapability& screenHdrCapability) const override;

    int32_t GetScreenType(ScreenId id, RSScreenType& type) const override;

    int32_t SetScreenSkipFrameInterval(ScreenId id, uint32_t skipFrameInterval) override;
    
    /* only used for mock tests */
    void MockHdiScreenConnected(std::unique_ptr<impl::RSScreen>& rsScreen) override
    {
        if (rsScreen == nullptr) {
            return;
        }
        screens_[rsScreen->Id()] = std::move(rsScreen);
    }
private:
    RSScreenManager();
    ~RSScreenManager() noexcept override;

    static void OnHotPlug(std::shared_ptr<HdiOutput> &output, bool connected, void *data);
    void OnHotPlugEvent(std::shared_ptr<HdiOutput> &output, bool connected);
    void ProcessScreenConnectedLocked(std::shared_ptr<HdiOutput> &output);
    void ProcessScreenDisConnectedLocked(std::shared_ptr<HdiOutput> &output);
    void HandleDefaultScreenDisConnectedLocked();
    std::vector<ScreenHotPlugEvent> pendingHotPlugEvents_;

    void GetVirtualScreenResolutionLocked(ScreenId id, RSVirtualScreenResolution& virtualScreenResolution) const;
    void GetScreenActiveModeLocked(ScreenId id, RSScreenModeInfo& screenModeInfo) const;
    std::vector<RSScreenModeInfo> GetScreenSupportedModesLocked(ScreenId id) const;
    RSScreenCapability GetScreenCapabilityLocked(ScreenId id) const;
    ScreenPowerStatus GetScreenPowerStatusLocked(ScreenId id) const;
    int32_t GetScreenBacklightLocked(ScreenId id) const;

    void RemoveVirtualScreenLocked(ScreenId id);
    ScreenId GenerateVirtualScreenIdLocked();
    void ReuseVirtualScreenIdLocked(ScreenId id);
    void MirrorChangeDefaultScreenResolution(ScreenId id, uint32_t width, uint32_t height);
    ScreenId GetMirrorScreenId(ScreenId id);

    int32_t GetScreenSupportedColorGamutsLocked(ScreenId id, std::vector<ScreenColorGamut>& mode) const;
    int32_t GetScreenSupportedMetaDataKeysLocked(ScreenId id, std::vector<ScreenHDRMetadataKey>& keys) const;
    int32_t GetScreenColorGamutLocked(ScreenId id, ScreenColorGamut& mode) const;
    int32_t SetScreenColorGamutLocked(ScreenId id, int32_t modeIdx);
    int32_t SetScreenGamutMapLocked(ScreenId id, ScreenGamutMap mode);
    int32_t GetScreenGamutMapLocked(ScreenId id, ScreenGamutMap& mode) const;
    int32_t GetScreenHDRCapabilityLocked(ScreenId id, RSScreenHDRCapability& screenHdrCapability) const;
    int32_t GetScreenTypeLocked(ScreenId id, RSScreenType& type) const;
    int32_t SetScreenSkipFrameIntervalLocked(ScreenId id, uint32_t skipFrameInterval);

    mutable std::mutex mutex_;
    HdiBackend *composer_ = nullptr;
    ScreenId defaultScreenId_ = INVALID_SCREEN_ID;
    std::unordered_map<ScreenId, std::unique_ptr<OHOS::Rosen::RSScreen>> screens_;
    std::queue<ScreenId> freeVirtualScreenIds_;
    uint32_t maxVirtualScreenNum_ = 0;
    std::vector<sptr<RSIScreenChangeCallback>> screenChangeCallbacks_;
    bool mipiCheckInFirstHotPlugEvent_ = false;
    std::vector<ScreenId> connectedIds_;
    std::unordered_map<ScreenId, uint32_t> screenPowerStatus_;

    static std::once_flag createFlag_;
    static sptr<OHOS::Rosen::RSScreenManager> instance_;
};
} // namespace impl
} // namespace Rosen
} // namespace OHOS
#endif // RS_SCREEN_MANAGER
