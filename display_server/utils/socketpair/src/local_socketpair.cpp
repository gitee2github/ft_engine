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

#include "local_socketpair.h"
#include "hilog/log.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <scoped_bytrace.h>

namespace OHOS {
using namespace OHOS::HiviewDFX;

namespace {
constexpr HiLogLabel LABEL = { LOG_CORE, 0xD001400, "LocalSocketPair" };
constexpr int32_t SOCKET_PAIR_SIZE = 2;
constexpr int32_t INVALID_FD = -1;
constexpr int32_t ERRNO_EAGAIN = -1;
constexpr int32_t ERRNO_OTHER = -2;
}  // namespace

LocalSocketPair::LocalSocketPair()
    : sendFd_(INVALID_FD), receiveFd_(INVALID_FD)
{
    HiLog::Debug(LABEL, "%{public}s sendFd: %{public}d", __func__, sendFd_);
}

LocalSocketPair::~LocalSocketPair()
{
    CloseFd(sendFd_);
    CloseFd(receiveFd_);
}

int32_t LocalSocketPair::CreateChannel(size_t sendSize, size_t receiveSize)
{
    if ((sendFd_ != INVALID_FD) || (receiveFd_ != INVALID_FD)) {
        HiLog::Debug(LABEL, "%{public}s already create socketpair", __func__);
        return 0;
    }

    int32_t socketPair[SOCKET_PAIR_SIZE] = { 0 };
    if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, socketPair) != 0) {
        ScopedBytrace func("Create socketpair failed, errno = " + std::to_string(errno));
        HiLog::Error(LABEL, "%{public}s create socketpair failed", __func__);
        return -1;
    }
    if (socketPair[0] == 0 || socketPair[1] == 0) {
        int32_t unusedFds[SOCKET_PAIR_SIZE] = {socketPair[0], socketPair[1]};
        int32_t err = socketpair(AF_UNIX, SOCK_SEQPACKET, 0, socketPair);
        CloseFd(unusedFds[0]);
        CloseFd(unusedFds[1]);
        if (err != 0) {
            ScopedBytrace func2("Create socketpair failed for the second time, errno = " + std::to_string(errno));
            HiLog::Error(LABEL, "%{public}s create socketpair failed", __func__);
            return -1;
        }
    }

    // set socket attr
    for (int i = 0; i < SOCKET_PAIR_SIZE; ++i) {
        int32_t ret = setsockopt(socketPair[i], SOL_SOCKET, SO_SNDBUF, &sendSize, sizeof(sendSize));
        if (ret != 0) {
            HiLog::Error(LABEL, "%{public}s setsockopt socketpair %{public}d sendbuffer size failed", __func__, i);
            return -1;
        }
        ret = setsockopt(socketPair[i], SOL_SOCKET, SO_RCVBUF, &receiveSize, sizeof(receiveSize));
        if (ret != 0) {
            HiLog::Error(LABEL, "%{public}s setsockopt socketpair %{public}d receivebuffer size failed", __func__, i);
            return -1;
        }
        ret = fcntl(socketPair[i], F_SETFL, O_NONBLOCK);
        if (ret != 0) {
            HiLog::Error(LABEL, "%{public}s fcntl socketpair %{public}d nonblock failed", __func__, i);
            return -1;
        }
    }
    sendFd_ = socketPair[0];
    receiveFd_ = socketPair[1];
    HiLog::Debug(LABEL, "%{public}s create socketpair success, receiveFd_ : %{public}d, sendFd_ : %{public}d", __func__,
                 receiveFd_, sendFd_);

    return 0;
}

int32_t LocalSocketPair::SendData(const void *vaddr, size_t size)
{
    if (vaddr == nullptr || sendFd_ < 0) {
        HiLog::Error(LABEL, "%{public}s failed, param is invalid", __func__);
        return -1;
    }
    ssize_t length;
    do {
        length = send(sendFd_, vaddr, size, MSG_DONTWAIT | MSG_NOSIGNAL);
    } while (errno == EINTR);
    if (length < 0) {
        ScopedBytrace func("SocketPair SendData failed, errno = " + std::to_string(errno) +
                            ", sendFd_ = " + std::to_string(sendFd_) + ", receiveFd_ = " + std::to_string(receiveFd_) +
                            ", length = " + std::to_string(length));
        HiLog::Debug(LABEL, "%{public}s send failed:%{public}d, length = %{public}d", __func__, errno, (int32_t)length);
        if (errno == EAGAIN) {
            return ERRNO_EAGAIN;
        } else {
            return ERRNO_OTHER;
        }
    }
    return length;
}

int32_t LocalSocketPair::ReceiveData(void *vaddr, size_t size)
{
    if (vaddr == nullptr || (receiveFd_ < 0)) {
        HiLog::Error(LABEL, "%{public}s failed, vaddr is null or receiveFd_ invalid", __func__);
        return -1;
    }
    ssize_t length;
    do {
        length = recv(receiveFd_, vaddr, size, MSG_DONTWAIT);
    } while (errno == EINTR);
    if (length < 0) {
        ScopedBytrace func("SocketPair ReceiveData failed errno = " + std::to_string(errno) +
                            ", sendFd_ = " + std::to_string(sendFd_) + ", receiveFd_ = " + std::to_string(receiveFd_) +
                            ", length = " + std::to_string(length));
        return -1;
    }
    return length;
}

// internal interface
int32_t LocalSocketPair::SendFdToBinder(MessageParcel &data, int32_t &fd)
{
    if (fd < 0) {
        return -1;
    }
    // need dup???
    bool result = data.WriteFileDescriptor(fd);
    if (!result) {
        return -1;
    }
    return 0;
}

int32_t LocalSocketPair::SendToBinder(MessageParcel &data)
{
    return SendFdToBinder(data, sendFd_);
}

int32_t LocalSocketPair::ReceiveToBinder(MessageParcel &data)
{
    return SendFdToBinder(data, receiveFd_);
}

// internal interface
void LocalSocketPair::CloseFd(int32_t &fd)
{
    if (fd != INVALID_FD) {
        close(fd);
        fd = INVALID_FD;
    }
}
}

