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
#include <include/core/SkDrawable.h>
#include <include/core/SkMatrix.h>
#include <include/core/SkPath.h>
#include <include/core/SkPicture.h>
#include <include/core/SkRRect.h>
#include <include/core/SkRegion.h>
#include <include/core/SkTextBlob.h>
#include <test_header.h>

#include "platform/ohos/overdraw/rs_cpu_overdraw_canvas_listener.h"
#include "platform/ohos/overdraw/rs_listened_canvas.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace Rosen {
class RSCPUOverdrawCanvasListenerTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
};

void RSCPUOverdrawCanvasListenerTest::SetUpTestCase() {}
void RSCPUOverdrawCanvasListenerTest::TearDownTestCase() {}
void RSCPUOverdrawCanvasListenerTest::SetUp() {}
void RSCPUOverdrawCanvasListenerTest::TearDown() {}

class MockSkCanvas : public SkCanvas {
public:
    MOCK_METHOD2(onDrawRegion, void(const SkRegion& region, const SkPaint& paint));
};

/*
 * Function: mock draw 3 intersecting regions
 * Type: Function
 * EnvConditions: N/A
 * CaseDescription: 1. new mock MockSkCanvas
 *                  2. expect MockSkCanvas call onDrawRegion 3 times
 *                  3. new RSCPUOverdrawCanvasListener from MockSkCanvas
 *                  4. call RSCPUOverdrawCanvasListener's onDrawRect 3 times
 *                      - rect{  0,   0, 600, 600}
 *                      - rect{100, 100, 400, 400}
 *                      - rect{200, 200, 200, 200}
 */
HWTEST_F(RSCPUOverdrawCanvasListenerTest, Intersect, Function | SmallTest | Level2)
{
    PART("CaseDescription")
    {
        std::unique_ptr<MockSkCanvas> mockSkCanvas = nullptr;
        STEP("1. new mock MockSkCanvas")
        {
            mockSkCanvas = std::make_unique<MockSkCanvas>();
        }

        STEP("2. expect MockSkCanvas call onDrawRegion 3 times")
        {
            EXPECT_CALL(*mockSkCanvas, onDrawRegion(_, _)).Times(3);
        }

        std::unique_ptr<RSCPUOverdrawCanvasListener> rsOverdrawCanvasListener = nullptr;
        STEP("3. new RSCPUOverdrawCanvasListener from MockSkCanvas")
        {
            rsOverdrawCanvasListener = std::make_unique<RSCPUOverdrawCanvasListener>(*mockSkCanvas);
        }

        STEP("4. call RSCPUOverdrawCanvasListener's onDrawRect 3 times")
        {
            rsOverdrawCanvasListener->onDrawRRect(SkRRect::MakeOval(SkRect::MakeXYWH(0, 0, 600, 600)), {});
            rsOverdrawCanvasListener->onDrawRRect(SkRRect::MakeOval(SkRect::MakeXYWH(100, 100, 400, 400)), {});
            rsOverdrawCanvasListener->onDrawRRect(SkRRect::MakeOval(SkRect::MakeXYWH(200, 200, 200, 200)), {});
            rsOverdrawCanvasListener->Draw();
        }
    }
}

/*
 * Function: mock draw 3 regions that do not intersect each other
 * Type: Function
 * EnvConditions: N/A
 * CaseDescription: 1. new mock MockSkCanvas
 *                  2. expect MockSkCanvas call onDrawRegion once
 *                  3. new RSCPUOverdrawCanvasListener from MockSkCanvas
 *                  4. call RSCPUOverdrawCanvasListener's onDrawRect 3 times
 *                      - rect{  0,   0, 100, 100}
 *                      - rect{200, 200, 100, 100}
 *                      - rect{400, 400, 100, 100}
 */
HWTEST_F(RSCPUOverdrawCanvasListenerTest, NoIntersect, Function | SmallTest | Level2)
{
    PART("CaseDescription")
    {
        std::unique_ptr<MockSkCanvas> mockSkCanvas = nullptr;
        STEP("1. new mock MockSkCanvas")
        {
            mockSkCanvas = std::make_unique<MockSkCanvas>();
        }

        STEP("2. expect MockSkCanvas call onDrawRegion once")
        {
            EXPECT_CALL(*mockSkCanvas, onDrawRegion(_, _)).Times(1);
        }

        std::unique_ptr<RSCPUOverdrawCanvasListener> rsOverdrawCanvasListener = nullptr;
        STEP("3. new RSCPUOverdrawCanvasListener from MockSkCanvas")
        {
            rsOverdrawCanvasListener = std::make_unique<RSCPUOverdrawCanvasListener>(*mockSkCanvas);
        }

        STEP("4. call RSCPUOverdrawCanvasListener's onDrawRect 3 times")
        {
            rsOverdrawCanvasListener->onDrawRect(SkRect::MakeXYWH(0, 0, 100, 100), {});
            rsOverdrawCanvasListener->onDrawRect(SkRect::MakeXYWH(200, 200, 100, 100), {});
            rsOverdrawCanvasListener->onDrawRect(SkRect::MakeXYWH(400, 400, 100, 100), {});
            rsOverdrawCanvasListener->Draw();
        }
    }
}

