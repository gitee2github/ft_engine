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

#ifndef RS_SCREEN
#define RS_SCREEN

#include <memory>
#include <optional>

#include <hdi_output.h>
#include <hdi_screen.h>
#include <screen_manager/screen_types.h>

namespace OHOS {
namespace Rosen {
struct VirtualScreenConfigs {
    ScreenId id = INVALID_SCREEN_ID;
    ScreenId mirrorId = INVALID_SCREEN_ID;
    std::string name;
    uint32_t width = 0;
    uint32_t height = 0;
    sptr<Surface> surface = nullptr;
    int32_t flags = 0; // reserve flag.
};

class RSScreen {
public:
    RSScreen() = default;
    virtual ~RSScreen() noexcept = default;

    virtual ScreenId Id() const = 0;
    virtual ScreenId MirrorId() const = 0;
    virtual void SetMirror(ScreenId mirrorId) = 0;
    virtual const std::string& Name() const = 0;
    virtual uint32_t Width() const = 0;
    virtual uint32_t Height() const = 0;
    virtual bool IsEnable() const = 0;
    virtual bool IsVirtual() const = 0;
    virtual void SetActiveMode(uint32_t modeId) = 0;
    virtual void SetResolution(uint32_t width, uint32_t height) = 0;
    virtual void SetPowerStatus(uint32_t powerStatus) = 0;
    virtual std::optional<DisplayModeInfo> GetActiveMode() const = 0;
    virtual const std::vector<DisplayModeInfo>& GetSupportedModes() const = 0;
    virtual const DisplayCapability& GetCapability() const = 0;
    virtual uint32_t GetPowerStatus() const = 0;
    virtual std::shared_ptr<HdiOutput> GetOutput() const = 0;
    virtual sptr<Surface> GetProducerSurface() const = 0;
    virtual void SetProducerSurface(sptr<Surface> producerSurface) = 0;
    virtual void DisplayDump(int32_t screenIndex, std::string& dumpString) = 0;
    virtual void SurfaceDump(int32_t screenIndex, std::string& dumpString) = 0;
    virtual void FpsDump(int32_t screenIndex, std::string& dumpString, std::string& arg) = 0;
    virtual void ClearFpsDump(int32_t screenIndex, std::string& dumpString, std::string& arg) = 0;
    virtual void SetScreenBacklight(uint32_t level) = 0;
    virtual int32_t GetScreenBacklight() const = 0;
    virtual int32_t GetScreenSupportedColorGamuts(std::vector<ScreenColorGamut> &mode) const = 0;
    virtual int32_t GetScreenSupportedMetaDataKeys(std::vector<ScreenHDRMetadataKey> &keys) const = 0;
    virtual int32_t GetScreenColorGamut(ScreenColorGamut &mode) const = 0;
    virtual int32_t SetScreenColorGamut(int32_t modeIdx) = 0;
    virtual int32_t SetScreenGamutMap(ScreenGamutMap mode) = 0;
    virtual int32_t GetScreenGamutMap(ScreenGamutMap &mode) const = 0;
    virtual int32_t GetActiveModePosByModeId(int32_t modeId) const = 0;
    virtual const HDRCapability& GetHDRCapability() = 0;
    virtual const RSScreenType& GetScreenType() const = 0;
    virtual void SetScreenSkipFrameInterval(uint32_t skipFrameInterval) = 0;
    virtual uint32_t GetScreenSkipFrameInterval() const = 0;
    virtual void SetScreenVsyncEnabled(bool enabled) const = 0;
};

namespace impl {
class RSScreen : public OHOS::Rosen::RSScreen {
public:
    RSScreen(
        ScreenId id,
        bool isVirtual,
        std::shared_ptr<HdiOutput> output,
        sptr<Surface> surface);
    RSScreen(const VirtualScreenConfigs &configs);
    ~RSScreen() noexcept override;

    RSScreen(const RSScreen &) = delete;
    RSScreen &operator=(const RSScreen &) = delete;

