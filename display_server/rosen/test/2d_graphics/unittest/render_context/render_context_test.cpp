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

#include <gtest/gtest.h>
#include <hilog/log.h>

#include "render_context.h"

using namespace testing::ext;

namespace OHOS::Rosen {
class RenderContextTest : public testing::Test {
public:
    static constexpr HiviewDFX::HiLogLabel LOG_LABEL = { LOG_CORE, 0, "RenderContextTest" };
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
};

void RenderContextTest::SetUpTestCase() {}
void RenderContextTest::TearDownTestCase() {}
void RenderContextTest::SetUp() {}
void RenderContextTest::TearDown() {}

/**
 * @tc.name: CreateEGLSurfaceTest001
 * @tc.desc: Verify the CreateEGLSurfaceTest001 of RenderContextTest
 * @tc.type: FUNC
 */
HWTEST_F(RenderContextTest, CreateEGLSurfaceTest001, Function | SmallTest | Level2)
{
#ifdef ACE_ENABLE_GL
    // start CreateEGLSurfaceTest001 test
    RenderContext renderContext;
    EGLSurface eglSurface = renderContext.CreateEGLSurface(nullptr);
    EXPECT_EQ(eglSurface, EGL_NO_SURFACE);
#endif
}

/**
 * @tc.name: CreateEGLSurfaceTest002
 * @tc.desc: Verify the CreateEGLSurfaceTest002 of RenderContextTest
 * @tc.type: FUNC
 */
HWTEST_F(RenderContextTest, CreateEGLSurfaceTest002, Function | SmallTest | Level2)
{
#ifdef ACE_ENABLE_GL
    // start CreateEGLSurfaceTest002 test
    RenderContext renderContext;
    renderContext.InitializeEglContext();
    EGLSurface eglSurface = renderContext.CreateEGLSurface(nullptr);
    EXPECT_EQ(eglSurface, EGL_NO_SURFACE);
#endif
}

/**
 * @tc.name: SetUpGrContextTest
 * @tc.desc: Verify the SetUpGrContextTest of RenderContextTest
 * @tc.type: FUNC
 */
HWTEST_F(RenderContextTest, SetUpGrContextTest, Function | SmallTest | Level2)
{
#ifdef ACE_ENABLE_GL
    // start SetUpGrContextTest test
    RenderContext renderContext;
    bool grContext = renderContext.SetUpGrContext();
    EXPECT_EQ(grContext, false);
#endif
}

/**
 * @tc.name: AcquireSurfaceTest
 * @tc.desc: Verify the AcquireSurfaceTest of RenderContextTest
 * @tc.type: FUNC
 */
HWTEST_F(RenderContextTest, AcquireSurfaceTest, Function | SmallTest | Level2)
{
#ifdef ACE_ENABLE_GL
    // start AcquireSurfaceTest test
    RenderContext renderContext;
    auto surface = renderContext.AcquireSurface(0, 0);
    EXPECT_TRUE(surface == nullptr);
#endif
}

/**
 * @tc.name: QueryEglBufferAgeTest001
 * @tc.desc: Verify the QueryEglBufferAgeTest001 of RenderContextTest
 * @tc.type: FUNC
 */
HWTEST_F(RenderContextTest, QueryEglBufferAgeTest001, Function | SmallTest | Level2)
{
#ifdef ACE_ENABLE_GL
    // start QueryEglBufferAgeTest001 test
    RenderContext renderContext;
    EGLint bufferAge = renderContext.QueryEglBufferAge();
    EXPECT_EQ(bufferAge, EGL_UNKNOWN);
#endif
}

/**
 * @tc.name: QueryEglBufferAgeTest002
 * @tc.desc: Verify the QueryEglBufferAgeTest002 of RenderContextTest
 * @tc.type: FUNC
 */
HWTEST_F(RenderContextTest, QueryEglBufferAgeTest002, Function | SmallTest | Level2)
{
#ifdef ACE_ENABLE_GL
    // start QueryEglBufferAgeTest002 test
    RenderContext renderContext;
    EGLint bufferAge = renderContext.QueryEglBufferAge();
    EXPECT_EQ(bufferAge, EGL_UNKNOWN);
#endif
}
} // namespace OHOS::Rosen