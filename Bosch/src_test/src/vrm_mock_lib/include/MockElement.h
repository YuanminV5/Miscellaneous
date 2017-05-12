#pragma once

#include <Config.h>
#include <gmock/gmock.h>
#include <string>

class DLLEXPORT MockElementList
{
public:
    MOCK_CONST_METHOD0(size, int());
};

class DLLEXPORT MockElement
{
public:
    MOCK_CONST_METHOD1(getText, bool(std::string&));
    MOCK_CONST_METHOD1(getText, bool(unsigned long&));
    MOCK_METHOD0(operatorBool, bool());
    MOCK_METHOD0(getChild, void());
    MOCK_METHOD1(getAttribute, bool(std::string&));
    MOCK_CONST_METHOD2(getAttribute, bool(const char*, std::string&));
    MOCK_CONST_METHOD2(getAttribute, bool(const char*, unsigned long&));
};