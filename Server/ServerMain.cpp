#include "ServerPCH.h"

#include "Server.h"

int main()
{
	unique_ptr<Server> server = std::make_unique<Server>();

	bool success = server->Init();

	if (success)
	{
		server->Run();
	}

	server->Shutdown();
}