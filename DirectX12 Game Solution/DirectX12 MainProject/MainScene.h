//
// MainScene.h
//

#pragma once

#include "Scene.h"
#include<random>

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class MainScene final : public Scene {
public:
    MainScene();
    virtual ~MainScene() { Terminate(); }

    MainScene(MainScene&&) = default;
    MainScene& operator= (MainScene&&) = default;

    MainScene(MainScene const&) = delete;
    MainScene& operator= (MainScene const&) = delete;

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

    // �ϐ��̐錾

    //�w�i
    DX9::SPRITE bgTestSprite;
    float bgPositionX;

    //�X�N���[�����x
    const float smallFishSpeed = 300.0f;
    const float mediumFishSpeed = 500.0f;
    const float largeFishSpeed = 700.0f;
    const float topSpeed = 1300.0f;

    //�w�i���[�v�ʒu
    const float bgResetPosition = 2560.0f;

    //���[�v�J�E���g
    int loopCount;

    //�v���C����
    float playTime;


    //�v���C���[
    //�v���C���[�摜
    DX9::SPRITE fishSprite[3];

    //�v���C���[�T�C�Y
    enum playerScale {
        //��
        smallFishScaleX = 110,
        smallFishScaleY = 70,
        //��
        mediumFishScaleX = 170,
        mediumFishScaleY = 105,
        //��
        largeFishScaleX = 230,
        largeFishScaleY = 170
    };
    const int       fishScaleX[4]{ smallFishScaleX, mediumFishScaleX, largeFishScaleX,largeFishScaleX };
    const int       fishScaleY[4]{ smallFishScaleY,mediumFishScaleY,largeFishScaleY,largeFishScaleY };
    float       fishSpeed[4];


    //�v���C���[�A�j���[�V����
    int playerSpriteAnimationX, playerSpriteAnimationY;
    //1�R�}�̎���
    const float playerAnimationFrame = 0.05f;
    //�A�j���[�V�����R�}��
    enum { frameNumber = 8 };
    //�A�j���[�V�������x
    float playerAnimationSpeed;

    //�v���C���[���
    int playerStatus;
    enum playerState {
        //��
        smallFishState,
        //��
        mediumFishState,
        //��
        largeFishState
    };

    //�X�s�[�h���
    float speedUpTime;
    int playerSpeedStatus;
    bool speedUp;
    enum playerSpeedState {
        smallFishSpeedState,
        mediumFishSpeedState,
        largeFishSpeedState,
        speedUpState
    };

    //�v���C���[���W
    float playerPositionX, playerPositionY;

    //�v���C���[�����ʒu
    const float playerInitialPositionX = 100.0f;
    const float playerInitialPositionY = 300.0f;

    //�v���C���[�����ړ����x
    enum { playerAutoMoveSpeed = 300 };

    //�v���C���[�ړ����x
    //�L�[�{�[�h
    const float   keyboardPlayerUpMoveSpeed = -30.0f;
    const float   keyboardPlayerDownMoveSpeed = 30.0f;
    const float   keyboardPlayerRightMoveSpeed = 30.0f;
    const float   keyboardPlayerLeftMoveSpeed = -30.0f;

    //�p�b�h
    //�\���L�[
    const float gamePadButtonPlayerMoveSpeedUp = -30.0f;
    const float gamePadButtonPlayerMoveSpeedDown = 30.0f;
    const float gamePadButtonPlayerMoveSpeedRight = 30.0f;
    const float gamePadButtonPlayerMoveSpeedLeft = -30.0f;
    //�X�e�B�b�N
    const float gamePadPlayerMoveSpeedX = 500.0f;
    const float gamePadPlayerMoveSpeedY = 500.0f;

    //����
    float playerInertiaX, playerInertiaY;

    //�v���C���[�ړ��͈�
    enum playerMoveRange {
        playerMoveRangeTop = 251,
        playerMoveRangeBottom = 720,
        playerMoveRangeRight = 1280,
        playerMoveRangeLeft = 0
    };


    //�a(�A�C�e��)
    DX9::SPRITE feedTestSprite;
    float feedPositionX, feedPositionY;

    //�a�T�C�Y
    enum {
        feedScaleX = 80,
        feedScaleY = 70
    };

    //�a�ړ����x
    const float feedMoveSpeed = 350.0f;

    //�a�����ʒu
    const float feedInitialPositionX = 1500.0f;
    const float feedInitialPositionY = 500.0f;

    //�a���Z�b�g�ʒu
    const float feedResetPositionX = -100.0f;

    //�a�����_�������ʒu
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> randomFeedPositionY;
    float feedResetPositionY;

    //�a�o���͈�
    const float feedAppearanceTop = 251.0f;
    const float feedAppearanceBottom = 650.0f;


    //��Q��
    //��
    DX9::SPRITE birdTestSprite;
    float birdPositionX, birdPositionY;

    //��(��)
    DX9::SPRITE bigRockTestSprite;
    float bigRockPositionX, bigRockPositionY;

    //��(��)
    DX9::SPRITE smallRockTestSprite;
    float smallRockPositionX, smallRockPositionY;

    //��
    DX9::SPRITE woodTestSprite;
    float woodPositionX, woodPositionY;

    //��Q���ړ����x
    const float obstacleMoveSpeed = 350.0f;

    //��Q�������ʒu
    const float obstacleInitialPositionX = 1500.0f;
    const float obstacleInitialPositionY = 300.0f;

    //��Q�����Z�b�g�ʒu
    const float obstacleResetPositionX = -300.0f;

    //��Q�������_���ʒu
    std::uniform_real_distribution<float> randomObstaclePositionY;
    float obstacleResetPositionY;

    //��(��)�����_���ʒu
    std::uniform_int_distribution<int> randomBigRockPosition;
    bool bigRockPositionPattern;

    //��Q�������_���p�^�[��
    std::uniform_int_distribution<int> randomObstacle;
    int obstaclePattern;

    //��Q���o���͈�
    const float obstacleAppearanceTop = 251.0f;
    const float obstacleAppearanceBottom = 510.0f;

    //��(��)�o���ꏊ
    int bigRockPositionState;
    enum bigRockPosition {
        topPositionState,
        bottomPositionState
    };
    const float bigRockTopPosition = 250.0f;
    const float bigRockBottomPosition = 480.0f;

    //��Q���T�C�Y
    enum obstacleScale {
        //��
        birdScaleX = 230,
        birdScaleY = 100,
        //��(��)
        bigRockScaleX = 230,
        bigRockScaleY = 210,
        //��(��)
        smallRockScaleX = 40,
        smallRockScaleY = 50,
        //��
        woodScaleX = 230,
        woodScaleY = 130
    };

    //��Q�����
    int obstacleStatus;
    enum obstacleState {
        birdState,
        bigRockState,
        smallRockState,
        woodState
    };


    //�v���C���[�A�a�A��Q��Z���W��
    const float zPositionWidth = 100.0f;


    //UI
    //�~�j�}�b�v
    DX9::SPRITE miniMapSprite;
    enum miniMapScale {
        miniMapScaleX = 750,
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
    
    //�Q�[�W
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

    //�V�[���J��
    //�S�[��
    DX9::SPRITE goalSprite;
    float goalSpritePositionX, goalSpritePositionY;
    bool goal;
    float sceneChangeBuffer;
    //�S�[������
    enum { lengthToGoal = 10 };
    //�S�[����J�ڂ܂ł̎���
    const float goalAfterTime = 2.0f;


    //�Q�[���I�[�o�[
    bool gameOver;


    //�f�o�b�O�p
    DX9::SPRITEFONT playerStatusFont;
    DX9::SPRITEFONT gaugeStageFont;


private:
    //Update���֐��̒�`

    //�w�i
    //�X�N���[�����x
    void bgMoveSpeedUpdate(const float deltaTime);

    //�w�i���[�v
    void bgLoopUpdate(const float deltaTime);

    //�X�N���[�����x�ύX
    void setBgScrollSpeed();


    //�N���A���Ԍv��
    void countPlayTimeUpdate(const float deltaTime);


    //��ԑJ�ڊ���
    int gaugePlayerStateAssignUpdate();

    //��ԑJ��
    int gaugeStateUpdate(const float deltaTime);


    //�v���C���[
    //�A�j���[�V���� 
    void playerAnimationUpdate(const float deltaTime);

    //�ړ��\�͈�
    void playerMoveRangeUpdate();
    
    //�����ړ�
    void playerAutoMoveUpdate(const float deltaTime);
    
    //�L�[�{�[�h����
    void playerControlKeyboardUpdate(const float deltaTime);
   
    //�p�b�h����
    void playerControlGamepadUpdate(const float deltaTime);


    //�a(�A�C�e��)
    //�a�ړ�
    void feedMoveUpdate(const float deltaTime);

    //�a���[�v
    void feedLoopUpdate();

    //�a�ʒu���Z�b�g
    void feedPositionResetUpdate();
    
    //�a�����蔻��
    bool isFeedCollisionDetectionUpdate();


    //��Q��
    //��Q���ړ�
    void obstacleMoveUpdate(const float deltaTime);

    /*
    //��
    void birdObstacleMoveUpdate(const float deltaTime);

    //��(��)
    void bigRockObstacleMoveUpdate(const float deltaTime);

    //��(��)
    void smallRockObstacleMoveUpdate(const float deltaTime);

    //��
    void woodObstacleMoveUpdate(const float deltaTime);
    */

    //��Q�������蔻�� 
    bool isObstacleCollisionDetectionUpdate();

    //��Q�����[�v
    void obstacleLoopUpdate();

    //��Q���Ē��I
    void obstacleReLotteryUpdate();

    //��Q���ʒu���Z�b�g
    void obstaclePositionResetUpdate();


    //UI
    //�~�j�}�b�v
    void miniMapMoveUpdate(const float deltaTime);
    
    //�Q�[�W
    void gaugeMoveUpdate();


    //�V�[���J��
    NextScene changeNextSceneUpdate(const float deltaTime);


    //�����蔻��֐�
    //�x�[�X�����蔻��
    bool isCollisionDetectionBase(Rect& rect1, Rect& rect2);

    //�v���C���[�͈͐ݒ�ςݓ����蔻��
    bool isPlayerCollisionDetection(Rect& rect2);

};