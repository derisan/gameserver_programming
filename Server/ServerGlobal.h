#pragma once

#include "Session.h"

extern class Server* gServer;
extern unordered_map<int, Session> gIdToSession;
extern unordered_map<WSAOVERLAPPED*, int> gOverToID;
extern void CALLBACK RecvCallback(DWORD err, DWORD numBytes, LPWSAOVERLAPPED over, DWORD flags);
extern void CALLBACK SendCallback(DWORD err, DWORD numBytes, LPWSAOVERLAPPED over, DWORD flags);
