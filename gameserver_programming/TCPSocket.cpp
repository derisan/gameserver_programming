#include "PCH.h"
#include "TCPSocket.h"

#include "SocketUtil.h"

TCPSocket::~TCPSocket()
{
	closesocket(mSocket);
}

int TCPSocket::Bind(const SocketAddress& addr)
{
	int error = bind(mSocket, &addr.mSockAddr, addr.GetSize());
	return NO_ERROR;
}

int TCPSocket::Listen(int backlog /*= SOMAXCONN*/)
{
	int error = listen(mSocket, backlog);
	return NO_ERROR;
}

int TCPSocket::Connect(const SocketAddress& addr)
{
	int error = connect(mSocket, &addr.mSockAddr, addr.GetSize());
	return NO_ERROR;
}

TCPSocketPtr TCPSocket::Accept(SocketAddress* outAddr)
{
	int addrlen = outAddr->GetSize();

	SOCKET newSock = accept(mSocket, &outAddr->mSockAddr, &addrlen);

	if (newSock == INVALID_SOCKET)
	{
		SocketUtil::ReportError(L"TCPSocket::Accept");
		return nullptr;
	}

	return TCPSocketPtr(new TCPSocket(newSock));
}

int TCPSocket::Send(const void* data, int len, int flags /*= 0*/)
{
	int sent = send(mSocket, static_cast<const char*>(data), len, flags);
	return sent;
}

int TCPSocket::Recv(void* outData, int len, int flags /*= 0*/)
{
	int read = recv(mSocket, static_cast<char*>(outData), len, flags);
	return read;
}

void TCPSocket::SetNonBlockingMode(u_long mode)
{
	ioctlsocket(mSocket, FIONBIO, &mode);
}
