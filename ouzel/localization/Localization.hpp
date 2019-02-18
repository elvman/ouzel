// Copyright 2015-2019 Elviss Strazdins. All rights reserved.

#ifndef OUZEL_LOCALIZATION_LOCALIZATION_HPP
#define OUZEL_LOCALIZATION_LOCALIZATION_HPP

#include <cstdint>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "Language.hpp"

namespace ouzel
{
    class Localization final
    {
    public:
        void addLanguage(const std::string& name, const std::vector<uint8_t>& data);
        void setLanguage(const std::string& language);
        std::string getString(const std::string& str) const;

    private:
        std::map<std::string, std::unique_ptr<Language>> languages;
        Language* currentLanguage = nullptr;
    };
}

#endif // OUZEL_LOCALIZATION_LOCALIZATION_HPP
