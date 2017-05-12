#pragma once

#include <Config.h>
#include <vcslib/HostAddress.h>

#include <string>
#include <MockUserId.h>

/** 
 * This is the wrapper class for UserId.
 * Q: Why need wrapper for UserId?
 * A: UserId is used as return type of RmsSession::getSource. If this class is defined as mock class, 
 * copy constructor will be deleted and thus result in compiling error.
 */
class DLLEXPORT UserId
{
public:
    void init(const vcslib::HostAddress& address, const std::string& username);
    const std::string& getUser() const;
    const vcslib::HostAddress& getHostAddress() const;

    static MockUserId* s_mock;
};