#include "ClientPCH.h"

#include <iostream>

#include "Client.h"

string gServerAddr;

int main()
{
	GS_LOG("������ IP �ּ� �Է�: ");
	std::getline(std::cin, gServerAddr);

	unique_ptr<Client> client = std::make_unique<Client>();

	bool success = client->Init();

	if (success)
	{
		client->Run();
	}

	client->Shutdown();
}