#include <vcslib/Guid.h>

MockGuid* Guid::s_mock = nullptr;

std::string Guid::toString()
{
    return s_mock->toString();
}