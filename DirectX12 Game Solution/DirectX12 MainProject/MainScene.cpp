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
    //�R���X�g���N�^�[
    //�ϐ��̏������i0�ɂ���j

}

// Initialize a variable and audio resources.
void MainScene::Initialize()
{
    //�Q�[���V�[���̏�����
    //�ϐ����������i��̓I�Ȑ��l��ݒ肷��j
    //�T�E���h��ǂݍ��񂾂肷��
    
    //�w�i
    bgPositionX = 0.0f;

    //���[�v�J�E���g
    loopCount = 0;

    //�N���A����
    DontDestroy->clearTime = 0.0f;

    //�Q�[�W�i�K
    gaugeState = firstStage;


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


    //�a(�A�C�e��)
    //�����ʒu
    feedPositionX = feedInitialPositionX;
    feedPositionY = feedInitialPositionY;

    //�a�ʒu���Z�b�g
    std::random_device rnd_dev;
    randomEngine = std::mt19937(rnd_dev());
    randomFeedPositionY = std::uniform_real_distribution<float>(feedAppearanceTop, feedAppearanceBottom);


    //��Q��
    //��Q�������ʒu
    //��
    birdPositionX = obstacleInitialPositionX;
    birdPositionY = obstacleInitialPositionY;

    //��(��)
    bigRockPositionX = obstacleInitialPositionX;
    bigRockPositionY = obstacleInitialPositionY;

    //��(��)
    smallRockPositionX = obstacleInitialPositionX;
    smallRockPositionY = obstacleInitialPositionY;

    //��
    woodPositionX = obstacleInitialPositionX;
    woodPositionY = obstacleInitialPositionY;

    //��Q�����
    obstacleStatus = bigRockState;

    //�����_�����Z�b�g���W
    randomObstaclePositionY = std::uniform_real_distribution<float>(obstacleAppearanceTop, obstacleAppearanceBottom);
    obstacleResetPositionY = randomObstaclePositionY(randomEngine);

    //�����_��
    randomObstacle = std::uniform_int_distribution<int>(1, 4);


    //UI
    //�~�j�}�b�v
    miniMapFishPositionX = miniMapPositionX - miniMapFishScaleX / 2;
    miniMapFishPositionY = miniMapPositionY - miniMapFishScaleY / 2;
    
    //�Q�[�W
    gaugeWidth = firstStage;


    //�S�[��
    goalSpritePositionX = 400.0f;
    goalSpritePositionY = 300.0f;
    goal = false;
    sceneChangeBuffer = 0.0f;

    //�Q�[���I�[�o�[
    gameOver = false;


    
}

// Allocate all memory the Direct3D and Direct2D resources.
void MainScene::LoadAssets()
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
    //bgTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bgTestSprite.png");
    bgTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bgSprite02.png");
    
    
    //�v���C���[
    //��
    //smallFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"smallFishSprite.png");
    fishTestSprite[0] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"smallFishSprite.png");

    //��
    //mediumFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"catfishTestSprite.png");
    mediumFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"mediumFishSprite.png");
    fishTestSprite[1] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"mediumFishSprite.png");

    //��
    //largeFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"largeFishSprite1.png");
    fishTestSprite[2] = DX9::Sprite::CreateFromFile(DXTK->Device9, L"largeFishSprite1.png");


    //�a(�A�C�e��)
    //feedTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"wormTestSprite.png");
    feedTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"wormSprite.png");


    //��Q��
    //��
    //birdTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"kingfisherTestSprite.png");
    birdTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"birdSprite.png");

    //��(��)
    bigRockTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bigRockTestSprite.png");

    //��(��)
    smallRockTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"smallRockTestSprite.png");

    //��
    woodTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"woodTestSprite.png");


    //UI
    //�~�j�}�b�v
    miniMapSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"miniMapSprite.png");
    miniMapFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"miniMapFishTestSprite.png");
    
    //�Q�[�W
    gaugeTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"gaugeTestSprite.png");
    gaugeBgTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"gaugeBgTestSprite.png");


    //�S�[��
    goalSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"goalTestSprite.png");


    //�f�o�b�O�p
    playerStatusFont = DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"�l�r �o����", 20);
    gaugeStageFont = DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"�l�r �o����", 20);

}

