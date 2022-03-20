#include "PCH.h"
#include "SocketAddress.h"

#include <sstream>

string SocketAddress::ToString() const
{
	std::stringstream ss;

	ss << inet_ntoa(GetAsSockAddrIn()->sin_addr) << ":" << ntohs(GetAsSockAddrIn()->sin_port);

	return ss.str();
}
