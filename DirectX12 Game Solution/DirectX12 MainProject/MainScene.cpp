//
// MainScene.cpp
//

#include "Base/pch.h"
#include "Base/dxtk.h"
#include "SceneFactory.h"
#include "DontDestroyOnLoad.h"

// Initialize member variables.
MainScene::MainScene() : dx9GpuDescriptor{}
{
    //コンストラクター
    //変数の初期化（0にする）

}

// Initialize a variable and audio resources.
void MainScene::Initialize()
{
    //ゲームシーンの初期化
    //変数を初期化（具体的な数値を設定する）
    //サウンドを読み込んだりする
    
    //背景
    bgPositionX = 0.0f;
    bgMoveDistance  = 0.0f;

    //ループカウント
    loopCount = 0;

    //クリア時間
    DontDestroy->clearTime = 0.0f;

    //ゲージ段階
    gaugeState = firstStage;


    //プレイヤー
    //座標
    playerPositionX = playerInitialPositionX;
    playerPositionY = playerInitialPositionX;

    //慣性
    playerInertiaX = 0.0f;
    playerInertiaY = 0.0f;

    //プレイヤーアニメーション
    playerSpriteAnimationX = 0;
    playerSpriteAnimationY = 0;

    //アニメーション速度
    playerAnimationSpeed = 0.0f;

    //高速時間
    speedUpTime = 0.0f;

    //高速フラグ
    speedUp = false;

    //状態
    playerStatus = smallFishState;
    playerSpeedStatus = smallFishSpeedState;


    //餌(アイテム)
    //初期位置
    feedPositionX = feedInitialPositionX;
    feedPositionY = feedInitialPositionY;

    //餌位置リセット
    std::random_device rnd_dev;
    randomEngine = std::mt19937(rnd_dev());
    randomFeedPositionY = std::uniform_real_distribution<float>(feedAppearanceTop, feedAppearanceBottom);


    //障害物
    //障害物初期位置
    //鳥
    birdPositionX = obstacleInitialPositionX;
    birdPositionY = obstacleInitialPositionY;

    //岩(大)
    bigRockPositionX = obstacleInitialPositionX;
    bigRockPositionY = obstacleInitialPositionY;

    //岩(小)
    smallRockPositionX = obstacleInitialPositionX;
    smallRockPositionY = obstacleInitialPositionY;

    //木
    woodPositionX = obstacleInitialPositionX;
    woodPositionY = obstacleInitialPositionY;

    //障害物状態
    obstacleStatus = bigRockState;

    //岩以外障害物ランダムリセット座標
    randomObstaclePositionY = std::uniform_real_distribution<float>(obstacleAppearanceTop, obstacleAppearanceBottom);
    obstacleResetPositionY = randomObstaclePositionY(randomEngine);

    //岩(大)場所状態
    bigRockPositionState = topPositionState;
    
    //岩(大)ランダムリセット座標
    randomBigRockPosition = std::uniform_int_distribution<int>(topPositionState, bottomPositionState);
    bigRockPositionPattern = randomBigRockPosition(randomEngine);

    //ランダム
    randomObstacle = std::uniform_int_distribution<int>(1, 4);


    //UI
    //ミニマップ
    miniMapFishPositionX = miniMapPositionX - miniMapFishScaleX / 2;
    miniMapFishPositionY = miniMapPositionY - miniMapFishScaleY / 2;
    
    //ゲージ
    gaugeWidth = firstStage;


    //ゴール
    goal = false;
    sceneChangeBuffer = 0.0f;

    //ゲームオーバー
    gameOver = false;


    
}

