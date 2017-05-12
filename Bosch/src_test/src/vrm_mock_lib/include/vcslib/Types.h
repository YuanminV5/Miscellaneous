#ifndef VCSLIB_TYPES_H
#define VCSLIB_TYPES_H

#include <string>

typedef char int8;
typedef short int16;
typedef int int32;


typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;


#ifdef _WIN32
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else 
typedef long long int64;
typedef unsigned long long uint64;
#endif

// The following three macros are used to create filename:line as string
#define VCSLIB_STRINGIFY_LINE(x) #x
#define VCSLIB_LINE_TO_STRING(x) VCSLIB_STRINGIFY_LINE(x)
#define __FILELINE__ __FILE__ ":" VCSLIB_LINE_TO_STRING(__LINE__)


#ifdef VCSLIB_EXTENDED_MEMORY_LOGGING
#	define vcs_new new(__FILELINE__)
void* operator new(std::size_t sz, const char* file);
void* operator new[](std::size_t sz, const char* file);
void operator delete(void* ptr, const char*);
void operator delete[](void* ptr, const char*);
#else
#	define vcs_new new
#endif

#endif