    ScreenId Id() const override;
    ScreenId MirrorId() const override;
    void SetMirror(ScreenId mirrorId) override;
    const std::string& Name() const override;
    uint32_t Width() const override;
    uint32_t Height() const override;
    bool IsEnable() const override;
    bool IsVirtual() const override;
    void SetActiveMode(uint32_t modeId) override;
    void SetResolution(uint32_t width, uint32_t height) override;
    void SetPowerStatus(uint32_t powerStatus) override;
    std::optional<DisplayModeInfo> GetActiveMode() const override;
    const std::vector<DisplayModeInfo>& GetSupportedModes() const override;
    const DisplayCapability& GetCapability() const override;
    uint32_t GetPowerStatus() const override;
    std::shared_ptr<HdiOutput> GetOutput() const override;
    sptr<Surface> GetProducerSurface() const override;
    void SetProducerSurface(sptr<Surface> producerSurface) override;
    void DisplayDump(int32_t screenIndex, std::string& dumpString) override;
    void SurfaceDump(int32_t screenIndex, std::string& dumpString) override;
    void FpsDump(int32_t screenIndex, std::string& dumpString, std::string& arg) override;
    void ClearFpsDump(int32_t screenIndex, std::string& dumpString, std::string& arg) override;
    void SetScreenBacklight(uint32_t level) override;
    int32_t GetScreenBacklight() const override;
    int32_t GetScreenSupportedColorGamuts(std::vector<ScreenColorGamut> &mode) const override;
    int32_t GetScreenSupportedMetaDataKeys(std::vector<ScreenHDRMetadataKey> &keys) const override;
    int32_t GetScreenColorGamut(ScreenColorGamut &mode) const override;
    int32_t SetScreenColorGamut(int32_t modeIdx) override;
    int32_t SetScreenGamutMap(ScreenGamutMap mode) override;
    int32_t GetScreenGamutMap(ScreenGamutMap &mode) const override;
    int32_t GetActiveModePosByModeId(int32_t modeId) const override;
    const HDRCapability& GetHDRCapability() override;
    const RSScreenType& GetScreenType() const override;
    void SetScreenSkipFrameInterval(uint32_t skipFrameInterval) override;
    uint32_t GetScreenSkipFrameInterval() const override;
    void SetScreenVsyncEnabled(bool enabled) const override;

private:
    // create hdiScreen and get some information from drivers.
    void PhysicalScreenInit() noexcept;

    void ModeInfoDump(std::string& dumpString);
    void CapabilityDump(std::string& dumpString);
    void PropDump(std::string& dumpString);
    void PowerStatusDump(std::string& dumpString);
    void CapabilityTypeDump(InterfaceType capabilityType, std::string& dumpString);
    void ScreenTypeDump(std::string& dumpString);

    // ScreenId for this screen.
    ScreenId id_ = INVALID_SCREEN_ID;
    // If this screen is the mirror of other screen, this member would be a valid id.
    ScreenId mirrorId_ = INVALID_SCREEN_ID;

    std::string name_;

    int32_t width_ = 0;
    int32_t height_ = 0;

    bool isVirtual_ = true;
    std::shared_ptr<HdiOutput> hdiOutput_; // has value if the screen is physical
    std::unique_ptr<HdiScreen> hdiScreen_; // has value if the screen is physical
    std::vector<DisplayModeInfo> supportedModes_;
    DisplayCapability capability_ = {"", ::DISP_INTF_HDMI, 0, 0, 0, 0, true, 0, nullptr};
    HDRCapability hdrCapability_ = {0, nullptr, 0, 0, 0};
    sptr<Surface> producerSurface_;  // has value if the screen is virtual
    DispPowerStatus powerStatus_ = ::POWER_STATUS_ON;

    std::vector<ScreenColorGamut> supportedVirtualColorGamuts_ = {
        COLOR_GAMUT_SRGB,
        COLOR_GAMUT_DCI_P3,
        COLOR_GAMUT_ADOBE_RGB };
    int32_t currentVirtualColorGamutIdx_ = 0;
    ScreenGamutMap currentVirtualGamutMap_ = GAMUT_MAP_CONSTANT;
    RSScreenType screenType_ = RSScreenType::UNKNOWN_TYPE_SCREEN;
    uint32_t skipFrameInterval_ = DEFAULT_SKIP_FRAME_INTERVAL;
};
} // namespace impl
} // namespace Rosen
} // namespace OHOS

#endif // RS_SCREEN