// Allocate all memory the Direct3D and Direct2D resources.
void MainScene::LoadAssets()
{
    //グラフィックの設定をする
    //Windows10はドライバがアップデートされると読み込み直しが必要になる

    descriptorHeap = DX12::CreateDescriptorHeap(DXTK->Device, 1);

    ResourceUploadBatch resourceUploadBatch(DXTK->Device);
    resourceUploadBatch.Begin();

    RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
    SpriteBatchPipelineStateDescription pd(rtState, &CommonStates::NonPremultiplied);
    D3D12_VIEWPORT viewport = {
        0.0f, 0.0f, 1280.0f, 720.0f,
        D3D12_MIN_DEPTH, D3D12_MAX_DEPTH
    };

    

    spriteBatch = DX12::CreateSpriteBatch(DXTK->Device, resourceUploadBatch, pd, &viewport);

    dx9GpuDescriptor = DXTK->Direct3D9->CreateShaderResourceView(descriptorHeap.get(), 0);

    auto uploadResourcesFinished = resourceUploadBatch.End(DXTK->CommandQueue);
    uploadResourcesFinished.wait();


    // グラフィックリソースの初期化処理

    //背景
    bgTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bgSprite02.png");
    
    
    //プレイヤー
    //小
    fishSprite[smallFishState] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"smallFishSprite.png");

    //中
    fishSprite[mediumFishState] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"mediumFishSprite.png");

    //大
    fishSprite[largeFishState] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"largeFishSprite.png");


    //餌(アイテム)
    feedTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"wormSprite.png");


    //障害物
    //鳥
    birdTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"birdSprite.png");

    //岩(大)
    bigRockSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bigRockSprite.png");

    //岩(小)
    smallRockSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"smallRockSprite.png");

    //木
    woodSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"woodSprite.png");


    //UI
    //ミニマップ
    miniMapSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"miniMapSprite.png");
    miniMapFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"miniMapFishTestSprite.png");
    
    //ゲージ
    gaugeTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"gaugeTestSprite.png");
    gaugeBgTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"gaugeBgTestSprite.png");


    //ゴール
    goalSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"goalSprite.png");


    //デバッグ用
    playerStatusFont = DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"ＭＳ Ｐ明朝", 20);
    gaugeStageFont = DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"ＭＳ Ｐ明朝", 20);

}

// Releasing resources required for termination.
void MainScene::Terminate()
{
    DXTK->ResetAudioEngine();
    DXTK->WaitForGpu();

    // TODO: Add your Termination logic here.
    //ゲーム(シーン)終了時に何かしたい場合に使う
}

// Direct3D resource cleanup.
void MainScene::OnDeviceLost()
{
    //ドライバーアップデートを検出した場合に使う
    //読み込みなおす前に何かしたい場合に何か書く
}

// Restart any looped sounds here
void MainScene::OnRestartSound()
{
    //サウンドデバイスが切り替わるとサウンドが全部停止する
    //BGMなどを再スタートさせる
}

// Updates the scene.
NextScene MainScene::Update(const float deltaTime)
{
    // If you use 'deltaTime', remove it.
    UNREFERENCED_PARAMETER(deltaTime);

    // TODO: Add your game logic here.
    //ゲームを動かすプログラムを記述する

    //背景
    bgLoopUpdate(deltaTime);

    //スクロール速度
    bgMoveSpeedUpdate(deltaTime);

    //スクロール速度割当
    setBgScrollSpeed();


    //クリア時間計測
    countPlayTimeUpdate(deltaTime);


    //状態遷移
    gaugeStateUpdate(deltaTime);

    //状態遷移割当
    auto old_state = playerStatus;
    if (old_state != gaugePlayerStateAssignUpdate()) {
        playerAnimationSpeed   = 0;
        playerSpriteAnimationX = 0;
    }
    
    //プレイヤー
    //アニメーション
    playerAnimationUpdate(deltaTime);

    //移動可能範囲
    playerMoveRangeUpdate();

    //自動移動
    playerAutoMoveUpdate(deltaTime);

    //キーボード操作
    playerControlKeyboardUpdate(deltaTime);

    //パッド操作
    playerControlGamepadUpdate(deltaTime);


    //餌(アイテム)
    //移動
    feedMoveUpdate(deltaTime);

    //餌ループ
    feedLoopUpdate();

    //障害物
    //移動
    obstacleMoveUpdate(deltaTime);

    //障害物ループ
    obstacleLoopUpdate();


    //UI
    //ミニマップ
    miniMapMoveUpdate(deltaTime);
    
    //ゲージ
    gaugeMoveUpdate();


    //シーン遷移
    return changeNextSceneUpdate(deltaTime);

}

