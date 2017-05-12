#pragma once 

#include <Config.h>
#include <string>

namespace vcslib
{
    class DLLEXPORT MacAddress
    {
    public:
        bool valid() const;
        std::string toString() const;
    };

    struct DLLEXPORT ParsedUrl
    {
        ParsedUrl(const std::string& url);
        std::string m_address;
    };
}