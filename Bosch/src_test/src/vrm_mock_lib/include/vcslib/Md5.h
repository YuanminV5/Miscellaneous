#pragma once

#include <Config.h>
#include <MockMd5.h>

namespace vcslib
{
	class DLLEXPORT Md5
	{
	public:
		Md5(const char* plainText);
		const char* digest();

        static MockMd5* s_mock;
	};
} 