// Draws the scene.
void MainScene::Render()
{
    // TODO: Add your rendering code here.
    //描画のプログラムを書く
    
    DXTK->Direct3D9->Clear(DX9::Colors::RGBA(0, 0, 0, 255));  // 手順1

    DXTK->Direct3D9->BeginScene();    // 手順2
    DX9::SpriteBatch->Begin();        // 手順4

    // (ここに2D描画の処理が入る)     // 手順5

    //背景
    DX9::SpriteBatch->DrawSimple(bgTestSprite.Get(), SimpleMath::Vector3(bgPositionX, 0, 10));


    //プレイヤー
    DX9::SpriteBatch->DrawSimple(
        fishSprite[playerStatus].Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -(playerPositionY + fishScaleY[playerStatus]) / zPositionWidth),
        RectWH(playerSpriteAnimationX, playerSpriteAnimationY, fishScaleX[playerStatus], fishScaleY[playerStatus])
    );

    ////小
    //if (gaugeStage == firstStage || gaugeStage == secondStage) {
    //    DX9::SpriteBatch->DrawSimple(smallFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5),
    //        RectWH(playerSpriteAnimationX, playerSpriteAnimationY, smallFishScaleX, smallFishScaleY)
    //    );
    //}
    ////中
    //if (gaugeStage == thirdStage || gaugeStage == forthStage) {
    //    DX9::SpriteBatch->DrawSimple(mediumFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5),
    //        RectWH(playerSpriteAnimationX,playerSpriteAnimationY,mediumFishScaleX,mediumFishScaleY)
    //        );
    //}
    ////大
    //if (gaugeStage == fifthStage) {
    //    DX9::SpriteBatch->DrawSimple(largeFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5),
    //        RectWH(playerSpriteAnimationX, playerSpriteAnimationY, largeFishScaleX, largeFishScaleY)
    //    );
    //}


    //餌(アイテム)
    DX9::SpriteBatch->DrawSimple(feedTestSprite.Get(), SimpleMath::Vector3(feedPositionX, feedPositionY, 0));


    //障害物
    //鳥 
    DX9::SpriteBatch->DrawSimple(birdTestSprite.Get(), SimpleMath::Vector3(birdPositionX, birdPositionY, -(birdPositionY + birdScaleY) / zPositionWidth));
   
    //岩(大)
    DX9::SpriteBatch->DrawSimple(bigRockSprite.Get(), SimpleMath::Vector3(bigRockPositionX, bigRockPositionY, -(bigRockPositionY + bigRockScaleY) / zPositionWidth));
    
    //岩(小)
    DX9::SpriteBatch->DrawSimple(smallRockSprite.Get(), SimpleMath::Vector3(smallRockPositionX, smallRockPositionY, -(bigRockPositionY + smallRockScaleY) / zPositionWidth));

    //木
    DX9::SpriteBatch->DrawSimple(woodSprite.Get(), SimpleMath::Vector3(woodPositionX, woodPositionY, -(woodPositionY + woodScaleY) / zPositionWidth));


    //UI
    //ミニマップ
    DX9::SpriteBatch->DrawSimple(miniMapSprite.Get(), SimpleMath::Vector3(miniMapPositionX, miniMapPositionY, -4));
    DX9::SpriteBatch->DrawSimple(miniMapFishTestSprite.Get(), SimpleMath::Vector3(miniMapFishPositionX, miniMapFishPositionY, -4));

    //ゲージ
    //本体
    DX9::SpriteBatch->DrawSimple(gaugeTestSprite.Get(), SimpleMath::Vector3(gaugePositionX, gaugePositionY, 8),
        RectWH(0.0f, 0.0f, gaugeWidth, 100.0f));

    //枠
    DX9::SpriteBatch->DrawSimple(gaugeBgTestSprite.Get(), SimpleMath::Vector3(gaugePositionX, gaugePositionY, 9));


    //ゴール
    if (goal)
        DX9::SpriteBatch->DrawSimple(goalSprite.Get(), SimpleMath::Vector3(goalSpritePositionX, goalSpritePositionY, -10));


    //デバッグ用
    DX9::SpriteBatch->DrawString
    (
        playerStatusFont.Get(), SimpleMath::Vector2(0, 670), 
        DX9::Colors::RGBA(0, 0, 0, 255), L"miniMapFishPositionX:%d", goal
    );

    DX9::SpriteBatch->DrawString(
        gaugeStageFont.Get(), SimpleMath::Vector2(500.0f, 670.0f), 
        DX9::Colors::RGBA(0, 0, 0, 255), L"clearTime:%d", (int)DontDestroy->clearTime
    );


    DX9::SpriteBatch->End();          // 手順6
    DXTK->Direct3D9->EndScene();      // 手順7

    DXTK->Direct3D9->UpdateResource();

    DXTK->ResetCommand();
    DXTK->ClearRenderTarget(Colors::CornflowerBlue);

    const auto heapes = descriptorHeap->Heap();
    DXTK->CommandList->SetDescriptorHeaps(1, &heapes);

    spriteBatch->Begin(DXTK->CommandList);
    spriteBatch->Draw(
        dx9GpuDescriptor,
        XMUINT2(1280, 720),
        SimpleMath::Vector2(0.0f,0.0f)
    );
    spriteBatch->End();

    DXTK->Direct3D9->WaitUpdate();
    DXTK->ExecuteCommandList();
}

