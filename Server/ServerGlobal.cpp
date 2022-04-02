#include "ServerPCH.h"
#include "ServerGlobal.h"

#include "Server.h"

Server* gServer = nullptr;
unordered_map<int, Session> gIdToSession;
unordered_map<WSAOVERLAPPED*, int> gOverToID;
