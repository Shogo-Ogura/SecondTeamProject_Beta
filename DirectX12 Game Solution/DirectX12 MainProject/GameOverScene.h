//
// GameOverScene.h
//

#pragma once

#include "Scene.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class GameOverScene final : public Scene {
public:
    GameOverScene();
    virtual ~GameOverScene() { Terminate(); }

    GameOverScene(GameOverScene&&) = default;
    GameOverScene& operator= (GameOverScene&&) = default;

    GameOverScene(GameOverScene const&) = delete;
    GameOverScene& operator= (GameOverScene const&) = delete;

    // These are the functions you will implement.
    void Initialize() override;
    void LoadAssets() override;

    void Terminate() override;

    void OnDeviceLost() override;
    void OnRestartSound() override;

    NextScene Update(const float deltaTime) override;
    void Render() override;

private:
    DX12::DESCRIPTORHEAP descriptorHeap;
    DX12::SPRITEBATCH    spriteBatch;
    DX12::HGPUDESCRIPTOR dx9GpuDescriptor;

    // ïœêîÇÃêÈåæ
    DX9::SPRITE gameoverSceneSprite;


private:
    NextScene changeNextSceneUpdate();
};