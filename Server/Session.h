#pragma once

class SendData
{
public:
	WSAOVERLAPPED Over;
	WSABUF WsaBuf;
	MemoryStream SendMess;

	SendData(const MemoryStream& mess);
};

class Session
{
public:
	Session() = default;
	Session(int clientID, SOCKET s);

	void DoRecv();
	void DoSend(const MemoryStream& packet);

	MemoryStream& GetMess() { return mMess; }
	int GetClientID() const { return mClientID; }

private:
	WSAOVERLAPPED mOverlapped;
	WSABUF mWsaBuf[1];

	int mClientID = -1;
	SOCKET mSocket;
	MemoryStream mMess;
};