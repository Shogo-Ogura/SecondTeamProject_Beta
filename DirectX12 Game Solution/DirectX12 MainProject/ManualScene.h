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

    // �ϐ��̐錾

    //�w�i
    DX9::SPRITE bgTestSprite;
    float bgPositionX;

    //�X�N���[�����x
    const float smallFishSpeed = 300.0f;
    const float mediumFishSpeed = 500.0f;
    const float largeFishSpeed = 700.0f;
    const float topFishSpeed = 1300.0f;

    //�w�i���[�v�ʒu
    const float bgResetPosition = 2560.0f;
    float bgMoveDistance;

    //���[�v�J�E���g
    int bgLoopCount;

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
    //1�R�}�̕\������
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
    const float   keyboardPlayerUpMoveSpeed    = -30.0f;
    const float   keyboardPlayerDownMoveSpeed  =  30.0f;
    const float   keyboardPlayerRightMoveSpeed =  30.0f;
    const float   keyboardPlayerLeftMoveSpeed  = -30.0f;

    //�p�b�h
    //�\���L�[
    const float gamePadButtonPlayerMoveSpeedUp    = -30.0f;
    const float gamePadButtonPlayerMoveSpeedDown  =  30.0f;
    const float gamePadButtonPlayerMoveSpeedRight =  30.0f;
    const float gamePadButtonPlayerMoveSpeedLeft  = -30.0f;
    //�X�e�B�b�N
    const float gamePadPlayerMoveSpeedX = 500.0f;
    const float gamePadPlayerMoveSpeedY = 500.0f;

    //����
    float playerInertiaX, playerInertiaY;

    //�v���C���[�ړ��͈�
    enum playerMoveRange {
        playerMoveRangeTop    = 251,
        playerMoveRangeBottom = 720,
        playerMoveRangeRight  = 1280,
        playerMoveRangeLeft   = 0
    };


    //�a(�A�C�e��)
    enum { feedMaxAmount = 14 };    //�ő��
    DX9::SPRITE feedTestSprite[feedMaxAmount];
    float feedPositionX[feedMaxAmount], feedPositionY[feedMaxAmount];
    bool feedGet;
    float feedGetTime;

    //�a�T�C�Y
    enum {
        feedScaleX = 80,
        feedScaleY = 70
    };

    //�a�ړ����x
    const float feedMoveSpeed = 350.0f;


    //��Q��
    //�e���Q���ő��
    enum obstacleamount {
        birdMaxAmount      = 3,
        bigRockMaxAmount   = 15,
        smallRockMaxAmount = 34,
        woodMaxAmount      = 2
    };

    //��
    DX9::SPRITE birdTestSprite[birdMaxAmount];
    float birdPositionX[birdMaxAmount];
    float birdPositionY[birdMaxAmount];

    //��(��)
    DX9::SPRITE bigRockSprite[bigRockMaxAmount];
    float bigRockPositionX[bigRockMaxAmount], bigRockPositionY[bigRockMaxAmount];

    //��(��)
    DX9::SPRITE smallRockSprite[smallRockMaxAmount];
    float smallRockPositionX[smallRockMaxAmount], smallRockPositionY[smallRockMaxAmount];

    //��
    DX9::SPRITE woodSprite[woodMaxAmount];
    float woodPositionX[woodMaxAmount], woodPositionY[woodMaxAmount];

    bool obstacleCollision;
    float obstacleCollisionTime;

    //��Q���ړ����x
    const float obstacleMoveSpeed = 350.0f;

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

    /*//��Q�����
    int obstacleStatus;
    enum obstacleState {
        birdState,
        bigRockState,
        smallRockState,
        woodState
    };*/


    //�v���C���[�A�a�A��Q��Z���W��
    const float zPositionWidth = 100.0f;


    //UI
    //�~�j�}�b�v
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
    const float goalSpritePositionX = 0.0f;
    const float goalSpritePositionY = 0.0f;
    float goalCollisionPositionX, goalCollisionPositionY;
    bool goal;
    float sceneChangeBuffer;
    //�S�[������
    enum { lengthToGoal = 3 };
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

    //���G����
    void invalidTime(const float deltaTime);


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
    //�a�ʒu�ݒ�
    void feedPositionSetting();
    
    //�a�ړ�
    void feedMoveUpdate(const float deltaTime);

    //�a�����蔻��
    bool isFeedCollisionDetectionUpdate();


    //��Q��
    //���ʒu�ݒ�
    void birdPositionSetting();

    //��(��)
    void bigRockPositionSetting();

    //��(��)
    void smallRockPositionSetting();

    //��
    void woodPositionSetting();
    
    //��Q���ړ�
    void obstacleMoveUpdate(const float deltaTime);

    //��Q�������蔻�� 
    bool isObstacleCollisionDetectionUpdate();

    //UI
    //�~�j�}�b�v
    void miniMapMoveUpdate(const float deltaTime);

    //�Q�[�W
    void gaugeMoveUpdate();

    //�S�[��
    void goalCollisionDetectionUpdate();


    //�V�[���J��
    NextScene changeNextSceneUpdate(const float deltaTime);


    //�����蔻��֐�
    //�x�[�X�����蔻��
    bool isCollisionDetectionBase(Rect& rect1, Rect& rect2);

    //�v���C���[�͈͐ݒ�ςݓ����蔻��
    bool isPlayerCollisionDetection(Rect& rect2);

};