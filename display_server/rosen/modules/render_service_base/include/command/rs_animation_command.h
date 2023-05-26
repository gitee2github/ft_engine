/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#ifndef ROSEN_RENDER_SERVICE_BASE_COMMAND_RS_ANIMATION_COMMAND_H
#define ROSEN_RENDER_SERVICE_BASE_COMMAND_RS_ANIMATION_COMMAND_H

#include "animation/rs_render_animation.h"
#include "animation/rs_render_curve_animation.h"
#include "animation/rs_render_keyframe_animation.h"
#include "animation/rs_render_path_animation.h"
#include "animation/rs_render_spring_animation.h"
#include "animation/rs_render_transition.h"
#include "command/rs_command_templates.h"
#include "common/rs_macros.h"
#include "pipeline/rs_render_node.h"

namespace OHOS {
namespace Rosen {
enum RSAnimationCommandType : uint16_t {
    // curve animation
    ANIMATION_CREATE_CURVE,
    // keyframe animation
    ANIMATION_CREATE_KEYFRAME,
    // path animation
    ANIMATION_CREATE_PATH,
    // transition animation
    ANIMATION_CREATE_TRANSITION,
    // spring animation
    ANIMATION_CREATE_SPRING,

    // operations
    ANIMATION_START,
    ANIMATION_PAUSE,
    ANIMATION_RESUME,
    ANIMATION_FINISH,
    ANIMATION_REVERSE,
    ANIMATION_SET_FRACTION,

    // UI operation
    ANIMATION_FINISH_CALLBACK,
};

class RSB_EXPORT AnimationCommandHelper {
public:
    template<void (RSRenderAnimation::*OP)()>
    static void AnimOp(RSContext& context, NodeId nodeId, AnimationId animId)
    {
        auto node = context.GetNodeMap().GetRenderNode<RSRenderNode>(nodeId);
        if (node == nullptr) {
            return;
        }
        auto animation = node->GetAnimationManager().GetAnimation(animId);
        if (animation == nullptr) {
            return;
        }
        (*animation.*OP)();
    }
    template<void (RSRenderAnimation::*OP)()>
    static void AnimOpReg(RSContext& context, NodeId nodeId, AnimationId animId)
    {
        auto node = context.GetNodeMap().GetRenderNode<RSRenderNode>(nodeId);
        if (node == nullptr) {
            return;
        }
        auto animation = node->GetAnimationManager().GetAnimation(animId);
        if (animation == nullptr) {
            return;
        }
        (*animation.*OP)();
        // register node on animation start or resume
        context.RegisterAnimatingRenderNode(node);
    }
    template<typename T, void (RSRenderAnimation::*OP)(T)>
    static void AnimOp(RSContext& context, NodeId nodeId, AnimationId animId, T param)
    {
        auto node = context.GetNodeMap().GetRenderNode<RSRenderNode>(nodeId);
        if (node == nullptr) {
            return;
        }
        auto animation = node->GetAnimationManager().GetAnimation(animId);
        if (animation == nullptr) {
            return;
        }
        (*animation.*OP)(param);
    }
    static void CreateAnimation(
        RSContext& context, NodeId targetId, const std::shared_ptr<RSRenderAnimation>& animation);

    using FinishCallbackProcessor = void (*)(NodeId, AnimationId);
    static void AnimationFinishCallback(RSContext& context, NodeId targetId, AnimationId animId);
    static RSB_EXPORT void SetFinishCallbackProcessor(FinishCallbackProcessor processor);
};

// animation operation
ADD_COMMAND(RSAnimationStart,
    ARG(ANIMATION, ANIMATION_START, AnimationCommandHelper::AnimOpReg<&RSRenderAnimation::Start>, NodeId, AnimationId))
ADD_COMMAND(RSAnimationPause,
    ARG(ANIMATION, ANIMATION_PAUSE, AnimationCommandHelper::AnimOp<&RSRenderAnimation::Pause>, NodeId, AnimationId))
ADD_COMMAND(RSAnimationResume, ARG(ANIMATION, ANIMATION_RESUME,
                                   AnimationCommandHelper::AnimOpReg<&RSRenderAnimation::Resume>, NodeId, AnimationId))
ADD_COMMAND(RSAnimationFinish,
    ARG(ANIMATION, ANIMATION_FINISH, AnimationCommandHelper::AnimOp<&RSRenderAnimation::Finish>, NodeId, AnimationId))
ADD_COMMAND(RSAnimationReverse,
    ARG(ANIMATION, ANIMATION_REVERSE, AnimationCommandHelper::AnimOp<bool, &RSRenderAnimation::SetReversed>, NodeId,
        AnimationId, bool))
ADD_COMMAND(RSAnimationSetFraction,
    ARG(ANIMATION, ANIMATION_SET_FRACTION, AnimationCommandHelper::AnimOp<float, &RSRenderAnimation::SetFraction>,
        NodeId, AnimationId, float))

ADD_COMMAND(RSAnimationFinishCallback,
    ARG(ANIMATION, ANIMATION_FINISH_CALLBACK, AnimationCommandHelper::AnimationFinishCallback, NodeId, AnimationId))

// create curve animation
ADD_COMMAND(RSAnimationCreateCurve, ARG(ANIMATION, ANIMATION_CREATE_CURVE, AnimationCommandHelper::CreateAnimation,
    NodeId, std::shared_ptr<RSRenderCurveAnimation>))

// create keyframe animation
ADD_COMMAND(RSAnimationCreateKeyframe,ARG(ANIMATION, ANIMATION_CREATE_KEYFRAME,
    AnimationCommandHelper::CreateAnimation, NodeId, std::shared_ptr<RSRenderKeyframeAnimation>))

// create path animation
ADD_COMMAND(RSAnimationCreatePath, ARG(ANIMATION, ANIMATION_CREATE_PATH,
    AnimationCommandHelper::CreateAnimation, NodeId, std::shared_ptr<RSRenderPathAnimation>))

// create transition animation
ADD_COMMAND(RSAnimationCreateTransition, ARG(ANIMATION, ANIMATION_CREATE_TRANSITION,
    AnimationCommandHelper::CreateAnimation, NodeId, std::shared_ptr<RSRenderTransition>))

// create spring animation
ADD_COMMAND(RSAnimationCreateSpring,ARG(ANIMATION, ANIMATION_CREATE_SPRING,
    AnimationCommandHelper::CreateAnimation, NodeId, std::shared_ptr<RSRenderSpringAnimation>))
} // namespace Rosen
} // namespace OHOS

#endif // ROSEN_RENDER_SERVICE_BASE_COMMAND_RS_ANIMATION_COMMAND_H
