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
#ifndef RENDER_SERVICE_CLIENT_CORE_PIPELINE_RS_RENDER_THREAD_H
#define RENDER_SERVICE_CLIENT_CORE_PIPELINE_RS_RENDER_THREAD_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>
#include <event_handler.h>

#include "common/rs_thread_handler.h"
#include "common/rs_thread_looper.h"
#include "jank_detector/rs_jank_detector.h"
#include "pipeline/rs_canvas_render_node.h"
#include "pipeline/rs_render_thread_visitor.h"
#include "platform/drawing/rs_vsync_client.h"
#include "render_context/render_context.h"
#include "transaction/rs_transaction_proxy.h"
#include "vsync_receiver.h"

namespace OHOS {
namespace Rosen {
class HighContrastObserver;
class RSRenderThread final {
public:
    static RSRenderThread& Instance();

    void Start();
    void Stop();

    void Detach(NodeId id);
    void RecvTransactionData(std::unique_ptr<RSTransactionData>& transactionData);
    void RequestNextVSync();
    void PostTask(RSTaskMessage::RSTask task);
    void PostPreTask();
    void UpdateWindowStatus(bool active);

    int32_t GetTid();

    std::string DumpRenderTree() const;

    RenderContext* GetRenderContext()
    {
        return renderContext_;
    }

    RSContext& GetContext()
    {
        return *context_;
    }
    const RSContext& GetContext() const
    {
        return *context_;
    }
    uint64_t GetUITimestamp() const
    {
        return uiTimestamp_;
    }
    void SetHighContrast(bool enabled)
    {
        isHighContrastEnabled_  = enabled;
    }
    bool isHighContrastEnabled() const
    {
        return isHighContrastEnabled_;
    }
    void UpdateRenderMode(bool needRender);
    void NotifyClearBufferCache();

    void SetCacheDir(const std::string& filePath)
    {
        cacheDir_ = filePath;
    }

    // If disabled partial render, rt forces to render whole frame
    void SetRTRenderForced(bool isRenderForced)
    {
        if ((isRTRenderForced_ != isRenderForced)) {
            ROSEN_LOGD("RSRenderThread::SetRenderForced %d -> %d", isRTRenderForced_, isRenderForced);
            isRTRenderForced_ = isRenderForced;
        }
    }

private:
    RSRenderThread();
    ~RSRenderThread();

    RSRenderThread(const RSRenderThread&) = delete;
    RSRenderThread(const RSRenderThread&&) = delete;
    RSRenderThread& operator=(const RSRenderThread&) = delete;
    RSRenderThread& operator=(const RSRenderThread&&) = delete;

    void RenderLoop();
    void CreateAndInitRenderContextIfNeed();

    void OnVsync(uint64_t timestamp);
    void ProcessCommands();
    void Animate(uint64_t timestamp);
    void Render();
    void SendCommands();

    void UpdateSurfaceNodeParentInRS();
    void ClearBufferCache();
    void MarkNeedUpdateSurfaceNode();
    std::atomic_bool running_ = false;
    std::atomic_bool hasSkipVsync_ = false;
    bool needRender_ = true;
    std::atomic_int activeWindowCnt_ = 0;
    std::unique_ptr<std::thread> thread_ = nullptr;
    std::shared_ptr<AppExecFwk::EventRunner> runner_ = nullptr;
    std::shared_ptr<AppExecFwk::EventHandler> handler_ = nullptr;
    std::shared_ptr<VSyncReceiver> receiver_ = nullptr;
    RSTaskMessage::RSTask preTask_ = nullptr;
    RSTaskMessage::RSTask mainFunc_;

    std::mutex mutex_;
    std::mutex cmdMutex_;
    std::vector<std::unique_ptr<RSTransactionData>> cmds_;
    bool hasRunningAnimation_ = false;
    std::shared_ptr<RSRenderThreadVisitor> visitor_;

    uint64_t timestamp_ = 0;
    uint64_t prevTimestamp_ = 0;
    uint64_t lastAnimateTimestamp_ = 0;
    int32_t tid_ = -1;
    uint64_t mValue = 0;

    uint64_t uiTimestamp_ = 0;
    uint64_t commandTimestamp_ = 0;

    // for jank frame detector
    std::shared_ptr<RSJankDetector> jankDetector_;

    std::shared_ptr<RSContext> context_;

    RenderContext* renderContext_ = nullptr;
    std::shared_ptr<HighContrastObserver> highContrastObserver_;
    std::atomic_bool isHighContrastEnabled_ = false;

    std::string cacheDir_;
    bool isRTRenderForced_ = false;
};
} // namespace Rosen
} // namespace OHOS

#endif // RENDER_SERVICE_CLIENT_CORE_PIPELINE_RS_RENDER_THREAD_H