//Update内関数の定義

//背景
//スクロール速度
void MainScene::bgMoveSpeedUpdate(const float deltaTime)
{
    /*if (playerSpeedStatus != speedUpState) {
        bgPositionX -= fishSpeed[playerSpeedStatus] * deltaTime;
    }
    else {
        speedUpTime += deltaTime;
        bgPositionX -= topSpeed * deltaTime;
        if (speedUpTime >= 2.0f)
        {
            speedUpTime = 0.0f;
            playerSpeedStatus = largeFishSpeedState;
            speedUp = false;
        }
    }*/
    if (!goal) {
        switch (playerSpeedStatus) {
        case smallFishSpeedState:
            bgPositionX -= smallFishSpeed * deltaTime;
            bgMoveDistance += smallFishSpeed * deltaTime;
            break;
        case mediumFishSpeedState:
            bgPositionX -= mediumFishSpeed * deltaTime;
            bgMoveDistance += mediumFishSpeed * deltaTime;
            break;
        case largeFishSpeedState:
            bgPositionX -= largeFishSpeed * deltaTime;
            bgMoveDistance += largeFishSpeed * deltaTime;
            break;
        case speedUpState:
            speedUpTime += deltaTime;
            bgPositionX -= topSpeed * deltaTime;
            bgMoveDistance += topSpeed * deltaTime;
            if (speedUpTime >= 2.0f)
            {
                playerSpeedStatus = largeFishSpeedState;
                speedUp = false;
            }
            break;
        }
    }
}

//背景ループ
void MainScene::bgLoopUpdate(const float deltaTime)
{
    if (bgPositionX <= -bgResetPosition) {
        bgPositionX = 0;
        ++loopCount;
    }
}

//スクロール速度変更
void MainScene::setBgScrollSpeed()
{
    switch (playerStatus){
    case smallFishState:
        playerSpeedStatus = smallFishSpeedState;
        break;
    case mediumFishState:
        playerSpeedStatus = mediumFishSpeedState;
        break;
    case largeFishState:
        if (speedUp)
        {
            playerSpeedStatus = speedUpState;
        }
        else
            playerSpeedStatus = largeFishSpeedState;
        break;
    }
}


//状態遷移割当
int MainScene::gaugePlayerStateAssignUpdate()
{
    if (gaugeState == firstStage || gaugeState == secondStage) {
        playerStatus = smallFishState;
    }
    else if (gaugeState == thirdStage || gaugeState == forthStage) {
        playerStatus = mediumFishState;
    }
    else if (gaugeState == fifthStage) {
        playerStatus = largeFishState;
    }

    return playerStatus;
}


//クリア時間計測
void MainScene::countPlayTimeUpdate(const float deltaTime)
{
    if (!goal)
        DontDestroy->clearTime += deltaTime;
}


