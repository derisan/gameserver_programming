#pragma once

constexpr int PIECE_WIDTH = 80;
constexpr int PIECE_HEIGHT = 80;

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

	void processPacket(MemoryStream* packet);
	void processUserInput(MemoryStream* packet);

private:
	TCPSocketPtr mClientSocket;
};

int GetChessBoardIndex(int position);

