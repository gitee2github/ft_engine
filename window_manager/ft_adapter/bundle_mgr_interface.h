/*
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
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

#ifndef BUNDLEMGR_BUNDLE_MGR_INTERFACE_H
#define BUNDLEMGR_BUNDLE_MGR_INTERFACE_H

#include <string>

#include <iremote_broker.h>

namespace OHOS {
namespace AppExecFwk {
class IBundleMgr : public IRemoteBroker {
public:
    DECLARE_INTERFACE_DESCRIPTOR(u"ohos.appexecfwk.BundleMgr");

    bool GetBundleNameForUid(const int uid, std::string &bundleName)
    {
        return false;
    }

    bool CheckIsSystemAppByUid(const int uid)
    {
        return false;
    }
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // BUNDLEMGR_BUNDLE_MGR_INTERFACE_H
