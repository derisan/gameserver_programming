#pragma once

class GameScene :
    public Scene
{
public:
    static void StaticCreate(Client* client);
    static GameScene* Get();

    virtual void Enter() override;
    virtual void Exit() override;
    virtual void ProcessInput() override;
    virtual void Update(float deltaTime) override;
    virtual void Render(SDL_Renderer* renderer) override;

private:
    GameScene(Client* client = nullptr);

    void processPacket(MemoryStream* outPacket);
    void processCreatePiece(MemoryStream* outPacket);
    void processUpdatePosition(MemoryStream* outPacket);
    void processLoginConfirmed(MemoryStream* outPacket);

private:
    static GameScene sInstance;

    TCPSocketPtr mClientSocket;

    Entity mMyPiece;

    bool mbLoginConfirmed = false;
};
