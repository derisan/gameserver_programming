#include "ServerPCH.h"

#include "Server.h"
#include "ServerGlobal.h"

int main()
{
	gServer = new Server;

	bool success = gServer->Init();

	if (success)
	{
		gServer->Run();
	}

	gServer->Shutdown();

	delete gServer;
	gServer = nullptr;
}