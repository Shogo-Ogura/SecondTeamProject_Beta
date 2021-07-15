//
// ManualScene.cpp
//

#include "Base/pch.h"
#include "Base/dxtk.h"
#include "SceneFactory.h"
#include "DontDestroyOnLoad.h"

// Initialize member variables.
ManualScene::ManualScene() : dx9GpuDescriptor{}
{
    //�R���X�g���N�^�[
    //�ϐ��̏������i0�ɂ���j

}

// Initialize a variable and audio resources.
void ManualScene::Initialize()
{
    //�Q�[���V�[���̏�����
    //�ϐ����������i��̓I�Ȑ��l��ݒ肷��j
    //�T�E���h��ǂݍ��񂾂肷��

    std::random_device seed;
    randomEngine = std::mt19937(seed());
    randomDist = std::uniform_int_distribution<>(0, 359);

    //�w�i
    bgPositionX = 0.0f;
    bgMoveDistance = 0.0f;

    //���[�v�J�E���g
    bgLoopCount = 0;

    //�N���A����
    DontDestroy->clearTime = 0.0f;

    //�Q�[�W�i�K
    gaugeStatus = firstState;


    //�v���C���[
    //���W
    playerPositionX = playerInitialPositionX;
    playerPositionY = playerInitialPositionX;

    //����
    playerInertiaX = 0.0f;
    playerInertiaY = 0.0f;

    //�v���C���[�A�j���[�V����
    playerSpriteAnimationX = 0;
    playerSpriteAnimationY = 0;

    //�A�j���[�V�������x
    playerAnimationSpeed = 0.0f;

    //��������
    speedUpTime = 0.0f;

    //�����t���O
    speedUp = false;

    //���
    playerStatus = smallFishState;
    playerSpeedStatus = smallFishSpeedState;


    //�a�ʒu�ݒ�
    feedPositionSetting();
    feedGet = false;
    feedGetTime = 0.0f;


    //���ʒu�ݒ�
    birdPositionSetting();

    //��(��)�ʒu�ݒ�
    bigRockPositionSetting();

    //��(��)�ʒu�ݒ�
    smallRockPositionSetting();

    //�؈ʒu�ݒ�
    woodPositionSetting();

    //��Q�������蔻��t���O
    obstacleCollision = false;
    obstacleCollisionTime = 0.0f;
    

    //UI
    //�~�j�}�b�v
    miniMapWidth = 65.0f;
    miniMapHeight = miniMapFishScaleY;

    //�Q�[�W
    gaugeWidth = firstState;


    //�S�[��
    goalCollisionPositionX = goalCollisionInitialPositionX;
    goalCollisionPositionY = 0;

    goal = false;
    sceneChangeBuffer = 0.0f;

    //�Q�[���I�[�o�[
    gameOver = false;

}

// Allocate all memory the Direct3D and Direct2D resources.
void ManualScene::LoadAssets()
{
    //�O���t�B�b�N�̐ݒ������
    //Windows10�̓h���C�o���A�b�v�f�[�g�����Ɠǂݍ��ݒ������K�v�ɂȂ�

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


    // �O���t�B�b�N���\�[�X�̏���������

    //�w�i
    bgSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bgSprite.png");


    //�v���C���[
    //��
    fishSprite[smallFishState] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"smallFishSprite.png");

    //��
    fishSprite[mediumFishState] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"mediumFishSprite.png");

    //��
    fishSprite[largeFishState] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"largeFishSprite.png");


    //�a(�A�C�e��)
    for (int f = 0; f < feedMaxAmount; ++f)
    {
        feedSprite[f] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"feedSprite.png");
    }

    //��Q��
    //��
    for (int b = 0; b < birdMaxAmount; ++b) 
    {
        birdTestSprite[b] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"birdSprite.png");
    }

    //��(��)
    for (int r = 0; r < bigRockMaxAmount; ++r) 
    {
        bigRockSprite[r] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bigRockSprite.png");
    }

    //��(��)
    for (int s = 0; s < smallRockMaxAmount; ++s) 
    {
        smallRockSprite[s] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"smallRockSprite.png");
    }

    //��
    for (int w = 0; w < woodMaxAmount; ++w) 
    {
        woodSprite[w] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"woodSprite.png");
    }


    //UI
    //�~�j�}�b�v
    miniMapSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"miniMapSprite.png");
    miniMapBgSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"miniMapBgSprite.png");
    miniMapFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"miniMapFishTestSprite.png");

    //�Q�[�W
    speedGaugeSprite[0]= DX9::Sprite::CreateFromFile(DXTK->Device9, L"speedGaugeSprite1.png");
    speedGaugeSprite[1] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"speedGaugeSprite2.png");
    speedGaugeSprite[2] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"speedGaugeSprite3.png");
    speedGaugeSprite[3] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"speedGaugeSprite4.png");
    speedGaugeSprite[4] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"speedGaugeSprite5.png");


    //�S�[��
    goalStringSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"goalSprite.png");
    goalLineSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"goalLineSprite.png");


    //�f�o�b�O�p
    playerStatusFont = DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"�l�r �o����", 20);
    gaugeStageFont = DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"�l�r �o����", 20);

}