//状態遷移
int MainScene::gaugeStateUpdate(const float deltaTime)
{
    switch (gaugeState) {
    case firstStage:
        if (isFeedCollisionDetectionUpdate())
        {
            feedPositionResetUpdate();
            gaugeState = secondStage;
        }
        else if (isObstacleCollisionDetectionUpdate())
        {
            gameOver = true;
        }
        break;
    case secondStage:
        if (isFeedCollisionDetectionUpdate())
        {
            feedPositionResetUpdate();
            gaugeState = thirdStage;
        }
        else if (isObstacleCollisionDetectionUpdate())
        {
            obstaclePositionResetUpdate();
            gaugeState = firstStage;
        }
        break;
    case thirdStage:
        if (isFeedCollisionDetectionUpdate())
        {
            feedPositionResetUpdate();
            gaugeState = forthStage;
        }
        else if (isObstacleCollisionDetectionUpdate())
        {
            obstaclePositionResetUpdate();
            gaugeState = secondStage;
        }
        break;
    case forthStage:
        if (isFeedCollisionDetectionUpdate())
        {
            feedPositionResetUpdate();
            gaugeState = fifthStage;
        }
        else if (isObstacleCollisionDetectionUpdate())
        {
            obstaclePositionResetUpdate();
            gaugeState = thirdStage;
        }
        break;
    case fifthStage:
        if (isFeedCollisionDetectionUpdate())
        {
            speedUp = true;
            feedPositionResetUpdate();
        }
        else if (isObstacleCollisionDetectionUpdate())
        {
            obstaclePositionResetUpdate();
            speedUpTime = 0.0f;
            gaugeState = forthStage;
        }
        break;
    }

    return gaugeState;
}


//プレイヤー
//アニメーション
void MainScene::playerAnimationUpdate(const float deltaTime)
{
    if (!goal) {
        playerAnimationSpeed += deltaTime;
        if (playerAnimationSpeed >= playerAnimationFrame)
        {
            playerSpriteAnimationX += fishScaleX[playerSpeedStatus];
            playerAnimationSpeed = 0.0f;
        }
        if (playerSpriteAnimationX >= (fishScaleX[playerSpeedStatus] * 8))
        {
            playerSpriteAnimationX = 0;
        }
    }
}

//移動可能範囲
void MainScene::playerMoveRangeUpdate()
{
    //上
    if (playerPositionY <= playerMoveRangeTop)
    {
        playerPositionY = playerMoveRangeTop;
    }
    //左
    if (playerPositionX <= playerMoveRangeLeft)
    {
        playerPositionX = playerMoveRangeLeft;
    }
    //右
    if (playerPositionX >= playerMoveRangeRight - fishScaleX[playerStatus])
    {
        playerPositionX = playerMoveRangeRight - fishScaleX[playerStatus];
    }
    //下
    if (playerPositionY >= playerMoveRangeBottom - fishScaleY[playerStatus])
    {
        playerPositionY = playerMoveRangeBottom - fishScaleY[playerStatus];
    }
}

//自動移動
void MainScene::playerAutoMoveUpdate(const float deltaTime)
{
    if (!goal)
        playerPositionX -= playerAutoMoveSpeed * deltaTime;
}

//キーボード操作
void MainScene::playerControlKeyboardUpdate(const float deltaTime)
{
    if (!goal) {
        //左右
        if (DXTK->KeyState->D)
        {
            playerInertiaX = playerInertiaX + deltaTime * (keyboardPlayerRightMoveSpeed - playerInertiaX);
        }
        if (DXTK->KeyState->A)
        {
            playerInertiaX = playerInertiaX + deltaTime * (keyboardPlayerLeftMoveSpeed - playerInertiaX);
        }
        if (!DXTK->KeyState->D && !DXTK->KeyState->A)
        {
            playerInertiaX = playerInertiaX + 2 * deltaTime * (0.0f - playerInertiaX);
        }

        playerPositionX += playerInertiaX;

        //上下
        if (DXTK->KeyState->S)
        {
            playerInertiaY = playerInertiaY + deltaTime * (keyboardPlayerDownMoveSpeed - playerInertiaY);
        }
        if (DXTK->KeyState->W)
        {
            playerInertiaY = playerInertiaY + deltaTime * (keyboardPlayerUpMoveSpeed - playerInertiaY);
        }
        if (!DXTK->KeyState->D && !DXTK->KeyState->A)
        {
            playerInertiaY = playerInertiaY + 2 * deltaTime * (0.0f - playerInertiaY);
        }

        playerPositionY += playerInertiaY;
    }
}

