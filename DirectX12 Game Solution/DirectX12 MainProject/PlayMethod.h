//
// PlayMethodScene.h
//

#pragma once

#include "Scene.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class PlayMethodScene final : public Scene {
public:
    PlayMethodScene();
    virtual ~PlayMethodScene() { Terminate(); }

    PlayMethodScene(PlayMethodScene&&) = default;
    PlayMethodScene& operator= (PlayMethodScene&&) = default;

    PlayMethodScene(PlayMethodScene const&) = delete;
    PlayMethodScene& operator= (PlayMethodScene const&) = delete;

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

    //ê‡ñæâÊñ 
    DX9::SPRITE playMethodSprite;


private:
    //ÉVÅ[ÉìëJà⁄
    NextScene changeMainScene();
};