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

#include "scoped_bytrace.h"

#include <hitrace_meter.h>
#include <hilog/log.h>

ScopedBytrace::ScopedBytrace(const std::string &proc) : proc_(proc)
{
    StartTrace(HITRACE_TAG_GRAPHIC_AGP, proc_);
    isEnd = false;
}

ScopedBytrace::~ScopedBytrace()
{
    if (isEnd == false) {
        FinishTrace(HITRACE_TAG_GRAPHIC_AGP);
    }
}

void ScopedBytrace::End()
{
    if (isEnd == false) {
        FinishTrace(HITRACE_TAG_GRAPHIC_AGP);
    }
}
