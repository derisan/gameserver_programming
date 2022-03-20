#pragma once

#include <spdlog/spdlog.h>

#define GS_LOG(...) spdlog::info(__VA_ARGS__)

#ifdef _DEBUG
#define GS_ASSERT(x, ...) {if(!x) { GS_LOG("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define GS_ASSERT(x, ...)
#endif