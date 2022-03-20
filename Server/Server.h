#pragma once

#include "Game.h"

constexpr int kPieceWidth = 80;
constexpr int kPieceHeight = 80;

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

int GetChessBoardIndex(int position);

