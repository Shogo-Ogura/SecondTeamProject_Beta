//
// GameClearScene.h
//

#pragma once

#include "Scene.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class GameClearScene final : public Scene {
public:
    GameClearScene();
    virtual ~GameClearScene() { Terminate(); }

    GameClearScene(GameClearScene&&) = default;
    GameClearScene& operator= (GameClearScene&&) = default;

    GameClearScene(GameClearScene const&) = delete;
    GameClearScene& operator= (GameClearScene const&) = delete;

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

    // 変数の宣言

    //クリア画面
    DX9::SPRITE clearSceneSprite;

    //クリア時間
    DX9::SPRITEFONT clearTimeFont;
    const float clearTimePositionX = 400.0f;
    const float clearTimePositionY = 600.0f;


    //関数の宣言
    //シーン遷移
    NextScene changeNextSceneUpdate();
};