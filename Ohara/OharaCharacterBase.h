#pragma once
//-----------------------------------------------------------------------------
//���������삵���L�����N�^�[�̊��N���X
//-----------------------------------------------------------------------------
#include "../BChara.h"
#include "OharaPlayerNumber.h"

class OCharaBase : public BChara
{
public:
	//�R���X�g���N�^
	OCharaBase()
		:
		//�񋓌^�ϐ�
		state(State::Default), playerNum(PlayerNum::Player1), playerType(PlayerType::HumanPlayer),
		//�ϐ�
		gameFps(60), animationCount(0)
	{}

	//�f�X�g���N�^
	virtual ~OCharaBase() {}

protected:
	//���N���X�E�\����
	struct DrawInformation
	{
		//�������o�ϐ�
		ML::Box2D draw, src;
	};

	//���񋓌^
	enum class PlayerType
	{
		HumanPlayer,	// �v���C�������삷��ꍇ
		CPUPlayer,		// �v���C�������삵�Ȃ��ꍇ
	};

	//���ϐ�
	int			gameFps;		// �Q�[���v���C�����őz�肵�Ă���FPS�l��ݒ�
	int			animationCount;	// �摜���A�j���[�V�����ɂ��邽�߂̃J�E���g
	PlayerType	playerType;		// �v���C�������삷�邩CPU���������邩�����߂�
	
public:
	//���񋓌^
	enum class State
	{
		//������
		Default,					//������ԁA�������Ȃ�

		//���T�{��~�j�Q�[���̃v���C���[�p
		PWork,						//�d�������
		PSabori,					//�T�{����
		PNoticed,					//�T�{��΂�̏��

		//���T�{��~�j�Q�[���̏�i�p
		JWork,						//�d�������
		JBeforeCheckingForSabori,	//�T�{��m�F�̗\��������
		JCheckForSabori,			//�T�{��m�F���
		JFoundSabori,				//�T�{�蔭�����

		//����H���~�j�Q�[���̃v���C���[�p
		PWait,						//�ҋ@���
		PEat,						//�H�������

		//����H���~�j�Q�[���̗����p
		FExist,						//�������c���Ă�����
		FNotExist,					//�������c���Ă��Ȃ����
	};

	//���ϐ�
	State		 state;		// �v���C���[�̏��
	PlayerNum    playerNum; // �v���C���[���ʗp�ԍ�

protected:
	//�����\�b�h
	virtual void Think() = 0;					//�L�����N�^�[�̏�Ԑ���
	void UpdateState(State nowState);			//��ԍX�V���̏���
	virtual void Move() = 0;					//��Ԗ��̍s������
	virtual DrawInformation GetDrawImage() = 0;	//�`��摜�̐���
};