/**
 * @tc.name: onDrawTextBlob001
 * @tc.desc: test
 * @tc.type:FUNC
 * @tc.require:
 */
HWTEST_F(RSCPUOverdrawCanvasListenerTest, onDrawTextBlob001, TestSize.Level1)
{
    MockSkCanvas canvas;
    SkScalar x = 0.1;
    SkScalar y = 0.1;
    SkPaint paint;
    RSCPUOverdrawCanvasListener listener(canvas);
    listener.onDrawTextBlob(nullptr, x, y, paint);
    string out = "Hello";
    sk_sp<SkTextBlob> SkTextBlob = SkTextBlob::MakeFromString(out.c_str(), SkFont(nullptr, 24.0f, 1.0f, 0.0f));
    listener.onDrawTextBlob(SkTextBlob.get(), x, y, paint);
}

/**
 * @tc.name: onDrawDrawable001
 * @tc.desc: test
 * @tc.type:FUNC
 * @tc.require:
 */
HWTEST_F(RSCPUOverdrawCanvasListenerTest, onDrawDrawable001, TestSize.Level1)
{
    MockSkCanvas canvas;
    RSCPUOverdrawCanvasListener listener(canvas);
    SkMatrix matrix = SkMatrix::I();
    listener.onDrawDrawable(nullptr, &matrix);
    const char data[] = { '0', '0' };
    sk_sp<SkDrawable> inSkDrawable = SkDrawable::Deserialize(data, sizeof(data));
    listener.onDrawDrawable(inSkDrawable.get(), nullptr);
    listener.onDrawDrawable(inSkDrawable.get(), &matrix);
}

/**
 * @tc.name: onDrawPicture001
 * @tc.desc: test
 * @tc.type:FUNC
 * @tc.require:
 */
HWTEST_F(RSCPUOverdrawCanvasListenerTest, onDrawPicture001, TestSize.Level1)
{
    MockSkCanvas canvas;
    RSCPUOverdrawCanvasListener listener(canvas);
    SkMatrix matrix = SkMatrix::I();
    SkPaint paint;
    listener.onDrawPicture(nullptr, &matrix, &paint);
    SkRect rect = SkRect::MakeXYWH(1.f, 1.f, 2.f, 3.f);
    sk_sp<SkPicture> skpicture = SkPicture::MakePlaceholder(rect);
    listener.onDrawPicture(skpicture.get(), nullptr, nullptr);
    listener.onDrawPicture(skpicture.get(), &matrix, &paint);
}

/**
 * @tc.name: onDrawPoints001
 * @tc.desc: test
 * @tc.type:FUNC
 * @tc.require:
 */
HWTEST_F(RSCPUOverdrawCanvasListenerTest, onDrawPoints001, TestSize.Level1)
{
    MockSkCanvas canvas;
    SkPaint paint;
    size_t count = 1;
    SkScalar x = 0.1;
    SkScalar y = 0.2;
    SkPoint point = SkPoint::Make(x, y);
    SkPoint pts[] = { point };
    RSCPUOverdrawCanvasListener listener(canvas);
    listener.onDrawPoints(SkCanvas::PointMode::kLines_PointMode, count, pts, paint);
}

/**
 * @tc.name: onDrawEdgeAAQuad001
 * @tc.desc: test
 * @tc.type:FUNC
 * @tc.require:
 */
HWTEST_F(RSCPUOverdrawCanvasListenerTest, onDrawEdgeAAQuad001, TestSize.Level1)
{
    MockSkCanvas canvas;
    SkRect rect;
    SkCanvas::QuadAAFlags aaFlags = SkCanvas::QuadAAFlags::kAll_QuadAAFlags;
    SkColor4f color;
    SkBlendMode mode = SkBlendMode::kClear;
    RSCPUOverdrawCanvasListener listener(canvas);
    listener.onDrawEdgeAAQuad(rect, {}, aaFlags, color, mode);
    SkScalar x = 0.1;
    SkScalar y = 0.2;
    SkPoint point = SkPoint::Make(x, y);
    SkPoint clip[4] = { point };
    listener.onDrawEdgeAAQuad(rect, clip, aaFlags, color, mode);
}
} // namespace Rosen
} // namespace OHOS