//パッド操作
void MainScene::playerControlGamepadUpdate(const float deltaTime)
{
    if (!goal) {
        //十字キー
        if (DXTK->GamePadState[0].IsDPadRightPressed())
        {
            playerInertiaX = playerInertiaX + deltaTime * (gamePadButtonPlayerMoveSpeedRight - playerInertiaX);
        }
        if (DXTK->GamePadState[0].IsDPadLeftPressed())
        {
            playerInertiaX = playerInertiaX + deltaTime * (gamePadButtonPlayerMoveSpeedLeft - playerInertiaX);
        }
        if (!DXTK->GamePadState[0].IsDPadRightPressed() && !DXTK->GamePadState[0].IsDPadLeftPressed())
        {
            playerInertiaX = playerInertiaX + 2 * deltaTime * (0.0f - playerInertiaX);
        }

        playerPositionX += playerInertiaX;


        if (DXTK->GamePadState[0].IsDPadDownPressed())
        {
            playerInertiaY = playerInertiaY + deltaTime * (gamePadButtonPlayerMoveSpeedDown - playerInertiaY);
        }
        if (DXTK->GamePadState[0].IsDPadUpPressed())
        {
            playerInertiaY = playerInertiaY + deltaTime * (gamePadButtonPlayerMoveSpeedUp - playerInertiaY);
        }
        if (!DXTK->GamePadState[0].IsDPadDownPressed() && !DXTK->GamePadState[0].IsDPadUpPressed())
        {
            playerInertiaY = playerInertiaY + 2 * deltaTime * (0.0f - playerInertiaY);
        }

        playerPositionY += playerInertiaY;

        //スティック
        /*playerPositionX += gamePadPlayerMoveSpeedX * DXTK->GamePadState[0].thumbSticks.leftX * deltaTime;
        playerPositionY -= gamePadPlayerMoveSpeedY * DXTK->GamePadState[0].thumbSticks.leftY * deltaTime;*/
    }
}


//餌(アイテム)
//移動
void MainScene::feedMoveUpdate(const float deltaTime)
{
    if (!goal) {
        switch (playerSpeedStatus) {
        case smallFishSpeedState:
            feedPositionX -= feedMoveSpeed * deltaTime;
            break;
        case mediumFishSpeedState:
            feedPositionX -= (feedMoveSpeed + mediumFishSpeed) * deltaTime;
            break;
        case largeFishSpeedState:
            feedPositionX -= (feedMoveSpeed + largeFishSpeed) * deltaTime;
            break;
        case speedUpState:
            feedPositionX -= (feedMoveSpeed + topSpeed) * deltaTime;
            break;
        }
    }
}

//餌ループ
void MainScene::feedLoopUpdate()
{
    if (feedPositionX <= feedResetPositionX)
    {
        feedPositionResetUpdate();
    }
}

//位置リセット
void MainScene::feedPositionResetUpdate()
{
    feedResetPositionY = randomFeedPositionY(randomEngine);

    feedPositionX = feedInitialPositionX;
    feedPositionY = feedResetPositionY;
}

//餌当たり判定
bool MainScene::isFeedCollisionDetectionUpdate()
{
    if (isPlayerCollisionDetection(RectWH(feedPositionX, feedPositionY, feedScaleX, feedScaleY)))
        return true;
    else
        return false;
}


