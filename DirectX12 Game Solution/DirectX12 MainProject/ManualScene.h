//
// ManualScene.h
//

#pragma once

#include "Scene.h"
#include<random>

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class ManualScene final : public Scene {
public:
    ManualScene();
    virtual ~ManualScene() { Terminate(); }

    ManualScene(ManualScene&&) = default;
    ManualScene& operator= (ManualScene&&) = default;

    ManualScene(ManualScene const&) = delete;
    ManualScene& operator= (ManualScene const&) = delete;

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

    //背景
    DX9::SPRITE bgTestSprite;
    float bgPositionX;

    //スクロール速度
    const float smallFishSpeed = 300.0f;
    const float mediumFishSpeed = 500.0f;
    const float largeFishSpeed = 700.0f;
    const float topFishSpeed = 1300.0f;

    //背景ループ位置
    const float bgResetPosition = 2560.0f;
    float bgMoveDistance;

    //ループカウント
    int bgLoopCount;

    //プレイ時間
    float playTime;


    //プレイヤー
    //プレイヤー画像
    DX9::SPRITE fishSprite[3];

    //プレイヤーサイズ
    enum playerScale {
        //小
        smallFishScaleX = 110,
        smallFishScaleY = 70,
        //中
        mediumFishScaleX = 170,
        mediumFishScaleY = 105,
        //大
        largeFishScaleX = 230,
        largeFishScaleY = 170
    };
    const int       fishScaleX[4]{ smallFishScaleX, mediumFishScaleX, largeFishScaleX,largeFishScaleX };
    const int       fishScaleY[4]{ smallFishScaleY,mediumFishScaleY,largeFishScaleY,largeFishScaleY };
    float       fishSpeed[4];


    //プレイヤーアニメーション
    int playerSpriteAnimationX, playerSpriteAnimationY;
    //1コマの表示時間
    const float playerAnimationFrame = 0.05f;
    //アニメーションコマ数
    enum { frameNumber = 8 };
    //アニメーション速度
    float playerAnimationSpeed;

    //プレイヤー状態
    int playerStatus;
    enum playerState {
        //小
        smallFishState,
        //中
        mediumFishState,
        //大
        largeFishState
    };

    //スピード状態
    float speedUpTime;
    int playerSpeedStatus;
    bool speedUp;
    enum playerSpeedState {
        smallFishSpeedState,
        mediumFishSpeedState,
        largeFishSpeedState,
        speedUpState
    };

    //プレイヤー座標
    float playerPositionX, playerPositionY;

    //プレイヤー初期位置
    const float playerInitialPositionX = 100.0f;
    const float playerInitialPositionY = 300.0f;

    //プレイヤー自動移動速度
    enum { playerAutoMoveSpeed = 300 };

    //プレイヤー移動速度
    //キーボード
    const float   keyboardPlayerUpMoveSpeed    = -30.0f;
    const float   keyboardPlayerDownMoveSpeed  =  30.0f;
    const float   keyboardPlayerRightMoveSpeed =  30.0f;
    const float   keyboardPlayerLeftMoveSpeed  = -30.0f;

    //パッド
    //十字キー
    const float gamePadButtonPlayerMoveSpeedUp    = -30.0f;
    const float gamePadButtonPlayerMoveSpeedDown  =  30.0f;
    const float gamePadButtonPlayerMoveSpeedRight =  30.0f;
    const float gamePadButtonPlayerMoveSpeedLeft  = -30.0f;
    //スティック
    const float gamePadPlayerMoveSpeedX = 500.0f;
    const float gamePadPlayerMoveSpeedY = 500.0f;

    //慣性
    float playerInertiaX, playerInertiaY;

    //プレイヤー移動範囲
    enum playerMoveRange {
        playerMoveRangeTop    = 251,
        playerMoveRangeBottom = 720,
        playerMoveRangeRight  = 1280,
        playerMoveRangeLeft   = 0
    };


    //餌(アイテム)
    enum { feedMaxAmount = 14 };    //最大量
    DX9::SPRITE feedTestSprite[feedMaxAmount];
    float feedPositionX[feedMaxAmount], feedPositionY[feedMaxAmount];
    bool feedGet;
    float feedGetTime;

    //餌サイズ
    enum {
        feedScaleX = 80,
        feedScaleY = 70
    };

    //餌移動速度
    const float feedMoveSpeed = 350.0f;


    //障害物
    //各種障害物最大量
    enum obstacleamount {
        birdMaxAmount      = 3,
        bigRockMaxAmount   = 15,
        smallRockMaxAmount = 34,
        woodMaxAmount      = 2
    };

    //鳥
    DX9::SPRITE birdTestSprite[birdMaxAmount];
    float birdPositionX[birdMaxAmount];
    float birdPositionY[birdMaxAmount];

    //岩(大)
    DX9::SPRITE bigRockSprite[bigRockMaxAmount];
    float bigRockPositionX[bigRockMaxAmount], bigRockPositionY[bigRockMaxAmount];

    //岩(小)
    DX9::SPRITE smallRockSprite[smallRockMaxAmount];
    float smallRockPositionX[smallRockMaxAmount], smallRockPositionY[smallRockMaxAmount];

    //木
    DX9::SPRITE woodSprite[woodMaxAmount];
    float woodPositionX[woodMaxAmount], woodPositionY[woodMaxAmount];

    bool obstacleCollision;
    float obstacleCollisionTime;

    //障害物移動速度
    const float obstacleMoveSpeed = 350.0f;

    //障害物サイズ
    enum obstacleScale {
        //鳥
        birdScaleX = 230,
        birdScaleY = 100,
        //岩(大)
        bigRockScaleX = 230,
        bigRockScaleY = 210,
        //岩(小)
        smallRockScaleX = 40,
        smallRockScaleY = 50,
        //木
        woodScaleX = 230,
        woodScaleY = 130
    };

    /*//障害物状態
    int obstacleStatus;
    enum obstacleState {
        birdState,
        bigRockState,
        smallRockState,
        woodState
    };*/


    //プレイヤー、餌、障害物Z座標幅
    const float zPositionWidth = 100.0f;


    //UI
    //ミニマップ
    DX9::SPRITE miniMapSprite;
    enum miniMapScale {
        miniMapScaleX = 600,
        miniMapScaleY = 30
    };
    DX9::SPRITE miniMapFishTestSprite;
    enum miniMapFishScale {
        miniMapFishScaleX = 94,
        miniMapFishScaleY = 70
    };
    const float miniMapPositionX = 100;
    const float miniMapPositionY = 35;
    float miniMapFishPositionX, miniMapFishPositionY;
    const float miniMapFishInitialPositionX = 100;
    const float miniMapFishInitialPositionY = 10;

    //ゲージ
    DX9::SPRITE gaugeTestSprite;
    DX9::SPRITE gaugeBgTestSprite;
    float gaugeWidth;
    int gaugeState;
    enum gaugeState {
        firstStage = 140,
        secondStage = 280,
        thirdStage = 420,
        forthStage = 560,
        fifthStage = 700
    };
    const float gaugePositionX = 70.0f;
    const float gaugePositionY = 100.0f;

    //シーン遷移
    //ゴール
    DX9::SPRITE goalSprite;
    const float goalSpritePositionX = 0.0f;
    const float goalSpritePositionY = 0.0f;
    float goalCollisionPositionX, goalCollisionPositionY;
    bool goal;
    float sceneChangeBuffer;
    //ゴール距離
    enum { lengthToGoal = 3 };
    //ゴール後遷移までの時間
    const float goalAfterTime = 2.0f;


    //ゲームオーバー
    bool gameOver;


    //デバッグ用
    DX9::SPRITEFONT playerStatusFont;
    DX9::SPRITEFONT gaugeStageFont;


private:
    //Update内関数の定義

    //背景
    //スクロール速度
    void bgMoveSpeedUpdate(const float deltaTime);

    //背景ループ
    void bgLoopUpdate(const float deltaTime);

    //スクロール速度変更
    void setBgScrollSpeed();


    //クリア時間計測
    void countPlayTimeUpdate(const float deltaTime);


    //状態遷移割当
    int gaugePlayerStateAssignUpdate();

    //状態遷移
    int gaugeStateUpdate(const float deltaTime);

    //無敵時間
    void invalidTime(const float deltaTime);


    //プレイヤー
    //アニメーション 
    void playerAnimationUpdate(const float deltaTime);

    //移動可能範囲
    void playerMoveRangeUpdate();

    //自動移動
    void playerAutoMoveUpdate(const float deltaTime);

    //キーボード操作
    void playerControlKeyboardUpdate(const float deltaTime);

    //パッド操作
    void playerControlGamepadUpdate(const float deltaTime);


    //餌(アイテム)
    //餌位置設定
    void feedPositionSetting();
    
    //餌移動
    void feedMoveUpdate(const float deltaTime);

    //餌当たり判定
    bool isFeedCollisionDetectionUpdate();


    //障害物
    //鳥位置設定
    void birdPositionSetting();

    //岩(大)
    void bigRockPositionSetting();

    //岩(小)
    void smallRockPositionSetting();

    //木
    void woodPositionSetting();
    
    //障害物移動
    void obstacleMoveUpdate(const float deltaTime);

    //障害物当たり判定 
    bool isObstacleCollisionDetectionUpdate();

    //UI
    //ミニマップ
    void miniMapMoveUpdate(const float deltaTime);

    //ゲージ
    void gaugeMoveUpdate();

    //ゴール
    void goalCollisionDetectionUpdate();


    //シーン遷移
    NextScene changeNextSceneUpdate(const float deltaTime);


    //当たり判定関数
    //ベース当たり判定
    bool isCollisionDetectionBase(Rect& rect1, Rect& rect2);

    //プレイヤー範囲設定済み当たり判定
    bool isPlayerCollisionDetection(Rect& rect2);

};