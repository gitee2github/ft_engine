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

#include "hdi_screen.h"
#include "mock_hdi_device.h"

#include <gtest/gtest.h>

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace Rosen {
class HdiScreenTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    static void CheckNullFunc();

    static inline std::unique_ptr<HdiScreen> hdiScreen_;
    static inline Mock::HdiDevice* mockDevice_;
};

void HdiScreenTest::CheckNullFunc()
{
    DisplayCapability dcap;
    ASSERT_EQ(hdiScreen_->GetScreenCapability(dcap), DISPLAY_NULL_PTR);
    std::vector<DisplayModeInfo> modes;
    ASSERT_EQ(hdiScreen_->GetScreenSupportedModes(modes), DISPLAY_NULL_PTR);
    uint32_t modeId = 0;
    ASSERT_EQ(hdiScreen_->GetScreenMode(modeId), DISPLAY_NULL_PTR);
    ASSERT_EQ(hdiScreen_->SetScreenMode(modeId), DISPLAY_NULL_PTR);
    DispPowerStatus status = POWER_STATUS_ON;
    ASSERT_EQ(hdiScreen_->GetScreenPowerStatus(status), DISPLAY_NULL_PTR);
    ASSERT_EQ(hdiScreen_->SetScreenPowerStatus(status), DISPLAY_NULL_PTR);
    uint32_t level = 0;
    ASSERT_EQ(hdiScreen_->GetScreenBacklight(level), DISPLAY_NULL_PTR);
    ASSERT_EQ(hdiScreen_->SetScreenBacklight(level), DISPLAY_NULL_PTR);
    bool enabled = false;
    ASSERT_EQ(hdiScreen_->SetScreenVsyncEnabled(enabled), DISPLAY_NULL_PTR);
    std::vector<GraphicColorGamut> gamuts;
    ASSERT_EQ(hdiScreen_->GetScreenSupportedColorGamuts(gamuts), DISPLAY_NULL_PTR);
    GraphicColorGamut gamut = GRAPHIC_COLOR_GAMUT_INVALID;
    ASSERT_EQ(hdiScreen_->SetScreenColorGamut(gamut), DISPLAY_NULL_PTR);
    ASSERT_EQ(hdiScreen_->GetScreenColorGamut(gamut), DISPLAY_NULL_PTR);
    GamutMap gamutMap = GAMUT_MAP_CONSTANT;
    ASSERT_EQ(hdiScreen_->SetScreenGamutMap(gamutMap), DISPLAY_NULL_PTR);
    ASSERT_EQ(hdiScreen_->GetScreenGamutMap(gamutMap), DISPLAY_NULL_PTR);
    float *matrix;
    ASSERT_EQ(hdiScreen_->SetScreenColorTransform(matrix), DISPLAY_NULL_PTR);
    HDRCapability infos;
    ASSERT_EQ(hdiScreen_->GetHDRCapabilityInfos(infos), DISPLAY_NULL_PTR);
    std::vector<GraphicHDRMetadataKey> keys;
    ASSERT_EQ(hdiScreen_->GetSupportedMetaDataKey(keys), DISPLAY_NULL_PTR);
}

