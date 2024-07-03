#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���[
//-------------------------------------------------------------------
#include "../BChara.h"

namespace TaxiGamePlayer2
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�v���C��");	//�O���[�v��
	const  string  defName("�^�N�V�[");	//�^�X�N��
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

		DG::Image::SP img;
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

		XI::GamePad::SP controller;//in1 in2�Ȃ�
		XI::VGamePad input;
		void Think();
		void Move();
		void Recieved() override;
		DG::Font::SP TestFont;
		string str;
		bool isClear;

		enum Button {
			A, B, X, Y
		};
		string btn[4] = {
			"A","B","X","Y"
		};
		int nowBtn = 0;
		void MatchButton();
		int matchCnt = 0;

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

		//�ʏ�
		class NormalState :public StateBase {
		public:
			NormalState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		//�ʏ�
		class ClearState :public StateBase {
		public:
			ClearState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		StateBase* state;

		void ChangeState(StateBase* state_);//��ԕύX
	};
}