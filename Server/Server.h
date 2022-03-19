#pragma once

#include "Game.h"

class Server : public Game
{
public:
	virtual bool Init() override;
	virtual void Run() override;
	virtual void Shutdown() override;

private:
	void waitPlayer();

private:
	TCPSocketPtr mClientSocket;
};

