#include <vcslib/FileSystem.h>

namespace vcslib
{
	std::string getExecutablePath()
	{
		return std::string();
	}

	FileSystemNode::FileSystemNode()
	{
	}

	FileSystemNode::FileSystemNode(const std::string& path)
	{
	}

	FileSystemNode::FileSystemNode(const char* path)
	{
	}

	bool FileSystemNode::navigateToParent()
	{
		return true;
	}

	DirectoryIterator::DirectoryIterator(const std::string& directory)
	{
	}

	DirectoryIterator::DirectoryIterator(const char* directory)
	{
	}

	DirectoryIterator::DirectoryIterator(const FileSystemNode& directory)
	{
	}


	bool DirectoryIterator::next()
	{
		return true;
	}

	FileSystemNode DirectoryIterator::getCurrentNode() const
	{
		return FileSystemNode();
	}

	std::string DirectoryIterator::getCurrentPath() const
	{
		return std::string();
	}

}
