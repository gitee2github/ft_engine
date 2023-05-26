/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, Hardware
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <test_header.h>

#include "platform/ohos/overdraw/rs_canvas_listener.h"
#include "platform/ohos/overdraw/rs_overdraw_controller.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace Rosen {
class RSOverdrawControllerTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
};

void RSOverdrawControllerTest::SetUpTestCase() {}
void RSOverdrawControllerTest::TearDownTestCase() {}
void RSOverdrawControllerTest::SetUp() {}
void RSOverdrawControllerTest::TearDown() {}

class MockRSPaintFilterCanvas : public RSPaintFilterCanvas {
public:
    explicit MockRSPaintFilterCanvas(SkCanvas *canvas) : RSPaintFilterCanvas(canvas) {}
    MOCK_METHOD2(onDrawRect, void(const SkRect& rect, const SkPaint& paint));
};

/*
 * Function: Singleton is single
 * Type: Function
 * EnvConditions: N/A
 * CaseDescription: 1. call GetInstance twice
 *                  2. check instance should be same
 */
HWTEST_F(RSOverdrawControllerTest, Singleton, Function | SmallTest | Level0)
{
    PART("CaseDescription") {
        STEP("1. call GetInstance twice") {
            auto &instance1 = RSOverdrawController::GetInstance();
            auto &instance2 = RSOverdrawController::GetInstance();
            STEP("2. check instance should be same") {
                STEP_ASSERT_EQ(&instance1, &instance2);
            }
        }
    }
}

/*
 * Function: IsEnabled/SetEnable
 * Type: Function
 * EnvConditions: N/A
 * CaseDescription: 1. save IsEnabled as e
 *                  2. SetEnable !e
 *                  3. check IsEnabled is !e
 */
HWTEST_F(RSOverdrawControllerTest, Enable, Function | SmallTest | Level0)
{
    PART("CaseDescription") {
        bool e = false;
        STEP("1. set enable as fasle") {
            RSOverdrawController::GetInstance().SetEnable(false);
            e = RSOverdrawController::GetInstance().IsEnabled();
            STEP_ASSERT_EQ(e, false);
        }

        STEP("2. set enable as false when enable is false") {
            RSOverdrawController::GetInstance().SetEnable(false);
            e = RSOverdrawController::GetInstance().IsEnabled();
            STEP_ASSERT_EQ(e, false);
        }

        STEP("3. set enable as true when enable is false") {
            RSOverdrawController::GetInstance().SetEnable(true);
            e = RSOverdrawController::GetInstance().IsEnabled();
            STEP_ASSERT_EQ(e, true);
        }

        STEP("4. set enable as true when enable is true") {
            RSOverdrawController::GetInstance().SetEnable(true);
            e = RSOverdrawController::GetInstance().IsEnabled();
            STEP_ASSERT_EQ(e, true);
        }
     
        STEP("5. set enable as false when enable is true") {
            RSOverdrawController::GetInstance().SetEnable(false);
            e = RSOverdrawController::GetInstance().IsEnabled();
            STEP_ASSERT_EQ(e, false);
        }
    }
}

/*
 * Function: SwitchFunction (true/false)
 * Type: Function
 * EnvConditions: RSOverdrawController disabled
 * CaseDescription: 1. SwitchFunction value=true
 *                  2. IsEnabled is true
 *                  3. SwitchFunction value=false
 *                  4. IsEnabled is false
 */
HWTEST_F(RSOverdrawControllerTest, SwitchFunction, Function | SmallTest | Level2)
{
    PART("EnvConditions") {
        RSOverdrawController::GetInstance().SetEnable(false);
    }

    PART("CaseDescription") {
        STEP("1. SwitchFunction value=true") {
            RSOverdrawController::SwitchFunction("", "true", &RSOverdrawController::GetInstance());
        }

        STEP("2. IsEnabled is true") {
            STEP_ASSERT_EQ(RSOverdrawController::GetInstance().IsEnabled(), true);
        }

        STEP("3. SwitchFunction value=false") {
            RSOverdrawController::SwitchFunction("", "false", &RSOverdrawController::GetInstance());
        }

        STEP("4. IsEnabled is false") {
            STEP_ASSERT_EQ(RSOverdrawController::GetInstance().IsEnabled(), false);
        }
    }
}

