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

	std::map<int32, GSID>& GetClientToPiece() { return mClientToPiece; }

private:
	Entity createEntity();

	void processUserInput(MemoryStream* packet, Session& session);
	void processLoginRequest(MemoryStream* packet, Session& session);

private:
	SOCKET mListenSocket;

	std::map<int32, GSID> mClientToPiece;
};

int GetChessBoardIndex(int position);

