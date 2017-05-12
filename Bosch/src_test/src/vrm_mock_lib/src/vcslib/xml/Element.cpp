#include <vcslib/xml/Element.h>

namespace vcslib
{
    namespace xml
    {
        MockElementList* ElementList::s_mock = nullptr;
        MockElement* Element::s_mock = nullptr;

        ElementList::ElementList(const char* xpath)
        {
        }

        int ElementList::size() const
        {
            return s_mock->size();
        }

        Element ElementList::getElement(unsigned int index)
        {
            return Element();
        }

        Element::Element()
        {
        }

        Element::Element(const char* xpath)
        {
        }

        Element::Element(ElementType type)
        {
        }

        bool Element::navigateToChild(const char* tagName, const char* attributeName, const char* attributeValue, StringMatch match)
        {
            return false;
        }

        bool Element::navigateToChild(const char* tagName, const char* attributeName, unsigned int attributeValue)
        {
            return false;
        }

        Element Element::getChild(const char* tagName, const char* attributeName, const char* attributeValue, StringMatch match) const
        {
            return Element();
        }

        Element Element::getChild(const char* tagName, const char* attributeName, unsigned int attributeValue) const
        {
            return Element();
        }

        Element Element::getParent() const
        {
            return Element();
        }

        bool Element::getAttributeName(int index, std::string& name) const
        {
            return false;
        }

        bool Element::getAttribute(const char* attribute, std::string& value) const
        {
            return s_mock->getAttribute(attribute, value);
        }

        bool Element::getAttribute(const std::string& attribute, std::string& value) const
        {
            return s_mock->getAttribute(value);
        }

        bool Element::getAttribute(const char* attribute, long& value) const
        {
            return false;
        }

        bool Element::getAttribute(const std::string& attribute, long& value) const
        {
            return false;
        }

        bool Element::getAttribute(const char* attribute, unsigned long& value) const
        {
            return s_mock->getAttribute(attribute, value);
        }

        bool Element::getAttribute(const std::string& attribute, unsigned long& value) const
        {
            return false;
        }

        bool Element::getText(std::string& text) const
        {
            return s_mock->getText(text);
        }

        bool Element::getText(long& value) const
        {
            return false;
        }

        bool Element::getText(unsigned long& value) const
        {
            return s_mock->getText(value);
        }

        bool Element::getText(std::wstring& text) const
        {
            return false;
        }

        Element::operator bool() const
        {
            return s_mock->operatorBool();
        }
    }
}