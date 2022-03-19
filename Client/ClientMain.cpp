#include "ClientPCH.h"

#include "Client.h"

int main()
{
	unique_ptr<Client> client = std::make_unique<Client>();

	bool success = client->Init();

	if (success)
	{
		client->Run();
	}

	client->Shutdown();
}