// Releasing resources required for termination.
void ManualScene::Terminate()
{
    DXTK->ResetAudioEngine();
    DXTK->WaitForGpu();

    // TODO: Add your Termination logic here.
    //�Q�[��(�V�[��)�I�����ɉ����������ꍇ�Ɏg��
}

// Direct3D resource cleanup.
void ManualScene::OnDeviceLost()
{
    //�h���C�o�[�A�b�v�f�[�g�����o�����ꍇ�Ɏg��
    //�ǂݍ��݂Ȃ����O�ɉ����������ꍇ�ɉ�������
}

// Restart any looped sounds here
void ManualScene::OnRestartSound()
{
    //�T�E���h�f�o�C�X���؂�ւ��ƃT�E���h���S����~����
    //BGM�Ȃǂ��ăX�^�[�g������
}

// Updates the scene.
NextScene ManualScene::Update(const float deltaTime)
{
    // If you use 'deltaTime', remove it.
    UNREFERENCED_PARAMETER(deltaTime);

    // TODO: Add your game logic here.
    //�Q�[���𓮂����v���O�������L�q����

    //�w�i
    bgLoopUpdate(deltaTime);

    //�X�N���[�����x
    bgMoveSpeedUpdate(deltaTime);

    //�X�N���[�����x����
    setBgScrollSpeed();


    //�N���A���Ԍv��
    countPlayTimeUpdate(deltaTime);


    //��ԑJ��
    gaugeStateUpdate(deltaTime);

    //���G����
    invalidTime(deltaTime);

    //��ԑJ�ڊ���
    auto old_state = playerStatus;
    if (old_state != gaugePlayerStateAssignUpdate()) {
        playerAnimationSpeed = 0;
        playerSpriteAnimationX = 0;
    }

    //�v���C���[
 //�A�j���[�V����
    playerAnimationUpdate(deltaTime);

    //�ړ��\�͈�
    playerMoveRangeUpdate();

    //�����ړ�
    playerAutoMoveUpdate(deltaTime);

    //�L�[�{�[�h����
    playerControlKeyboardUpdate(deltaTime);

    //�p�b�h����
    playerControlGamepadUpdate(deltaTime);


    //�a(�A�C�e��)
    //�ړ�
    feedMoveUpdate(deltaTime);

    //��Q��
    //�ړ�
    obstacleMoveUpdate(deltaTime);


    //UI
    //�~�j�}�b�v
    miniMapMoveUpdate(deltaTime);

    //�Q�[�W
    gaugeMoveUpdate();

    //�S�[��
    goalCollisionDetectionUpdate();

     
    //�V�[���J��
    return changeNextSceneUpdate(deltaTime);

}

