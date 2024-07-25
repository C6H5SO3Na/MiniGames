#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���[
//-------------------------------------------------------------------
#include "../BChara.h"

namespace TaxiGamePlayer
{
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

		DG::Image::SP imgPlayer;
		DG::Image::SP imgBtn[2][4];//[0,1][A,B,X,Y]
		DG::Image::SP imgClear;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
		//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		static void Spawn(const ML::Vec2& pos_, XI::GamePad::SP controller_);
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
		ML::Vec2 prePos;

		//�N���A�t���O
		bool isClear;

		int animCnt = 0;

		int nowBtn = 0;
		int matchCnt = 0;

		//���\�b�h
		void Think();
		void Move();
		void Recieved() override;
		void DrawButton();
		void MatchButton();

		//�{�^���̃r�b�g
		enum Button {
			A = 0b00010000,
			B = 0b00100000,
			X = 0b01000000,
			Y = 0b10000000
		};

		//�|�����[�t�B�Y��
		//��Ԃ̒��ۃC���^�[�t�F�[�X
		class StateBase {
		public:
			virtual ~StateBase() {}
			virtual void think() = 0;
			virtual void move() = 0;
			virtual void render() = 0;
		private:
			ML::Box2D animTable_;
		};

		//�Î~
		class IdleState :public StateBase {
		public:
			IdleState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		//�����Ă���
		class MoveState :public StateBase {
		public:
			MoveState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		//�N���A
		class ClearState :public StateBase {
		public:
			ClearState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		//�~�X
		class MissState :public StateBase {
		public:
			MissState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		StateBase* state;

		void ChangeState(StateBase* state_);//��ԕύX
		int BUTTON(int state);
		void AddScore(int& n, XI::GamePad::SP con);

	public:
		bool IsClear() const { return isClear; }
		static int playerScore;//����
	};
}