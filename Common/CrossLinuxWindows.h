#pragma once
#include <string>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define BUILD_WIN
const std::string PATH_SP = "\\";
#else
#define BUILD_LINUX
const std::string PATH_SP = "/";
#endif



