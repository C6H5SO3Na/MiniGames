#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�T�{��~�j�Q�[���{��
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include "OharaPlayerNumber.h"

namespace  SaboriGame
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�T�{��Q�[���{��");		//�O���[�v��
	const  string  defName("�T�{��Q�[������");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		DG::Image::SP gameRuleImage;
		DG::Image::SP fightImage;
		DG::Image::SP finishImage;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
		//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;	//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		//�ύX������������������������������������������������������
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//�T�{��Q�[���֌W--------------------------------------------------------------------------------------------------------------------------------------------
		//���ϐ�
		bool		gameStart;				// �Q�[���J�n��true	
		int			countToNextTask;		// ���̃^�X�N�ɂ���܂ł̃J�E���g
		float		timeLimit;				// ��������

		//�����\�b�h
		void		Work();					// ��Ԗ��̏���

	public:
		//���ϐ�
		bool		isInGame;				// �~�j�Q�[���������f����B�~�j�Q�[����true�ɂ���

	private:
		//�v���C���[�֌W----------------------------------------------------------------------------------------------------------------------------------------------
		//���\����
		//���ʌ��߂ɕK�v�ȃv���C���[�̏��
		struct PlayerInformation
		{
			PlayerNum playerNum;		// �v���C���[���ʗp�ԍ�
			float	  totalSaboriTime;	// ���v���ڂ莞��
			int		  rank;				// ����
		};

		//���ϐ�
		ML::Vec2				playerFirstPos[4];	// �v���C���[�̏����ʒu
		vector<XI::GamePad::SP> useControllers;		// ���ۂɎg�p����R���g���[���[���i�[����
		PlayerNum				playersNum[4];		// �v���C���[�̎��ʔԍ��ݒ�p
		PlayerInformation		playersInfo[4];		// ���ʌ��߂ɕK�v�ȃv���C���[�̏��
		int						playerCount;		// �Q�[����V�ԃv���C���[�̐l��(1�`4�͈̔͂Œl������)

		//���֐�
		void Ranking();																				//���ʌ��߂̏���
		bool compare(const PlayerInformation& playerInfoA, const PlayerInformation& playerInfoB);	//playerInfoA��playerInfoB��totalSaboriTime�Ŕ�r���AplayerInfoA�̕����傫����true��Ԃ�
		void SendScore();																			//ge->score�ɓ��_�𑗂�

		//��i�֌W----------------------------------------------------------------------------------------------------------------------------------------------------
		//���ϐ�
		ML::Vec2 joushiFirstPos;	//��i�̏����ʒu

	public:
		//getter�֐�--------------------------------------------------------------------------------------------------------------------------------------------------
		int GetPlayerCount();	// �Q�[����V�ԃv���C���[�̐l���̏���n��

	};
}