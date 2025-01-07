#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���[
//-------------------------------------------------------------------
#include "../BChara.h"
#include "Task_TaxiGamePlayerStateBase.h"

namespace TaxiGamePlayer
{
	//�O���錾
	class StateBase;

	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�^�N�V�[");	//�O���[�v��
	const  string  defName("�v���C��");	//�^�X�N��
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

		DG::Image::SP imageButton[2][4];//[0,1][A,B,X,Y]
		DG::Image::SP imageClear;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
		//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		static void Spawn(const ML::Vec2& pos_, XI::GamePad::SP controller_, const int& controllerNum_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);

		//�R���g���[���[�̕ϐ�
		XI::GamePad::SP controller;//in1 in2�Ȃ�
		XI::VGamePad input;

		//�O��̈ʒu(�C�[�W���O�p)
		ML::Vec2 prePosition;

		//�v���C���[�摜
		DG::Image::SP imagePlayer;

		//�v���C���[�摜�̃p�X
		const string imgPlayerPath[4] = {
			"./data/image/game_otsan_run_red.png",
			"./data/image/game_otsan_run.png",
			"./data/image/game_otsan_run_yellow.png",
			"./data/image/game_otsan_run_green.png"
		};

		//�N���A�t���O
		bool isClear;

		int animationCount = 0;

		int nowButton = 0;
		int matchCount = 0;

		int controllerNum = 1;

		//���\�b�h
		void Think();
		void Move();
		void Received() override;

		//�{�^���̃r�b�g
		enum Button {
			A = 0b00010000,
			B = 0b00100000,
			X = 0b01000000,
			Y = 0b10000000
		};

		StateBase* state;

	public:
		int GetButtonNum(int state);
		void ChangeState(StateBase* state_);//��ԕύX
		bool IsClear() const { return isClear; }
		static int playerScore;//����
		void AddScore(int& n, XI::GamePad::SP con);
		void MatchedButton();

		int GetNowButton() const { return nowButton; }
		XI::GamePad::SP GetController() const { return controller; }
		int GetControllerNum() const { return controllerNum; }

		int GetAnimationCount() const { return animationCount; }

		ML::Vec2 GetPrePosition() const { return prePosition; }

		void DrawClearMessage() const; //�N���A���b�Z�[�W�\��
		void DrawButton() const; //�{�^���\��
		DG::Image::SP GetImagePlayer() const { return imagePlayer; }//�v���C���[�摜���\�[�X���擾
	};
}