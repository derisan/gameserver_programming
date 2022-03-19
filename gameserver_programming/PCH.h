#pragma once

#pragma warning(disable:4819)
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "fmt")
#pragma comment(lib, "SDL2")
#pragma comment(lib, "SDL2_image")
#pragma comment(lib, "SDL2_ttf")
#pragma comment(lib, "brotlicommon")
#pragma comment(lib, "brotlidec")
#pragma comment(lib, "bz2")
#pragma comment(lib, "freetype")
#pragma comment(lib, "libpng16")
#pragma comment(lib, "zlib")
#pragma comment(lib, "spdlog")

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVER_PORT (9000)

#include "GSMath.h"
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#undef main
#include <entt/entt.hpp>

#include <string>
#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <deque>

using std::string;
using std::wstring;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::deque;
using std::array;
using std::unordered_map;

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

#include "Log.h"