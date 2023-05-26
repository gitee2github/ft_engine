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

#include <cstdlib>
#include <cstring>
#include <memory>
#include <string.h>
#include <securec.h>
#include "gtest/gtest.h"
#include "gtest/hwext/gtest-tag.h"

#include "modifier/rs_render_modifier.h"
#include "pipeline/rs_recording_canvas.h"

#include "message_parcel.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS::Rosen {
class RSRenderModifierTest : public testing::Test {
public:
    constexpr static float floatData[] = {
        0.0f, 485.44f, 2.0f,
        std::numeric_limits<float>::max(), std::numeric_limits<float>::min(),
        };
    PropertyId id = 100;
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
};

void RSRenderModifierTest::SetUpTestCase() {}
void RSRenderModifierTest::TearDownTestCase() {}
void RSRenderModifierTest::SetUp() {}
void RSRenderModifierTest::TearDown() {}

/**
 * @tc.name: LifeCycle001
 * @tc.desc:
 * @tc.type:FUNC
 */
HWTEST_F(RSRenderModifierTest, LifeCycle001, TestSize.Level1)
{
    auto prop = std::make_shared<RSRenderProperty<float>>();
    auto modifier = std::make_shared<RSAlphaRenderModifier>(prop);
    ASSERT_TRUE(modifier != nullptr);
    ASSERT_TRUE(modifier->GetProperty() == prop);
    ASSERT_TRUE(modifier->GetPropertyId() == 0);

    auto prop2 = std::make_shared<RSRenderProperty<float>>(floatData[0], id);
    auto modifier2 = std::make_shared<RSAlphaRenderModifier>(prop2);
    ASSERT_TRUE(modifier2 != nullptr);
    ASSERT_EQ(modifier2->GetPropertyId(), id);
}

/**
 * @tc.name: Modifier001
 * @tc.desc:
 * @tc.type:FUNC
 */
HWTEST_F(RSRenderModifierTest, Modifier001, TestSize.Level1)
{
    auto prop = std::make_shared<RSRenderProperty<float>>(floatData[0], id);
    auto modifier = std::make_shared<RSAlphaRenderModifier>(prop);
    ASSERT_TRUE(prop != nullptr);
    ASSERT_EQ(modifier->GetProperty(), prop);

    auto prop1 = std::make_shared<RSRenderProperty<float>>(floatData[1], id);
    modifier->Update(prop1, false);
    ASSERT_EQ(std::static_pointer_cast<RSRenderProperty<float>>(modifier->GetProperty())->Get(), floatData[1]);

    auto prop2 = std::make_shared<RSRenderProperty<float>>(floatData[2], id);
    modifier->Update(prop2, true);
    ASSERT_EQ(std::static_pointer_cast<RSRenderProperty<float>>(modifier->GetProperty())->Get(),
        floatData[1] + floatData[2]);
}

/**
 * @tc.name: DrawCmdListModifier001
 * @tc.desc:
 * @tc.type:FUNC
 */
HWTEST_F(RSRenderModifierTest, DrawCmdListModifier001, TestSize.Level1)
{
    RSRecordingCanvas canvas(100, 100);
    canvas.translate(15.f, 15.f);
    auto prop = std::make_shared<RSRenderProperty<DrawCmdListPtr>>(canvas.GetDrawCmdList(), id);
    auto modifier = std::make_shared<RSDrawCmdListRenderModifier>(prop);

    MessageParcel parcel;
    ASSERT_TRUE(modifier->Marshalling(parcel));
    ASSERT_TRUE(RSDrawCmdListRenderModifier::Unmarshalling(parcel) != nullptr);

    canvas.scale(2.f, 2.f);
    modifier->Update(nullptr, false);
    auto prop1 = std::make_shared<RSRenderProperty<DrawCmdListPtr>>(canvas.GetDrawCmdList(), id);
    modifier->Update(prop1, true);

    std::shared_ptr<RectI> rect = std::make_shared<RectI>();
    modifier->SetOverlayBounds(rect);
    ASSERT_TRUE(modifier->GetOverlayBounds() == rect);
    ASSERT_TRUE(modifier->Marshalling(parcel));
    ASSERT_TRUE(RSDrawCmdListRenderModifier::Unmarshalling(parcel) != nullptr);

    MessageParcel parcel1;
    char* buffer = static_cast<char *>(malloc(parcel1.GetMaxCapacity()));
    memset_s(buffer, parcel1.GetMaxCapacity(), 0, parcel1.GetMaxCapacity());
    ASSERT_TRUE(parcel1.WriteUnpadBuffer(buffer, parcel1.GetMaxCapacity()));
    bool ret = false;
    while (!ret) {
        ret = (modifier->Marshalling(parcel) & (RSDrawCmdListRenderModifier::Unmarshalling(parcel) != nullptr));
        parcel1.SetMaxCapacity(parcel1.GetMaxCapacity() + 1);
    }
    free(buffer);
    ASSERT_TRUE(ret);
}
}