//障害物
//障害物移動 
void MainScene::obstacleMoveUpdate(const float deltaTime)
{
    if (!goal) {
        switch (obstacleStatus) {
        case birdState:
            switch (playerSpeedStatus) {
            case smallFishSpeedState:
                birdPositionX -= obstacleMoveSpeed * deltaTime;
                break;
            case mediumFishSpeedState:
                birdPositionX -= (obstacleMoveSpeed + mediumFishSpeed) * deltaTime;
                break;
            case largeFishState:
                birdPositionX -= (obstacleMoveSpeed + largeFishSpeed) * deltaTime;
                break;
            case speedUpState:
                birdPositionX -= (obstacleMoveSpeed + topSpeed) * deltaTime;
                break;
            }
            break;
        case bigRockState:
            switch (playerSpeedStatus) {
            case smallFishSpeedState:
                bigRockPositionX -= obstacleMoveSpeed * deltaTime;
                break;
            case mediumFishSpeedState:
                bigRockPositionX -= (obstacleMoveSpeed + mediumFishSpeed) * deltaTime;
                break;
            case largeFishSpeedState:
                bigRockPositionX -= (obstacleMoveSpeed + largeFishSpeed) * deltaTime;
                break;
            case speedUpState:
                bigRockPositionX -= (obstacleMoveSpeed + topSpeed) * deltaTime;
                break;
            }
            break;
        case smallRockState:
            switch (playerSpeedStatus) {
            case smallFishSpeedState:
                smallRockPositionX -= obstacleMoveSpeed * deltaTime;
                break;
            case mediumFishSpeedState:
                smallRockPositionX -= (obstacleMoveSpeed + mediumFishSpeed) * deltaTime;
                break;
            case largeFishSpeedState:
                smallRockPositionX -= (obstacleMoveSpeed + largeFishSpeed) * deltaTime;
                break;
            case speedUpState:
                smallRockPositionX -= (obstacleMoveSpeed + topSpeed) * deltaTime;
                break;
            }
            break;
        case woodState:
            switch (playerSpeedStatus) {
            case smallFishSpeedState:
                woodPositionX -= obstacleMoveSpeed * deltaTime;
                break;
            case mediumFishSpeedState:
                woodPositionX -= (obstacleMoveSpeed + mediumFishSpeed) * deltaTime;
                break;
            case largeFishSpeedState:
                woodPositionX -= (obstacleMoveSpeed + largeFishSpeed) * deltaTime;
                break;
            case speedUpState:
                woodPositionX -= (obstacleMoveSpeed + topSpeed) * deltaTime;
                break;
            }
            break;
        }
    }
}

/*
//鳥
void MainScene::birdObstacleMoveUpdate(const float deltaTime)
{
    birdPositionX -= 300.0f * deltaTime;
}

//岩(大)
void MainScene::bigRockObstacleMoveUpdate(const float deltaTime)
{
    bigRockPositionX -= 300.0f * deltaTime;
}

//岩(小)
void MainScene::smallRockObstacleMoveUpdate(const float deltaTime)
{
    smallRockPositionX -= 300.0f * deltaTime;
}

//木
void MainScene::woodObstacleMoveUpdate(const float deltaTime)
{
    woodPositionX -= 300.0f * deltaTime;
}
*/

//障害物当たり判定
bool MainScene::isObstacleCollisionDetectionUpdate()
{
    switch (obstacleStatus) {
    case birdState:
        if (isPlayerCollisionDetection(RectWH(birdPositionX, birdPositionY, birdScaleX, birdScaleY)))
            return true;
        break;
    case bigRockState:
        if (isPlayerCollisionDetection(RectWH(bigRockPositionX, bigRockPositionY, bigRockScaleX, bigRockScaleY)))
            return true;
        break;
    case smallRockState:
        if (isPlayerCollisionDetection(RectWH(smallRockPositionX, smallRockPositionY, smallRockScaleX, smallRockScaleY)))
            return true;
        break;
    case woodState:
        if (isPlayerCollisionDetection(RectWH(woodPositionX, woodPositionY, woodScaleX, woodScaleY)))
            return true;
        break;
    }

    return false;
}

//障害物ループ
void MainScene::obstacleLoopUpdate()
{
    if (birdPositionX < obstacleResetPositionX || bigRockPositionX < obstacleResetPositionX || smallRockPositionX < obstacleResetPositionX || woodPositionX < obstacleResetPositionX) 
    {
        obstaclePositionResetUpdate();
    }
}

