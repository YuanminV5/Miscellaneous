#include <vcslib\Md5.h>

#include <string.h>
#include <stdio.h>

namespace vcslib
{
    MockMd5* Md5::s_mock = nullptr;

    Md5::Md5(const char* plainText)
    {
    }

    const char* Md5::digest()
    {
        return s_mock->digest();
    }
}