// Releasing resources required for termination.
void MainScene::Terminate()
{
    DXTK->ResetAudioEngine();
    DXTK->WaitForGpu();

    // TODO: Add your Termination logic here.
    //�Q�[��(�V�[��)�I�����ɉ����������ꍇ�Ɏg��
}

// Direct3D resource cleanup.
void MainScene::OnDeviceLost()
{
    //�h���C�o�[�A�b�v�f�[�g�����o�����ꍇ�Ɏg��
    //�ǂݍ��݂Ȃ����O�ɉ����������ꍇ�ɉ�������
}

// Restart any looped sounds here
void MainScene::OnRestartSound()
{
    //�T�E���h�f�o�C�X���؂�ւ��ƃT�E���h���S����~����
    //BGM�Ȃǂ��ăX�^�[�g������
}

// Updates the scene.
NextScene MainScene::Update(const float deltaTime)
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

    //��ԑJ�ڊ���
    auto old_state = playerStatus;
    if (old_state != gaugePlayerStateAssignUpdate()) {
        playerAnimationSpeed   = 0;
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

    //�a���[�v
    feedLoopUpdate();

    //��Q��
    //�ړ�
    obstacleMoveUpdate(deltaTime);

    //��Q�����[�v
    obstacleLoopUpdate();


    //UI
    //�~�j�}�b�v
    //miniMapMoveUpdate(deltaTime);
    
    //�Q�[�W
    gaugeMoveUpdate();


    //�V�[���J��
    return changeNextSceneUpdate(deltaTime);

}