// Draws the scene.
void ManualScene::Render()
{
    // TODO: Add your rendering code here.
    //�`��̃v���O����������

    DXTK->Direct3D9->Clear(DX9::Colors::RGBA(0, 0, 0, 255));  // �菇1

    DXTK->Direct3D9->BeginScene();    // �菇2
    DX9::SpriteBatch->Begin();        // �菇4

    // (������2D�`��̏���������)     // �菇5

    //�w�i
    DX9::SpriteBatch->DrawSimple(bgSprite.Get(), SimpleMath::Vector3(bgPositionX, 0, 10));


    //�v���C���[
    DX9::SpriteBatch->DrawSimple(
        fishSprite[playerStatus].Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -(playerPositionY + fishScaleY[playerStatus]) / zPositionWidth),
        RectWH(playerSpriteAnimationX, playerSpriteAnimationY, fishScaleX[playerStatus], fishScaleY[playerStatus])
    );

    
    //�a(�A�C�e��)
    for (int f = 0; f < feedMaxAmount; ++f)
    {
        DX9::SpriteBatch->DrawSimple(feedSprite[f].Get(), SimpleMath::Vector3(feedPositionX[f], feedPositionY[f], 0));
    }


    //��Q��
    //��
    for (int b = 0; b < birdMaxAmount; ++b) 
    {
        DX9::SpriteBatch->DrawSimple(birdTestSprite[b].Get(), SimpleMath::Vector3(birdPositionX[b], birdPositionY[b], -(birdPositionY[b] + birdScaleY) / zPositionWidth));
    }

    //��(��)
    for (int r = 0; r < bigRockMaxAmount; ++r)
    {
        DX9::SpriteBatch->DrawSimple(bigRockSprite[r].Get(), SimpleMath::Vector3(bigRockPositionX[r], bigRockPositionY[r], -(bigRockPositionY[r] + bigRockScaleY) / zPositionWidth));
    }

    //��(��)
    for (int s = 0; s < smallRockMaxAmount; ++s) 
    {
        DX9::SpriteBatch->DrawSimple(smallRockSprite[s].Get(), SimpleMath::Vector3(smallRockPositionX[s], smallRockPositionY[s], -(smallRockPositionY[s] + smallRockScaleY) / zPositionWidth));
    }

    //��
    for (int w = 0; w < woodMaxAmount; ++w)
    {
        DX9::SpriteBatch->DrawSimple(woodSprite[w].Get(), SimpleMath::Vector3(woodPositionX[w], woodPositionY[w], -(woodPositionY[w] + woodScaleY) / zPositionWidth));
    }


    //UI
    //�~�j�}�b�v
    DX9::SpriteBatch->DrawSimple(miniMapSprite.Get(),
        SimpleMath::Vector3(miniMapPositionX, miniMapPositionY, -5),
        RectWH(0.0f, 0.0f, miniMapWidth, miniMapHeight)
    );
    DX9::SpriteBatch->DrawSimple(miniMapBgSprite.Get(), SimpleMath::Vector3(miniMapPositionX, miniMapPositionY, -4));

    //�Q�[�W
    //�{��
    /*DX9::SpriteBatch->DrawSimple(speedGaugeSprite.Get(), 
        SimpleMath::Vector3(gaugePositionX, gaugePositionY, 8),
        RectWH(0.0f, 0.0f, gaugeWidth, 100.0f)
    );*/

    DX9::SpriteBatch->DrawSimple(speedGaugeSprite[gaugeStatus].Get(), SimpleMath::Vector3(gaugePositionX, gaugePositionY, 8));

    ////�g
    //DX9::SpriteBatch->DrawSimple(gaugeBgTestSprite.Get(), SimpleMath::Vector3(gaugePositionX, gaugePositionY, 9));


    //�S�[��
    //����
    if (goal)
        DX9::SpriteBatch->DrawSimple(goalStringSprite.Get(), SimpleMath::Vector3(goalStringSpritePositionX, goalStringSpritePositionY, -10));

    //�S�[�����C��
    DX9::SpriteBatch->DrawSimple(goalLineSprite.Get(), SimpleMath::Vector3(goalCollisionPositionX, goalCollisionPositionY + 250, 0));


    //�f�o�b�O�p
    /*DX9::SpriteBatch->DrawString
    (
        playerStatusFont.Get(), SimpleMath::Vector2(0, 670),
        DX9::Colors::RGBA(0, 0, 0, 255), L"goalCollisionPositionX:%d", (int)goalCollisionPositionX
    );*/

    /*DX9::SpriteBatch->DrawString(
        gaugeStageFont.Get(), SimpleMath::Vector2(420.0f, 670.0f),
        DX9::Colors::RGBA(0, 0, 0, 255), L"goalCollisionPositionY:%d", (int)goalCollisionPositionY
    );*/

    /*DX9::SpriteBatch->DrawString(
        gaugeStageFont.Get(), SimpleMath::Vector2(800.0f, 670.0f),
        DX9::Colors::RGBA(0, 0, 0, 255), L"goal:%d", (int)goal
    );*/

    /*DX9::SpriteBatch->DrawString(
        gaugeStageFont.Get(), SimpleMath::Vector2(900.0f, 670.0f),
        DX9::Colors::RGBA(0, 0, 0, 255), L"sceneChangeBuffer:%d", (int)sceneChangeBuffer
    );*/


    DX9::SpriteBatch->End();          // �菇6
    DXTK->Direct3D9->EndScene();      // �菇7

    DXTK->Direct3D9->UpdateResource();

    DXTK->ResetCommand();
    DXTK->ClearRenderTarget(Colors::CornflowerBlue);

    const auto heapes = descriptorHeap->Heap();
    DXTK->CommandList->SetDescriptorHeaps(1, &heapes);

    spriteBatch->Begin(DXTK->CommandList);
    spriteBatch->Draw(
        dx9GpuDescriptor,
        XMUINT2(1280, 720),
        SimpleMath::Vector2(0.0f, 0.0f)
    );
    spriteBatch->End();

    DXTK->Direct3D9->WaitUpdate();
    DXTK->ExecuteCommandList();
}