//障害物再抽選
void MainScene::obstacleReLotteryUpdate()
{
    obstaclePattern = randomObstacle(randomEngine);
    switch (obstaclePattern) {
    case birdState:
        obstacleStatus = birdState;
        break;
    case bigRockState:
        bigRockPositionPattern = randomBigRockPosition(randomEngine);
        obstacleStatus = bigRockState;
        break;
    case smallRockState:
        obstacleStatus = smallRockState;
        break;
    case woodState:
        obstacleStatus = woodState;
        break;
    }
}

//障害物位置リセット
void MainScene::obstaclePositionResetUpdate()
{
    obstacleReLotteryUpdate();
    if (obstacleStatus == bigRockState) {
        switch (bigRockPositionPattern) {
        case topPositionState:
            obstacleResetPositionY = bigRockTopPosition;
            break;
        case bottomPositionState:
            obstacleResetPositionY = bigRockBottomPosition;
            break;
        }

    }
    else {
        obstacleResetPositionY = randomObstaclePositionY(randomEngine);
    }

    //鳥
    birdPositionX = obstacleInitialPositionX;
    birdPositionY = obstacleResetPositionY;
    //岩(大)
    bigRockPositionX = obstacleInitialPositionX;
    bigRockPositionY = obstacleResetPositionY;
    //岩(小)
    smallRockPositionX = obstacleInitialPositionX;
    smallRockPositionY = obstacleResetPositionY;
    //木
    woodPositionX = obstacleInitialPositionX;
    woodPositionY = obstacleResetPositionY;
}


//UI
//ミニマップ
void MainScene::miniMapMoveUpdate(const float deltaTime)
{
    miniMapFishPositionX = bgMoveDistance / (bgResetPosition * lengthToGoal) * miniMapScaleX + miniMapFishScaleX/2;
}

//ゲージ
void MainScene::gaugeMoveUpdate()
{
    switch (gaugeState) {
    case firstStage:
        gaugeWidth = firstStage;
        break;
    case secondStage:
        gaugeWidth = secondStage;
        break;
    case thirdStage:
        gaugeWidth = thirdStage;
        break;
    case forthStage:
        gaugeWidth = forthStage;
        break;
    case fifthStage:
        gaugeWidth = fifthStage;
        break;
    }
}


//シーン遷移
NextScene MainScene::changeNextSceneUpdate(const float deltaTime)
{
    //クリア
    if (loopCount >= lengthToGoal)
    {
        sceneChangeBuffer += deltaTime;
        goal = true;
        if (sceneChangeBuffer >= goalAfterTime) {
            return NextScene::GameClearScene;
        }
    }
    //ゲームオーバー
    else if (gameOver)
    {
        return NextScene::GameOverScene;
    }

    return NextScene::Continue;
}


//当たり判定
//ベース当たり判定
bool MainScene::isCollisionDetectionBase(Rect& rect1, Rect& rect2) {

    if (rect1.left > rect2.right || rect1.right < rect2.left ||
        rect1.top > rect2.bottom || rect1.bottom < rect2.top) {
        return false;
    }
    return true;
}

//プレイヤーサイズ設定済み当たり判定
bool MainScene::isPlayerCollisionDetection(Rect& rect2)
{
    Rect goldfishRange = RectWH(playerPositionX, playerPositionY, smallFishScaleX, smallFishScaleY);
    Rect catfishRange = RectWH(playerPositionX, playerPositionY, mediumFishScaleX, mediumFishScaleY);
    Rect carpRange = RectWH(playerPositionX, playerPositionY, largeFishScaleX, largeFishScaleY);
    
    switch (playerStatus) {
    case smallFishState:
        return isCollisionDetectionBase(goldfishRange, rect2);
        break;
    case mediumFishState:
        return isCollisionDetectionBase(catfishRange, rect2);
        break;
    case largeFishState:
        return isCollisionDetectionBase(carpRange, rect2);
        break;
    }

    return false;
}

