#pragma once

#include <Config.h>
#include <string>

namespace vcslib
{
	DLLEXPORT std::string getExecutablePath();

	class DLLEXPORT FileSystemNode
	{
	public:
		FileSystemNode();
		FileSystemNode(const std::string& path);
		FileSystemNode(const char* path);
		bool exists() const { return true;}
		std::string getExtension() const { return std::string(); }
		bool navigateToParent();
		const std::string getPath() const { return std::string(); }
	};

	class DLLEXPORT DirectoryIterator
	{
	public:
		DirectoryIterator(const std::string& directory);
		DirectoryIterator(const char* directory);
		DirectoryIterator(const FileSystemNode& directory);
		bool next();
		FileSystemNode getCurrentNode() const;
		std::string getCurrentPath() const;
	};
}