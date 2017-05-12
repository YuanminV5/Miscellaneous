#include <vrmsrc/util/System.h>

static std::string m_baseDirectoryPathString = "";

const std::string& getBaseDirectory()
{
	return m_baseDirectoryPathString;
}