//Update���֐��̒�`

//�w�i
//�X�N���[�����x
void ManualScene::bgMoveSpeedUpdate(const float deltaTime)
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
            bgPositionX -= topFishSpeed * deltaTime;
            bgMoveDistance += topFishSpeed * deltaTime;
            if (speedUpTime >= 2.0f)
            {
                playerSpeedStatus = largeFishSpeedState;
                speedUp = false;
            }
            break;
        }
    }
}

//�w�i���[�v
void ManualScene::bgLoopUpdate(const float deltaTime)
{
    if (bgPositionX <= -bgResetPosition) {
        bgPositionX = 0;
        ++bgLoopCount;
    }
}

//�X�N���[�����x�ύX
void ManualScene::setBgScrollSpeed()
{
    switch (playerStatus) {
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


//��ԑJ�ڊ���
int ManualScene::gaugePlayerStateAssignUpdate()
{
    if (gaugeStatus == firstState || gaugeStatus == secondState) {
        playerStatus = smallFishState;
    }
    else if (gaugeStatus == thirdState || gaugeStatus == forthState) {
        playerStatus = mediumFishState;
    }
    else if (gaugeStatus == fifthState) {
        playerStatus = largeFishState;
    }

    return playerStatus;
}


//�N���A���Ԍv��
void ManualScene::countPlayTimeUpdate(const float deltaTime)
{
    if (!goal)
        DontDestroy->clearTime += deltaTime;
}


//��ԑJ��
int ManualScene::gaugeStateUpdate(const float deltaTime)
{
    switch (gaugeStatus) {
    case firstState:
        if (isFeedCollisionDetectionUpdate() && (feedGet == false))
        {
            gaugeStatus = secondState;
            feedGet = true;
        }
        else if (
            (isBirdCollisionDetectionUpdate()||isBigRockCollisionDetectionUpdate()
                ||isSmallRockCollisionDetectionUpdate()||isWoodCollisionDetectionUpdate()) 
            && (obstacleCollision == false)
            )
        {
            gameOver = true;
            obstacleCollision = true;
        }
        break;
    case secondState:
        if (isFeedCollisionDetectionUpdate() && (feedGet == false))
        {
            gaugeStatus = thirdState;
            feedGet = true;
        }
        else if (
            (isBirdCollisionDetectionUpdate() || isBigRockCollisionDetectionUpdate()
                || isSmallRockCollisionDetectionUpdate() || isWoodCollisionDetectionUpdate())
            && (obstacleCollision == false)
            )
        {
            gaugeStatus = firstState;
            obstacleCollision = true;
        }
        break;
    case thirdState:
        if (isFeedCollisionDetectionUpdate() && (feedGet == false))
        {;
            gaugeStatus = forthState;
            feedGet = true;
        }
        else if (
            (isBirdCollisionDetectionUpdate() || isBigRockCollisionDetectionUpdate()
                || isSmallRockCollisionDetectionUpdate() || isWoodCollisionDetectionUpdate())
            && (obstacleCollision == false)
            )
        {
            gaugeStatus = secondState;
            obstacleCollision = true;
        }
        break;
    case forthState:
        if (isFeedCollisionDetectionUpdate() && (feedGet == false))
        {
            gaugeStatus = fifthState;
            feedGet = true;
        }
        else if (
            (isBirdCollisionDetectionUpdate() || isBigRockCollisionDetectionUpdate()
                || isSmallRockCollisionDetectionUpdate() || isWoodCollisionDetectionUpdate())
            && (obstacleCollision == false)
            )
        {
            gaugeStatus = thirdState;
            obstacleCollision = true;
        }
        break;
    case fifthState:
        if (isFeedCollisionDetectionUpdate() && (feedGet == false))
        {
            speedUp = true;
            feedGet = true;
        }
        else if (
            (isBirdCollisionDetectionUpdate() || isBigRockCollisionDetectionUpdate()
                || isSmallRockCollisionDetectionUpdate() || isWoodCollisionDetectionUpdate())
            && (obstacleCollision == false)
            )
        {
            speedUpTime = 0.0f;
            gaugeStatus = forthState;
            obstacleCollision = true;
        }
        break;
    }

    return gaugeStatus;
}

//���G����
void ManualScene::invalidTime(const float deltaTime)
{
    //�a
    if (feedGet) 
    {
        feedGetTime += deltaTime;
        if (feedGetTime >= 1.0f)
        {
            feedGetTime = 0.0f;
            feedGet = false;
        }
    }

    //��Q��
    if (obstacleCollision)
    {
        obstacleCollisionTime += deltaTime;
        if (obstacleCollisionTime >= 1.0f)
        {
            obstacleCollisionTime = 0.0f;
            obstacleCollision = false;
        }
    }
}


//�v���C���[
//�A�j���[�V����
void ManualScene::playerAnimationUpdate(const float deltaTime)
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

//�ړ��\�͈�
void ManualScene::playerMoveRangeUpdate()
{
    //��
    if (playerPositionY <= playerMoveRangeTop)
    {
        playerPositionY = playerMoveRangeTop;
    }
    //��
    if (playerPositionX <= playerMoveRangeLeft-350)
    {
        gameOver = true;
    }
    //�E
    if (playerPositionX >= playerMoveRangeRight - fishScaleX[playerStatus])
    {
        playerPositionX = playerMoveRangeRight - fishScaleX[playerStatus];
    }
    //��
    if (playerPositionY >= playerMoveRangeBottom - fishScaleY[playerStatus])
    {
        playerPositionY = playerMoveRangeBottom - fishScaleY[playerStatus];
    }
}

//�����ړ�
void ManualScene::playerAutoMoveUpdate(const float deltaTime)
{
    if (!goal)
        playerPositionX -= playerAutoMoveSpeed * deltaTime;
}

//�L�[�{�[�h����
void ManualScene::playerControlKeyboardUpdate(const float deltaTime)
{
    if (!goal) {
        //���E
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

        //�㉺
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

//�p�b�h����
void ManualScene::playerControlGamepadUpdate(const float deltaTime)
{
    if (!goal) {
        //�\���L�[
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

        //�X�e�B�b�N
        /*playerPositionX += gamePadPlayerMoveSpeedX * DXTK->GamePadState[0].thumbSticks.leftX * deltaTime;
        playerPositionY -= gamePadPlayerMoveSpeedY * DXTK->GamePadState[0].thumbSticks.leftY * deltaTime;*/
    }
}


//�a(�A�C�e��)
//�a�ʒu�ݒ�
void ManualScene::feedPositionSetting()
{
    feedPositionX[0] = 1840;
    feedPositionY[0] = 480;

    feedPositionX[1] = 2280;
    feedPositionY[1] = 600;

    feedPositionX[2] = 2710;
    feedPositionY[2] = 550;

    feedPositionX[3] = 3160;
    feedPositionY[3] = 300;

    feedPositionX[4] = 4110;
    feedPositionY[4] = 420;

    feedPositionX[5] = 4360;
    feedPositionY[5] = 640;

    feedPositionX[6] = 4660;
    feedPositionY[6] = 330;

    feedPositionX[7] = 5920;
    feedPositionY[7] = 580;

    feedPositionX[8] = 6120;
    feedPositionY[8] = 320;

    feedPositionX[9] = 6680;
    feedPositionY[9] = 320;

    feedPositionX[10] = 6830;
    feedPositionY[10] = 680;

    feedPositionX[11] = 8380;
    feedPositionY[11] = 550;

    feedPositionX[12] = 8680;
    feedPositionY[12] = 310;

    feedPositionX[13] = 11140;
    feedPositionY[13] = 380;

    for (int i = 0; i < _countof(feedPositionY); ++i) {
        feedBasePosY[i] = feedPositionY[i];
        feedTheta   [i] = (float)randomDist(randomEngine);
    }
}

//�ړ�
void ManualScene::feedMoveUpdate(const float deltaTime)
{
    if (!goal) {
        switch (playerSpeedStatus) {
            case smallFishSpeedState:
            {
                for (int f = 0; f < feedMaxAmount; ++f)
                    feedPositionX[f] -= feedMoveSpeed * deltaTime;
                break;
            }
            case mediumFishSpeedState:
            {
                for (int f = 0; f < feedMaxAmount; ++f)
                    feedPositionX[f] -= (feedMoveSpeed + mediumFishSpeed) * deltaTime;
                break;
            }
            case largeFishSpeedState:
            {
                for (int f = 0; f < feedMaxAmount; ++f)
                    feedPositionX[f] -= (feedMoveSpeed + largeFishSpeed) * deltaTime;
                break;
            }
            case speedUpState:
            {
                for (int f = 0; f < feedMaxAmount; ++f)
                    feedPositionX[f] -= (feedMoveSpeed + topFishSpeed) * deltaTime;
                break;
            }
        }

        const float DELTA_ANGLE = XM_PI * deltaTime;
        for (int i = 0; i < _countof(feedBasePosY); ++i) {
            feedPositionY[i]  = feedBasePosY[i] + sin(feedTheta[i]) * 10.0f;
            feedTheta    [i] += DELTA_ANGLE;
            if (feedTheta[i] >= XM_2PI)
                feedTheta[i] -= XM_2PI;
        }
    }
}

//�a�����蔻��
bool ManualScene::isFeedCollisionDetectionUpdate()
{
    for (int f = 0; f < feedMaxAmount; ++f)
    {
        if (isPlayerCollisionDetection(RectWH(feedPositionX[f], feedPositionY[f], feedScaleX, feedScaleY)))
            return true;
    }
    return false;
}


//��Q��
//���ʒu�ݒ�
void ManualScene::birdPositionSetting()
{
    birdPositionX[0] = 4760;
    birdPositionY[0] = 600;

    birdPositionX[1] = 8780;
    birdPositionY[1] = 600;

    birdPositionX[2] = 11140;
    birdPositionY[2] = 640;
}

//��(��)�ʒu�ݒ�
void ManualScene::bigRockPositionSetting()
{
    bigRockPositionX[0] = 2200;
    bigRockPositionY[0] = 360;

    bigRockPositionX[1] = 2450;
    bigRockPositionY[1] = 360;

    bigRockPositionX[2] = 2680;
    bigRockPositionY[2] = 360;

    bigRockPositionX[3] = 3160;
    bigRockPositionY[3] = 600;

    bigRockPositionX[4] = 3730;
    bigRockPositionY[4] = 360;

    bigRockPositionX[5] = 3960;
    bigRockPositionY[5] = 360;

    bigRockPositionX[6] = 5950;
    bigRockPositionY[6] = 360;

    bigRockPositionX[7] = 7380;
    bigRockPositionY[7] = 360;

    bigRockPositionX[8] = 7880;
    bigRockPositionY[8] = 600;

    bigRockPositionX[9] = 8300;
    bigRockPositionY[9] = 360;

    bigRockPositionX[10] = 9610;
    bigRockPositionY[10] = 600;

    bigRockPositionX[11] = 10440;
    bigRockPositionY[11] = 600;

    bigRockPositionX[12] = 10840;
    bigRockPositionY[12] = 360;

    bigRockPositionX[13] = 11340;
    bigRockPositionY[13] = 600;

    bigRockPositionX[14] = 11720;
    bigRockPositionY[14] = 600;
}

//��(��)�ʒu�ݒ�
void ManualScene::smallRockPositionSetting()
{
    smallRockPositionX[0] = 560 + 1280;
    smallRockPositionY[0] = 260;

    smallRockPositionX[1] = 560 + 1280;
    smallRockPositionY[1] = 700;

    smallRockPositionX[2] = 420 + 3840;
    smallRockPositionY[2] = 480;

    smallRockPositionX[3] = 520 + 3840;
    smallRockPositionY[3] = 480;

    smallRockPositionX[4] = 620 + 3840;
    smallRockPositionY[4] = 480;

    smallRockPositionX[5] = 720 + 3840;
    smallRockPositionY[5] = 480;

    smallRockPositionX[6] = 820 + 3840;
    smallRockPositionY[6] = 480;

    smallRockPositionX[7] = 920 + 3840;
    smallRockPositionY[7] = 500;
    
    smallRockPositionX[8] = 1020 + 3840;
    smallRockPositionY[8] = 500;

    smallRockPositionX[9] = 20 + 5120;
    smallRockPositionY[9] = 480;

    smallRockPositionX[10] = 120 + 5120;
    smallRockPositionY[10] = 480;

    smallRockPositionX[11] = 220 + 5120;
    smallRockPositionY[11] = 480;

    smallRockPositionX[12] = 320 + 5120;
    smallRockPositionY[12] = 480;

    smallRockPositionX[13] = 280 + 6400;
    smallRockPositionY[13] = 480;

    smallRockPositionX[14] = 380 + 6400;
    smallRockPositionY[14] = 480;

    smallRockPositionX[15] = 480 + 6400;
    smallRockPositionY[15] = 500;

    smallRockPositionX[16] = 580 + 6400;
    smallRockPositionY[16] = 270;

    smallRockPositionX[17] = 580 + 6400;
    smallRockPositionY[17] = 500;

    smallRockPositionX[18] = 1100 + 6400;
    smallRockPositionY[18] = 270;

    smallRockPositionX[19] = 1160 + 6400;
    smallRockPositionY[19] = 700;

    smallRockPositionX[20] = 100 + 8960;
    smallRockPositionY[20] = 260;

    smallRockPositionX[21] = 100 + 8960;
    smallRockPositionY[21] = 700;

    smallRockPositionX[22] = 200 + 8960;
    smallRockPositionY[22] = 280;

    smallRockPositionX[23] = 200 + 8960;
    smallRockPositionY[23] = 680;

    smallRockPositionX[24] = 300 + 8960;
    smallRockPositionY[24] = 300;

    smallRockPositionX[25] = 300 + 8960;
    smallRockPositionY[25] = 640;

    smallRockPositionX[26] = 900 + 8960;
    smallRockPositionY[26] = 260;

    smallRockPositionX[27] = 900 + 8960;
    smallRockPositionY[27] = 680;

    smallRockPositionX[28] = 1050 + 8960;
    smallRockPositionY[28] = 280;

    smallRockPositionX[29] = 600 + 11520;
    smallRockPositionY[29] = 260;

    smallRockPositionX[30] = 600 + 11520;
    smallRockPositionY[30] = 530;

    smallRockPositionX[31] = 600 + 11520;
    smallRockPositionY[31] = 700;

    smallRockPositionX[32] = 800 + 11520;
    smallRockPositionY[32] = 300;

    smallRockPositionX[33] = 800 + 11520;
    smallRockPositionY[33] = 660;
}

//�؈ʒu�ݒ�
void ManualScene::woodPositionSetting()
{
    woodPositionX[0] = 1170 + 5120;
    woodPositionY[0] = 600;

    woodPositionX[1] = 120 + 6400;
    woodPositionY[1] = 600;
}

//��Q���ړ� 
void ManualScene::obstacleMoveUpdate(const float deltaTime)
{
    if (!goal) {
        switch (playerSpeedStatus) {
        case smallFishSpeedState:
            for (int b = 0; b < birdMaxAmount; ++b) 
            {
                birdPositionX[b] -= obstacleMoveSpeed * deltaTime;
            }
            for (int r = 0; r < bigRockMaxAmount; ++r)
            {
                bigRockPositionX[r] -= obstacleMoveSpeed * deltaTime;
            }
            for (int s = 0; s < smallRockMaxAmount; ++s)
            {
                smallRockPositionX[s] -= obstacleMoveSpeed * deltaTime;
            }
            for (int w = 0; w < woodMaxAmount; ++w)
            {
                woodPositionX[w] -= obstacleMoveSpeed * deltaTime;
            }
            break;
        case mediumFishSpeedState:
            for (int b = 0; b < birdMaxAmount; ++b)
            {
                birdPositionX[b] -= (obstacleMoveSpeed + mediumFishSpeed) * deltaTime;
            }
            for (int r = 0; r < bigRockMaxAmount; ++r)
            {
                bigRockPositionX[r] -= (obstacleMoveSpeed + mediumFishSpeed) * deltaTime;
            }
            for (int s = 0; s < smallRockMaxAmount; ++s)
            {
                smallRockPositionX[s] -= (obstacleMoveSpeed + mediumFishSpeed) * deltaTime;
            }
            for (int w = 0; w < woodMaxAmount; ++w)
            {
                woodPositionX[w] -= (obstacleMoveSpeed + mediumFishSpeed) * deltaTime;
            }
            break;
        case largeFishSpeedState:
            for (int b = 0; b < birdMaxAmount; ++b)
            {
                birdPositionX[b] -= (obstacleMoveSpeed + largeFishSpeed) * deltaTime;
            }
            for (int r = 0; r < bigRockMaxAmount; ++r)
            {
                bigRockPositionX[r] -= (obstacleMoveSpeed + largeFishSpeed) * deltaTime;
            }
            for (int s = 0; s < smallRockMaxAmount; ++s)
            {
                smallRockPositionX[s] -= (obstacleMoveSpeed + largeFishSpeed) * deltaTime;
            }
            for (int w = 0; w < woodMaxAmount; ++w)
            {
                woodPositionX[w] -= (obstacleMoveSpeed + largeFishSpeed) * deltaTime;
            }
            break;
        case speedUpState:
            for (int b = 0; b < birdMaxAmount; ++b)
            {
                birdPositionX[b] -= (obstacleMoveSpeed + topFishSpeed) * deltaTime;
            }
            for (int r = 0; r < bigRockMaxAmount; ++r)
            {
                bigRockPositionX[r] -= (obstacleMoveSpeed + topFishSpeed) * deltaTime;
            }
            for (int s = 0; s < smallRockMaxAmount; ++s)
            {
                smallRockPositionX[s] -= (obstacleMoveSpeed + topFishSpeed) * deltaTime;
            }
            for (int w = 0; w < woodMaxAmount; ++w)
            {
                woodPositionX[w] -= (obstacleMoveSpeed + topFishSpeed) * deltaTime;
            }
            break;
        }
    }
}

//�������蔻��
bool ManualScene::isBirdCollisionDetectionUpdate()
{
    for (int b = 0; b < birdMaxAmount; ++b) 
    {
        if (isPlayerCollisionDetection(RectWH(birdPositionX[b], birdPositionY[b], birdScaleX, birdScaleY)))
            return true;
    }
    
    return false;
}

//��(��)
bool ManualScene::isBigRockCollisionDetectionUpdate()
{
    for (int r = 0; r < bigRockMaxAmount; ++r) 
    {
        if (isPlayerCollisionDetection(RectWH(bigRockPositionX[r], bigRockPositionY[r] + 125, bigRockScaleX, 85)))
            return true;
    }

        return false;
}

//��(��)
bool ManualScene::isSmallRockCollisionDetectionUpdate()
{
    for (int s = 0; s < smallRockMaxAmount; ++s)
    {
        if (isPlayerCollisionDetection(RectWH(smallRockPositionX[s], smallRockPositionY[s], smallRockScaleX, smallRockScaleY)))
            return true;
    }
    
    return false;
}

//��
bool ManualScene::isWoodCollisionDetectionUpdate()
{
    for (int w = 0; w < woodMaxAmount; ++w)
    {
        if (isPlayerCollisionDetection(RectWH(woodPositionX[w], woodPositionY[w], woodScaleX, woodScaleY)))
            return true;
    }
    
    return false;
}


//UI
//�~�j�}�b�v
void ManualScene::miniMapMoveUpdate(const float deltaTime)
{
    miniMapWidth = (80+(bgMoveDistance / (bgResetPosition * lengthToGoal) * 475));
}

//�Q�[�W
void ManualScene::gaugeMoveUpdate()
{
    switch (gaugeStatus) {
    case firstState:
        gaugeWidth = firstState;
        break;
    case secondState:
        gaugeWidth = secondState;
        break;
    case thirdState:
        gaugeWidth = thirdState;
        break;
    case forthState:
        gaugeWidth = forthState;
        break;
    case fifthState:
        gaugeWidth = fifthState;
        break;
    }
}

//�S�[��
void ManualScene::goalCollisionDetectionUpdate()
{
    goalCollisionPositionX = smallRockPositionX[33] + 300;

    if (isPlayerCollisionDetection(RectWH(goalCollisionPositionX, goalCollisionPositionY, 50, 720)))
        goal = true;
}


//�V�[���J��
NextScene ManualScene::changeNextSceneUpdate(const float deltaTime)
{
    //�N���A
    /*if (bgLoopCount >= lengthToGoal)
    {
        sceneChangeBuffer += deltaTime;
        goal = true;
        if (sceneChangeBuffer >= goalAfterTime) {
            return NextScene::GameClearScene;
        }
    }*/
    if (goal)
    {
        sceneChangeBuffer += deltaTime;
        if (sceneChangeBuffer >= goalAfterTime) {
            return NextScene::GameClearScene;
        }
    }
    //�Q�[���I�[�o�[
    else if (gameOver)
    {
        return NextScene::GameOverScene;
    }

    return NextScene::Continue;
}


//�����蔻��
//�x�[�X�����蔻��
bool ManualScene::isCollisionDetectionBase(Rect& rect1, Rect& rect2) {

    if (rect1.left > rect2.right || rect1.right < rect2.left ||
        rect1.top > rect2.bottom || rect1.bottom < rect2.top) {
        return false;
    }
    return true;
}

//�v���C���[�T�C�Y�ݒ�ςݓ����蔻��
bool ManualScene::isPlayerCollisionDetection(Rect& rect2)
{
    Rect smallFishRange = RectWH(playerPositionX, playerPositionY, smallFishScaleX, smallFishScaleY);
    Rect mediumFishRange = RectWH(playerPositionX, playerPositionY, mediumFishScaleX, mediumFishScaleY);
    Rect largeFishRange = RectWH(playerPositionX, playerPositionY, largeFishScaleX, largeFishScaleY);

    switch (playerStatus) {
    case smallFishState:
        return isCollisionDetectionBase(smallFishRange, rect2);
        break;
    case mediumFishState:
        return isCollisionDetectionBase(mediumFishRange, rect2);
        break;
    case largeFishState:
        return isCollisionDetectionBase(largeFishRange, rect2);
        break;
    }

    return false;
}

