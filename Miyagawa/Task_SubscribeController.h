#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�R���g���[���[�o�^���
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"

namespace SubscribeController
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�R���g���[���[�o�^���");	//�O���[�v��
	const  string  defName("Noname");//�^�X�N��
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

		DG::Image::SP playerNumLogo;
		DG::Image::SP playerImg[4];
		DG::Image::SP startButton[2];
		DG::Image::SP AButton[2];
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
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������

		void Subscribe(XI::GamePad::SP controller, bool& isPush, const int& i);
		void StartEasing(const int& index) const;

		void DrawPlayerNums() const;
		void DrawPlayers() const;

		void DrawUI() const;
		void DrawOperation() const;
		void DrawButton(DG::Image::SP img, const ML::Vec2& pos) const;

		DG::Font::SP TestFont;
		bool isPushButton[4] = {};
		int controllerIndex[4] = {};//~P��~�R���g���[���[��
		int subscribeCnt = 0;
		float easingPos[4] = {};

		//�v���C���[�̃��\�[�X��`
		ML::Box2D srcPlayer = { 436, 0, 436, 542 };

		vector<XI::GamePad::SP> inputs;
		int animCnt = 0;
	};
}