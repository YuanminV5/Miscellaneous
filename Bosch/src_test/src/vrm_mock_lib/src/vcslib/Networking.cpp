#include <vcslib/Networking.h>

namespace vcslib
{

    std::string MacAddress::toString() const
    {
        return "";
    }

    bool MacAddress::valid() const
    {
        return true;
    }

    ParsedUrl::ParsedUrl(const std::string& url)
    {
        m_address = url;
    }
}