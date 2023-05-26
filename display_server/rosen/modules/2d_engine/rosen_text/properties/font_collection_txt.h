/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.. All rights reserved.
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

#ifndef ROSEN_TEXT_PROPERTIES_FONT_COLLECTION_TXT_H_
#define ROSEN_TEXT_PROPERTIES_FONT_COLLECTION_TXT_H_

#include <memory>
#include <vector>

#include "rosen_text/properties/font_collection_txt_base.h"
#include "txt/asset_font_manager.h"
#include "txt/font_collection.h"

namespace rosen {
class FontCollectionTxt : public FontCollectionTxtBase {
public:
    explicit FontCollectionTxt(bool createWithICU = true);

    ~FontCollectionTxt() override;
    std::shared_ptr<txt::FontCollection> GetFontCollection() const;

    void RegisterTestFonts() override;

    void LoadFontFromList(const uint8_t* font_data,
                          int length,
                          std::string family_name) override;
    void LoadSystemFont() override;
private:
    std::shared_ptr<txt::FontCollection> txtCollection;
    sk_sp<txt::DynamicFontManager> dynamicFontManager;
    FontCollectionTxt(const FontCollectionTxt&) = delete;
    FontCollectionTxt& operator=(const FontCollectionTxt&) = delete;
};
} // namespace rosen
#endif // ROSEN_TEXT_PROPERTIES_FONT_COLLECTION_TXT_H_
