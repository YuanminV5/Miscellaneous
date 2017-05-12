#pragma once

#include <Config.h>
#include <MockElement.h>
#include <string>

namespace vcslib
{
    namespace xml
    {
        class Element;

        enum ElementType
        {
            RootElement
        };

        enum StringMatch
        {
            MatchComplete,
            MatchStart,
            MatchEnd
        };

        class DLLEXPORT ElementList
        {
        public:
            ElementList(const char* xpath);

            int size() const;

            Element getElement(unsigned int index);

            static MockElementList* s_mock;
        };

        class DLLEXPORT Element 
        {
        public:
            Element();
            Element(const char* xpath);
            Element(ElementType type);

            bool navigateToChild(const char* tagName, 
                const char* attributeName = 0, 
                const char* attributeValue = 0, 
                StringMatch match = MatchComplete);

            bool navigateToChild(const char* tagName, 
                const char* attributeName, 
                unsigned int attributeValue);

            Element getChild(const char* tagName, 
                const char* attributeName = 0, 
                const char* attributeValue = 0, 
                StringMatch match = MatchComplete) const;

            Element getChild(const char* tagName, 
                const char* attributeName,
                unsigned int attributeValue) const;

            Element getParent() const;

            bool getAttributeName(int index, std::string& name) const;
            bool getAttribute(const char* attribute, std::string& value) const;
            bool getAttribute(const std::string& attribute, std::string& value) const;
            bool getAttribute(const char* attribute, long& value) const;
            bool getAttribute(const std::string& attribute, long& value) const;
            bool getAttribute(const char* attribute, unsigned long& value) const;
            bool getAttribute(const std::string& attribute, unsigned long& value) const;

            bool getText(std::string& text) const;
            bool getText(long& value) const;
            bool getText(unsigned long& value) const;
            bool getText(std::wstring& text) const;

            operator bool() const;

            static MockElement* s_mock;
        };
    }
}
