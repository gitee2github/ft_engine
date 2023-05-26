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

#include "gtest/gtest.h"
#include "pipeline/rs_processor_factory.h"
#include "screen_manager/rs_screen_manager.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS::Rosen {
class RSProcessorFactoryTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
};

void RSProcessorFactoryTest::SetUpTestCase() {}
void RSProcessorFactoryTest::TearDownTestCase() {}
void RSProcessorFactoryTest::SetUp() {}
void RSProcessorFactoryTest::TearDown() {}

/**
 * @tc.name: CreateAndDestroy001
 * @tc.desc:
 * @tc.type:
 * @tc.require:
 * @tc.author:
 */
HWTEST_F(RSProcessorFactoryTest, CreateAndDestroy001, TestSize.Level1)
{
    // The using of RSProcessorFactory constructor is not suggested, but allowed.
    // The using of RSProcessorFactory destructor is not suggested, but allowed.
    // Use its static function by :: first.
    RSProcessorFactory f;
    f.CreateProcessor(RSDisplayRenderNode::CompositeType::HARDWARE_COMPOSITE);
}

/**
 * @tc.name: CreateProcessor001
 * @tc.desc:
 * @tc.type:
 * @tc.require:
 * @tc.author:
 */
HWTEST_F(RSProcessorFactoryTest, CreateProcessor001, TestSize.Level1)
{
    // The way to create shared point of RSSoftwareProcessor.
    auto p = RSProcessorFactory::CreateProcessor(RSDisplayRenderNode::CompositeType::HARDWARE_COMPOSITE);
    EXPECT_FALSE(nullptr == p);
}

/**
 * @tc.name: CreateProcessor002
 * @tc.desc:
 * @tc.type:
 * @tc.require:
 * @tc.author:
 */
HWTEST_F(RSProcessorFactoryTest, CreateProcessor002, TestSize.Level1)
{
    // The way to create shared point of RSHardwareProcessor.
    auto p = RSProcessorFactory::CreateProcessor(RSDisplayRenderNode::CompositeType::SOFTWARE_COMPOSITE);
    EXPECT_FALSE(nullptr == p);
}

/**
 * @tc.name: CreateProcessor003
 * @tc.desc:
 * @tc.type:
 * @tc.require:
 * @tc.author:
 */
HWTEST_F(RSProcessorFactoryTest, CreateProcessor003, TestSize.Level1)
{
    auto p = RSProcessorFactory::CreateProcessor(RSDisplayRenderNode::CompositeType::UNI_RENDER_COMPOSITE);
    EXPECT_TRUE(nullptr != p);
}
} // namespace OHOS::Rosen
