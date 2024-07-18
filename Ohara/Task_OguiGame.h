#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//��H���~�j�Q�[���{��
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include "OharaPlayerNumber.h"

namespace  OguiGame
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{��");		//�O���[�v��
	const  string  defName("��H���Q�[������");	//�^�X�N��
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
		DG::Image::SP	img;
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
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		DG::Font::SP TestFont;

		//���ϐ�
		float timeLimit;	//��������
		bool  isInGame;		//�~�j�Q�[�����Atrue�ɂ���

	private:
		//���ϐ�
		ML::Vec2 playerFirstPos[4] = {	//�v���C���[�̏����ʒu
			{ ge->screen2DWidth / 5.f, ge->screen2DHeight / 2.f }, { ge->screen2DWidth * 2.f / 5.f, ge->screen2DHeight / 2.f },
			{ ge->screen2DWidth * 3.f / 5.f, ge->screen2DHeight / 2.f }, { ge->screen2DWidth * 4.f / 5.f, ge->screen2DHeight / 2.f }
		};
		XI::GamePad::SP controllers[4] = { ge->in1, ge->in2, ge->in3, ge->in4 };	//�擾����R���g���[���[
		PlayerNum playersNum[4] = { PlayerNum::Player1, PlayerNum::Player2, PlayerNum::Player3, PlayerNum::Player4 }; //�v���C���[�̎��ʔԍ��ݒ�p

		int   gameStateChangeCount;	//GameState��ύX����܂ł̃J�E���g
		int   countToNextTask;		//���̃^�X�N�ɂ���܂ł̃J�E���g

		//���N���X�E�\����
		enum class GameState
		{
			BeforeGameStart,	//�Q�[���J�n�O
			Game,				//�Q�[����
			Result,				//���U���g
		};
		GameState gameState;

		//���֐�
		void GameStateTransition();					//�Q�[���̏�ԑJ��
		void UpdateGameState(GameState nowState);	//�Q�[���̏�ԕύX������
		void Work();								//��Ԗ��̏���
	};
}