void HdiScreenTest::SetUpTestCase()
{
    uint32_t screenId = 0;
    hdiScreen_ = HdiScreen::CreateHdiScreen(screenId);
    HdiScreenTest::CheckNullFunc();
    mockDevice_ = Mock::HdiDevice::GetInstance();

    EXPECT_CALL(*mockDevice_, GetScreenCapability(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, GetScreenSupportedModes(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, GetScreenMode(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, SetScreenMode(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, GetScreenPowerStatus(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, SetScreenPowerStatus(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, GetScreenBacklight(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, SetScreenBacklight(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, GetScreenSupportedColorGamuts(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, SetScreenColorGamut(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, GetScreenColorGamut(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, SetScreenGamutMap(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, GetScreenGamutMap(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, GetHDRCapabilityInfos(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, GetSupportedMetaDataKey(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, SetScreenVsyncEnabled(_, _)).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(*mockDevice_, SetScreenColorTransform(_, _)).WillRepeatedly(testing::Return(0));
    // set hdiScreen_->hdidevice_ to null
    hdiScreen_->SetHdiDevice(nullptr);
    // set hdiScreen_->hdidevice_ to mockDevice_
    hdiScreen_->SetHdiDevice(mockDevice_);
    // reset hdiScreen_->hdidevice_ to mockDevice_
    hdiScreen_->SetHdiDevice(mockDevice_);
}

void HdiScreenTest::TearDownTestCase() {}

namespace {
/*
* Function: Init001
* Type: Function
* Rank: Important(3)
* EnvConditions: N/A
* CaseDescription: 1. call Init001
*                  2. check ret
*/
HWTEST_F(HdiScreenTest, Init001, Function | MediumTest| Level3)
{
    hdiScreen_->OnVsync(0, 0, nullptr);
    hdiScreen_->OnVsync(0, 1, nullptr);
    ASSERT_EQ(HdiScreenTest::hdiScreen_->Init(), true);
}

/*
* Function: GetScreenCapability001
* Type: Function
* Rank: Important(3)
* EnvConditions: N/A
* CaseDescription: 1. call GetScreenCapability
*                  2. check ret
*/
HWTEST_F(HdiScreenTest, GetScreenCapability001, Function | MediumTest| Level3)
{
    DisplayCapability displayCapability = {
        .name = "test",
        .type = InterfaceType::DISP_INTF_BT1120,
        .phyWidth = 800,
        .phyHeight = 600,
        .supportLayers = 1,
        .virtualDispCount = 1,
        .supportWriteBack = true,
        .propertyCount = 1,
        .props = nullptr,
    };
    ASSERT_EQ(HdiScreenTest::hdiScreen_->GetScreenCapability(displayCapability), 0);
}

/*
* Function: GetScreenSupportedModes001
* Type: Function
* Rank: Important(3)
* EnvConditions: N/A
* CaseDescription: 1. call GetScreenSupportedModes
*                  2. check ret
*/
HWTEST_F(HdiScreenTest, GetScreenSupportedModes001, Function | MediumTest| Level3)
{
    DisplayModeInfo displayModeInfo = {
        .width = 800,
        .height = 600,
        .freshRate = 60,
        .id = 0,
    };
    std::vector<DisplayModeInfo> modeInfo = { displayModeInfo };
    ASSERT_EQ(HdiScreenTest::hdiScreen_->GetScreenSupportedModes(modeInfo), 0);
}

/*
* Function: GetScreenMode001
* Type: Function
* Rank: Important(3)
* EnvConditions: N/A
* CaseDescription: 1. call GetScreenMode
*                  2. check ret
*/
HWTEST_F(HdiScreenTest, GetScreenMode001, Function | MediumTest| Level3)
{
    uint32_t modeId = 0;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->GetScreenMode(modeId), 0);
}

/*
* Function: SetScreenMode001
* Type: Function
* Rank: Important(3)
* EnvConditions: N/A
* CaseDescription: 1. call SetScreenMode
*                  2. check ret
*/
HWTEST_F(HdiScreenTest, SetScreenMode001, Function | MediumTest| Level3)
{
    uint32_t modeId = 0;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->SetScreenMode(modeId), 0);
}

/*
* Function: GetScreenPowerStatus001
* Type: Function
* Rank: Important(3)
* EnvConditions: N/A
* CaseDescription: 1. call GetScreenPowerStatus
*                  2. check ret
*/
HWTEST_F(HdiScreenTest, GetScreenPowerStatus001, Function | MediumTest| Level3)
{
    DispPowerStatus dispPowerStatus = DispPowerStatus::POWER_STATUS_ON;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->GetScreenPowerStatus(dispPowerStatus), 0);
}

/*
* Function: SetScreenPowerStatus001
* Type: Function
* Rank: Important(3)
* EnvConditions: N/A
* CaseDescription: 1. call SetScreenPowerStatus
*                  2. check ret
*/
HWTEST_F(HdiScreenTest, SetScreenPowerStatus001, Function | MediumTest| Level3)
{
    DispPowerStatus dispPowerStatus = DispPowerStatus::POWER_STATUS_ON;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->SetScreenPowerStatus(dispPowerStatus), 0);
}

/*
* Function: GetScreenBacklight001
* Type: Function
* Rank: Important(3)
* EnvConditions: N/A
* CaseDescription: 1. call GetScreenBacklight
*                  2. check ret
*/
HWTEST_F(HdiScreenTest, GetScreenBacklight001, Function | MediumTest| Level3)
{
    uint32_t level = 0;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->GetScreenBacklight(level), 0);
}

/*
* Function: SetScreenBacklight001
* Type: Function
* Rank: Important(3)
* EnvConditions: N/A
* CaseDescription: 1. call SetScreenBacklight
*                  2. check ret
*/
HWTEST_F(HdiScreenTest, SetScreenBacklight001, Function | MediumTest| Level3)
{
    uint32_t level = 0;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->SetScreenBacklight(level), 0);
}

/*
* Function: SetScreenVsyncEnabled001
* Type: Function
* Rank: Important(3)
* EnvConditions: N/A
* CaseDescription: 1. call SetScreenVsyncEnabled
*                  2. check ret
*/
HWTEST_F(HdiScreenTest, SetScreenVsyncEnabled001, Function | MediumTest| Level3)
{
    bool enabled = false;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->SetScreenVsyncEnabled(enabled), 0);
}

/*
* Function: SetScreenColorTransform001
* Type: Function
* Rank: Important(3)
* EnvConditions: N/A
* CaseDescription: 1. call SetScreenColorTransform
*                  2. check ret
*/
HWTEST_F(HdiScreenTest, SetScreenColorTransform001, Function | MediumTest| Level3)
{
    float *matrix = nullptr;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->SetScreenColorTransform(matrix), 0);
}

/*
* Function: GetScreenSupportedColorGamuts001
* Type: Function
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call GetScreenSupportedColorGamuts
*                  2. check ret
 */
HWTEST_F(HdiScreenTest, GetScreenSupportedColorGamuts001, Function | MediumTest | Level2)
{
    GraphicColorGamut colorGamut = GraphicColorGamut::GRAPHIC_COLOR_GAMUT_DCI_P3;
    std::vector<GraphicColorGamut> gamuts = { colorGamut };
    ASSERT_EQ(HdiScreenTest::hdiScreen_->GetScreenSupportedColorGamuts(gamuts), 0);
}

/*
* Function: SetScreenColorGamut001
* Type: Function
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call SetScreenColorGamut
*                  2. check ret
 */
HWTEST_F(HdiScreenTest, SetScreenColorGamut001, Function | MediumTest | Level2)
{
    GraphicColorGamut colorGamut = GraphicColorGamut::GRAPHIC_COLOR_GAMUT_DCI_P3;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->SetScreenColorGamut(colorGamut), 0);
}

/*
* Function: GetScreenColorGamut001
* Type: Function
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call GetScreenColorGamut
*                  2. check ret
 */
HWTEST_F(HdiScreenTest, GetScreenColorGamut001, Function | MediumTest | Level2)
{
    GraphicColorGamut colorGamut = GraphicColorGamut::GRAPHIC_COLOR_GAMUT_DCI_P3;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->GetScreenColorGamut(colorGamut), 0);
}

/*
* Function: SetScreenGamutMap001
* Type: Function
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call SetScreenGamutMap
*                  2. check ret
 */
HWTEST_F(HdiScreenTest, SetScreenGamutMap001, Function | MediumTest | Level2)
{
    GamutMap gamutMap = GamutMap::GAMUT_MAP_CONSTANT;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->SetScreenGamutMap(gamutMap), 0);
}

/*
* Function: GetScreenGamutMap001
* Type: Function
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call GetScreenGamutMap
*                  2. check ret
 */
HWTEST_F(HdiScreenTest, GetScreenGamutMap001, Function | MediumTest | Level2)
{
    GamutMap gamutMap = GamutMap::GAMUT_MAP_CONSTANT;
    ASSERT_EQ(HdiScreenTest::hdiScreen_->GetScreenGamutMap(gamutMap), 0);
}

/*
* Function: GetHDRCapabilityInfos001
* Type: Function
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call GetHDRCapabilityInfos
*                  2. check ret
 */
HWTEST_F(HdiScreenTest, GetHDRCapabilityInfos001, Function | MediumTest | Level2)
{
    HDRCapability capaility = {
        .formatCount = 0,
        .formats = nullptr,
        .maxLum = 0,
        .maxAverageLum = 0,
        .minLum = 0,
    };
    ASSERT_EQ(HdiScreenTest::hdiScreen_->GetHDRCapabilityInfos(capaility), 0);
}

/*
* Function: GetSupportedMetaDataKey001
* Type: Function
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call GetSupportedMetaDataKey
*                  2. check ret
 */
HWTEST_F(HdiScreenTest, GetSupportedMetaDataKey001, Function | MediumTest | Level2)
{
    GraphicHDRMetadataKey key = GraphicHDRMetadataKey::GRAPHIC_MATAKEY_RED_PRIMARY_X;
    std::vector<GraphicHDRMetadataKey> keys = { key };
    ASSERT_EQ(HdiScreenTest::hdiScreen_->GetSupportedMetaDataKey(keys), 0);
}
} // namespace
} // namespace Rosen
} // namespace OHOS