// Draws the scene.
void MainScene::Render()
{
    // TODO: Add your rendering code here.
    //�`��̃v���O����������
    
    DXTK->Direct3D9->Clear(DX9::Colors::RGBA(0, 0, 0, 255));  // �菇1

    DXTK->Direct3D9->BeginScene();    // �菇2
    DX9::SpriteBatch->Begin();        // �菇4

    // (������2D�`��̏���������)     // �菇5

    //�w�i
    DX9::SpriteBatch->DrawSimple(bgTestSprite.Get(), SimpleMath::Vector3(bgPositionX, 0, 10));


    //�v���C���[
    DX9::SpriteBatch->DrawSimple(
        fishTestSprite[playerStatus].Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -(playerPositionY + fishScaleY[playerStatus]) / zPositionWidth),
        RectWH(playerSpriteAnimationX, playerSpriteAnimationY, fishScaleX[playerStatus], fishScaleY[playerStatus])
    );

    ////��
    //if (gaugeStage == firstStage || gaugeStage == secondStage) {
    //    DX9::SpriteBatch->DrawSimple(smallFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5),
    //        RectWH(playerSpriteAnimationX, playerSpriteAnimationY, smallFishScaleX, smallFishScaleY)
    //    );
    //}
    ////��
    //if (gaugeStage == thirdStage || gaugeStage == forthStage) {
    //    DX9::SpriteBatch->DrawSimple(mediumFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5),
    //        RectWH(playerSpriteAnimationX,playerSpriteAnimationY,mediumFishScaleX,mediumFishScaleY)
    //        );
    //}
    ////��
    //if (gaugeStage == fifthStage) {
    //    DX9::SpriteBatch->DrawSimple(largeFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5),
    //        RectWH(playerSpriteAnimationX, playerSpriteAnimationY, largeFishScaleX, largeFishScaleY)
    //    );
    //}


    //�a(�A�C�e��)
    DX9::SpriteBatch->DrawSimple(feedTestSprite.Get(), SimpleMath::Vector3(feedPositionX, feedPositionY, 0));


    //��Q��
    //�� 
    DX9::SpriteBatch->DrawSimple(birdTestSprite.Get(), SimpleMath::Vector3(birdPositionX, birdPositionY, -(birdPositionY + birdScaleY) / zPositionWidth));
   
    //��(��)
    DX9::SpriteBatch->DrawSimple(bigRockTestSprite.Get(), SimpleMath::Vector3(bigRockPositionX, bigRockPositionY, -(bigRockPositionY + bigRockScaleY) / zPositionWidth));
    
    //��(��)
    DX9::SpriteBatch->DrawSimple(smallRockTestSprite.Get(), SimpleMath::Vector3(smallRockPositionX, smallRockPositionY, -(bigRockPositionY + smallRockScaleY) / zPositionWidth));

    //��
    DX9::SpriteBatch->DrawSimple(woodTestSprite.Get(), SimpleMath::Vector3(woodPositionX, woodPositionY, -(woodPositionY + woodScaleY) / zPositionWidth));


    //UI
    //�~�j�}�b�v
    DX9::SpriteBatch->DrawSimple(miniMapSprite.Get(), SimpleMath::Vector3(miniMapPositionX, miniMapPositionY, -4));
    DX9::SpriteBatch->DrawSimple(miniMapFishTestSprite.Get(), SimpleMath::Vector3(miniMapFishPositionX, miniMapFishPositionY, -4));

    //�Q�[�W
    //�{��
    DX9::SpriteBatch->DrawSimple(gaugeTestSprite.Get(), SimpleMath::Vector3(gaugePositionX, gaugePositionY, 8),
        RectWH(0.0f, 0.0f, gaugeWidth, 100.0f));

    //�g
    DX9::SpriteBatch->DrawSimple(gaugeBgTestSprite.Get(), SimpleMath::Vector3(gaugePositionX, gaugePositionY, 9));


    //�S�[��
    if (goal)
        DX9::SpriteBatch->DrawSimple(goalSprite.Get(), SimpleMath::Vector3(goalSpritePositionX, goalSpritePositionY, -10));


    //�f�o�b�O�p
    DX9::SpriteBatch->DrawString
    (
        playerStatusFont.Get(), SimpleMath::Vector2(0, 670), 
        DX9::Colors::RGBA(0, 0, 0, 255), L"loopCount:%d", (int)loopCount
    );

    DX9::SpriteBatch->DrawString(
        gaugeStageFont.Get(), SimpleMath::Vector2(500.0f, 670.0f), 
        DX9::Colors::RGBA(0, 0, 0, 255), L"playerSpriteAnimationX:%d", playerSpriteAnimationX
    );


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
        SimpleMath::Vector2(0.0f,0.0f)
    );
    spriteBatch->End();

    DXTK->Direct3D9->WaitUpdate();
    DXTK->ExecuteCommandList();
}

//Update���֐��̒�`

//�w�i
//�X�N���[�����x
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

    switch (playerSpeedStatus) {
    case smallFishSpeedState:
        bgPositionX -= smallFishSpeed * deltaTime;
        break;
    case mediumFishSpeedState:
        bgPositionX -= mediumFishSpeed * deltaTime;
        break;
    case largeFishSpeedState:
        bgPositionX -= largeFishSpeed * deltaTime;
        break;
    case speedUpState:
        speedUpTime += deltaTime;
        bgPositionX -= topSpeed * deltaTime;
        if (speedUpTime >= 2.0f)
        {
            speedUpTime = 0.0f;
            playerSpeedStatus = largeFishSpeedState;
            speedUp = false;
        }
        break;
    }
}

//�w�i���[�v
void MainScene::bgLoopUpdate(const float deltaTime)
{
    if (bgPositionX <= -bgResetPosition) {
        bgPositionX = 0;
        ++loopCount;
    }
}

//�X�N���[�����x�ύX
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


//��ԑJ�ڊ���
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


//�N���A���Ԍv��
void MainScene::countPlayTimeUpdate(const float deltaTime)
{
    DontDestroy->clearTime += deltaTime;
}


//��ԑJ��
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
            gaugeState = forthStage;
        }
        break;
    }

    return gaugeState;
}


