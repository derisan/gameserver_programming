#include "ServerPCH.h"
#include "Session.h"

#include "ServerGlobal.h"

SendData::SendData(const MemoryStream& mess)
	: SendMess(mess)
{
	ZeroMemory(&Over, sizeof(Over));
	WsaBuf.buf = reinterpret_cast<char*>(&SendMess);
	WsaBuf.len = sizeof(SendMess);
}

Session::Session(int clientID, SOCKET s)
	: mClientID(clientID)
	, mSocket(s)
{
	mWsaBuf[0].buf = reinterpret_cast<char*>(&mMess);
	mWsaBuf[0].len = sizeof(mMess);
	gOverToID[&mOverlapped] = clientID;
}

void Session::DoRecv()
{
	DWORD flag = 0;
	ZeroMemory(&mOverlapped, sizeof(mOverlapped));
	WSARecv(mSocket, mWsaBuf, 1, 0, &flag, &mOverlapped, RecvCallback);
}

void Session::DoSend(const MemoryStream& packet)
{
	SendData* sendData = new SendData(packet);
	WSASend(mSocket, &(sendData->WsaBuf), 1, 0, 0, &sendData->Over, SendCallback);
}
