#pragma once

#include "TCPSocket.h"

class SocketUtil
{
public:
	static void Init();
	static void Shutdown();
	static void ReportError(const wstring& desc);
	static void ReportError(const wstring& desc, int error);
	static TCPSocketPtr CreateTCPSocket();
};