//�v���C���[
//�A�j���[�V����
void MainScene::playerAnimationUpdate(const float deltaTime)
{
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

    /*switch (playerSpeedStatus) {
    case smallFishSpeedState:
        playerAnimationSpeed += deltaTime;
        if (playerAnimationSpeed >= 0.05f) 
        {
            playerSpriteAnimationX += fishScaleX[playerSpeedStatus];
            playerAnimationSpeed = 0.0f;
        }
        if (playerSpriteAnimationX >= (fishScaleX[playerSpeedStatus] * 8))
        {
            playerSpriteAnimationX = 0;
        }
        break;
    case mediumFishSpeedState:
        playerAnimationSpeed += deltaTime; 
        if (playerAnimationSpeed >= 0.05f)
        {
            playerSpriteAnimationX += mediumFishScaleX;
            playerAnimationSpeed = 0.0f;
        }
        if (playerSpriteAnimationX == (mediumFishScaleX * 8))
        {
            playerSpriteAnimationX = 0;
        }
        break;
    case largeFishSpeedState:
        playerAnimationSpeed += deltaTime;
        if (playerAnimationSpeed >= 0.05f)
        {
            playerSpriteAnimationX += largeFishScaleX;
            playerAnimationSpeed = 0.0f;
        }
        if (playerSpriteAnimationX == (largeFishScaleX * 8))
        {
            playerSpriteAnimationX = 0;
        }
        break;
    }*/
}

//�ړ��\�͈�
void MainScene::playerMoveRangeUpdate()
{
    //��E��
    if (playerPositionY <= playerMoveRangeTop)
    {
        playerPositionY = playerMoveRangeTop;
    }
    if (playerPositionX <= playerMoveRangeLeft)
    {
        playerPositionX = playerMoveRangeLeft;
    }
    //�E�E��
    //����
    if (playerStatus == smallFishState) {
        if (playerPositionX >= playerMoveRangeRight - smallFishScaleX)
        {
            playerPositionX = playerMoveRangeRight - smallFishScaleX;
        }
        if (playerPositionY >= playerMoveRangeBottom - smallFishScaleY)
        {
            playerPositionY = playerMoveRangeBottom - smallFishScaleY;
        }
    }
    //�i�}�Y
    if (playerStatus == mediumFishState) {
        if (playerPositionX >= playerMoveRangeRight - mediumFishScaleX)
        {
            playerPositionX = playerMoveRangeRight - mediumFishScaleX;
        }
        if (playerPositionY >= playerMoveRangeBottom - mediumFishScaleY)
        {
            playerPositionY = playerMoveRangeBottom - mediumFishScaleY;
        }
    }
    //��
    if (playerStatus == largeFishState) {
        if (playerPositionX >= playerMoveRangeRight - largeFishScaleX)
        {
            playerPositionX = playerMoveRangeRight - largeFishScaleX;
        }
        if (playerPositionY >= playerMoveRangeBottom - largeFishScaleY)
        {
            playerPositionY = playerMoveRangeBottom - largeFishScaleY;
        }
    }
}

//�����ړ�
void MainScene::playerAutoMoveUpdate(const float deltaTime)
{
    playerPositionX -= playerAutoMoveSpeed * deltaTime;
}

