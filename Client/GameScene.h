#pragma once

#include "Scene.h"
#include "Entity.h"

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

private:
    static GameScene sInstance;

    Entity mPiece;

    int mChessPieceOffset;
};

