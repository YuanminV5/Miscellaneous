#include <vrmsrc/util/UserId.h>

MockUserId* UserId::s_mock = nullptr;

void UserId::init(const vcslib::HostAddress& address, const std::string& username)
{
    s_mock->init(address, username);
}

const std::string& UserId::getUser() const
{
    return s_mock->getUser();
}

const vcslib::HostAddress& UserId::getHostAddress() const
{
    return s_mock->getHostAddress();
}