//�L�[�{�[�h����
void MainScene::playerControlKeyboardUpdate(const float deltaTime)
{
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

//�p�b�h����
void MainScene::playerControlGamepadUpdate(const float deltaTime)
{
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


//�a(�A�C�e��)
//�ړ�
void MainScene::feedMoveUpdate(const float deltaTime)
{
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

//�a���[�v
void MainScene::feedLoopUpdate()
{
    if (feedPositionX <= feedResetPositionX)
    {
        feedPositionResetUpdate();
    }
}

//�ʒu���Z�b�g
void MainScene::feedPositionResetUpdate()
{
    feedResetPositionY = randomFeedPositionY(randomEngine);

    feedPositionX = feedInitialPositionX;
    feedPositionY = feedResetPositionY;
}

//�a�����蔻��
bool MainScene::isFeedCollisionDetectionUpdate()
{
    if (isPlayerCollisionDetection(RectWH(feedPositionX, feedPositionY, feedScaleX, feedScaleY)))
        return true;
    else
        return false;
}


//��Q��
//��Q���ړ� 
void MainScene::obstacleMoveUpdate(const float deltaTime)
{
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

/*
//��
void MainScene::birdObstacleMoveUpdate(const float deltaTime)
{
    birdPositionX -= 300.0f * deltaTime;
}

//��(��)
void MainScene::bigRockObstacleMoveUpdate(const float deltaTime)
{
    bigRockPositionX -= 300.0f * deltaTime;
}

//��(��)
void MainScene::smallRockObstacleMoveUpdate(const float deltaTime)
{
    smallRockPositionX -= 300.0f * deltaTime;
}

//��
void MainScene::woodObstacleMoveUpdate(const float deltaTime)
{
    woodPositionX -= 300.0f * deltaTime;
}
*/

//��Q�������蔻��
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

//��Q�����[�v
void MainScene::obstacleLoopUpdate()
{
    if (birdPositionX < obstacleResetPositionX || bigRockPositionX < obstacleResetPositionX || smallRockPositionX < obstacleResetPositionX || woodPositionX < obstacleResetPositionX) 
    {
        obstaclePositionResetUpdate();
    }
}

//��Q���Ē��I
void MainScene::obstacleReLotteryUpdate()
{
    obstaclePattern = randomObstacle(randomEngine);
    switch (obstaclePattern) {
    case birdState:
        obstacleStatus = birdState;
        break;
    case bigRockState:
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

//��Q���ʒu���Z�b�g
void MainScene::obstaclePositionResetUpdate()
{
    obstacleReLotteryUpdate();
    obstacleResetPositionY = randomObstaclePositionY(randomEngine);

    //��
    birdPositionX = obstacleInitialPositionX;
    birdPositionY = obstacleResetPositionY;
    //��(��)
    bigRockPositionX = obstacleInitialPositionX;
    bigRockPositionY = obstacleResetPositionY;
    //��(��)
    smallRockPositionX = obstacleInitialPositionX;
    smallRockPositionY = obstacleResetPositionY;
    //��
    woodPositionX = obstacleInitialPositionX;
    woodPositionY = obstacleResetPositionY;
}


//UI
//�~�j�}�b�v
void MainScene::miniMapMoveUpdate(const float deltaTime)
{
    switch (playerSpeedStatus) {
    case smallFishSpeedState:
        miniMapFishPositionX += smallFishSpeed * deltaTime;
        break;
    case mediumFishSpeedState:
        miniMapFishPositionX += mediumFishSpeed * deltaTime;
        break;
    case largeFishSpeedState:
        miniMapFishPositionX += largeFishSpeed * deltaTime;
        break;
    case speedUpState:
        miniMapFishPositionX += topSpeed * deltaTime;
        break;
    }
}

//�Q�[�W
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


//�V�[���J��
NextScene MainScene::changeNextSceneUpdate(const float deltaTime)
{
    //�N���A
    if (loopCount >= lengthToGoal)
    {
        sceneChangeBuffer += deltaTime;
        goal = true;
        if (sceneChangeBuffer >= goalAfterTime) {
            return NextScene::GameClearScene;
        }
    }
    //�Q�[���I�[�o�[
    else if (gameOver)
    {
        //return NextScene::GameOverScene;
    }

    return NextScene::Continue;
}


//�����蔻��
//�x�[�X�����蔻��
bool MainScene::isCollisionDetectionBase(Rect& rect1, Rect& rect2) {

    if (rect1.left > rect2.right || rect1.right < rect2.left ||
        rect1.top > rect2.bottom || rect1.bottom < rect2.top) {
        return false;
    }
    return true;
}

//�v���C���[�T�C�Y�ݒ�ςݓ����蔻��
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