#pragma once

#include <memory>

#if EXPORT_MOCKS
	#define DLLEXPORT __declspec(dllexport)
#else
	#define DLLEXPORT __declspec(dllimport)
#endif