class RSValidCanvasListener : public RSCanvasListener {
public:
    explicit RSValidCanvasListener(SkCanvas &canvas) : RSCanvasListener(canvas) {}

    bool IsValid() const override
    {
        return true;
    }

    const char *Name() const override
    {
        return "RSValidCanvasListener";
    }
};

/*
 * Function: CreateListener in disabled
 * Type: Function
 * EnvConditions: RSOverdrawController disabled
 * CaseDescription: 1. CreateListener<RSCanvasListener>(MockRSPaintFilterCanvas) == nullptr
 *                  2. CreateListener<RSCanvasListener>(nullptr) == nullptr
 *                  3. CreateListener<RSValidCanvasListener>(MockRSPaintFilterCanvas) == nullptr
 */
HWTEST_F(RSOverdrawControllerTest, CreateListenerDisable, Function | SmallTest | Level2)
{
    auto &controller = RSOverdrawController::GetInstance();
    PART("EnvConditions") {
        controller.SetEnable(false);
    }

    PART("CaseDescription") {
        auto skCanvas = std::make_unique<SkCanvas>();
        std::shared_ptr<RSPaintFilterCanvas> canvas = nullptr;
        STEP("1. CreateListener<RSCanvasListener>(MockRSPaintFilterCanvas) == nullptr") {
            auto mockRSPaintFilterCanvas = std::make_shared<MockRSPaintFilterCanvas>(skCanvas.get());
            canvas = mockRSPaintFilterCanvas;
            auto listener = controller.CreateListener<RSCanvasListener>(canvas.get());
            STEP_ASSERT_EQ(listener, nullptr);
        }

        STEP("2. CreateListener<RSCanvasListener>(nullptr) == nullptr") {
            canvas = nullptr;
            auto listener = controller.CreateListener<RSCanvasListener>(canvas.get());
            STEP_ASSERT_EQ(listener, nullptr);
        }

        STEP("3. CreateListener<RSValidCanvasListener>(MockRSPaintFilterCanvas) == nullptr") {
            auto mockRSPaintFilterCanvas = std::make_shared<MockRSPaintFilterCanvas>(skCanvas.get());
            canvas = mockRSPaintFilterCanvas;
            auto listener = controller.CreateListener<RSValidCanvasListener>(canvas.get());
            STEP_ASSERT_EQ(listener, nullptr);
        }
    }
}

/*
 * Function: CreateListener in enabled
 * Type: Function
 * EnvConditions: RSOverdrawController enabled
 * CaseDescription: 1. CreateListener<RSCanvasListener>(nullptr) == nullptr
 *                  2. CreateListener<RSCanvasListener>(MockRSPaintFilterCanvas) == nullptr
 *                  3. CreateListener<RSValidCanvasListener>(MockRSPaintFilterCanvas) != nullptr
 */
HWTEST_F(RSOverdrawControllerTest, CreateListenerEnable, Function | SmallTest | Level2)
{
    auto &controller = RSOverdrawController::GetInstance();
    PART("EnvConditions") {
        controller.SetEnable(true);
    }

    PART("CaseDescription") {
        auto skCanvas = std::make_unique<SkCanvas>();
        std::shared_ptr<RSPaintFilterCanvas> canvas = nullptr;
        STEP("1. CreateListener<RSCanvasListener>(nullptr) == nullptr") {
            canvas = nullptr;
            auto listener = controller.CreateListener<RSCanvasListener>(canvas.get());
            STEP_ASSERT_EQ(listener, nullptr);
        }

        STEP("2. CreateListener<RSCanvasListener>(MockRSPaintFilterCanvas) == nullptr") {
            auto mockRSPaintFilterCanvas = std::make_shared<MockRSPaintFilterCanvas>(skCanvas.get());
            canvas = mockRSPaintFilterCanvas;
            auto listener = controller.CreateListener<RSCanvasListener>(canvas.get());
            STEP_ASSERT_EQ(listener, nullptr);
        }

        STEP("3. CreateListener<RSValidCanvasListener>(MockRSPaintFilterCanvas) != nullptr") {
            auto mockRSPaintFilterCanvas = std::make_shared<MockRSPaintFilterCanvas>(skCanvas.get());
            canvas = mockRSPaintFilterCanvas;
            auto listener = controller.CreateListener<RSValidCanvasListener>(canvas.get());
            STEP_ASSERT_NE(listener, nullptr);
        }
    }
}
} // namespace Rosen
} // namespace OHOS
