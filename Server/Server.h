#pragma once

#include "Game.h"

class Server : public Game
{
public:
	virtual bool Init() override;
	virtual void Run() override;
	virtual void Shutdown() override;

private:
	Entity createEntity();

	void waitPlayer();
	void initGameWorld();

private:
	TCPSocketPtr mClientSocket;
};

