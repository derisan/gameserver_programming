#pragma once

constexpr int PIECE_WIDTH = 80;
constexpr int PIECE_HEIGHT = 80;
constexpr int MAX_PLAYER_NUM = 10;

#include "Session.h"

class Server : public Game
{
	enum
	{
		DONT_CARE = -1,
	};

public:
	virtual bool Init() override;
	virtual void Run() override;
	virtual void Shutdown() override;

	void ProcessPacket(MemoryStream* packet, Session& session);

private:
	Entity createEntity();

	void processUserInput(MemoryStream* packet, Session& session);
	void processLoginRequest(MemoryStream* packet, Session& session);

private:
	SOCKET mListenSocket;
};

int GetChessBoardIndex(int position);

