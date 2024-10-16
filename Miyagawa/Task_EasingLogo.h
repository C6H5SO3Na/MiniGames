#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�[���̍ŏ��ɏo��w��
//-------------------------------------------------------------------
#include "../BChara.h"
#include  "Task_GameMessage.h"

namespace EasingLogo
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("UI");	//�O���[�v��
	const  string  defName("�C�[�W���O���郍�S");//�^�X�N��
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

		//���ꂼ��̃Q�[���̎w���̉摜���i�[
		vector<shared_ptr<DG::Image>> img;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
		//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		static  Object::SP  Spawn(const bool& isFinish);
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

		ML::Vec2 pos;
		GameMessage::Object::WP logo;//�z�Q�Ƃ�h������weak_ptr���g�p
		bool isFinish;
		bool isCreated = false;

		ML::Box2D srcTable[8] = {
			ML::Box2D(0, 0, 647, 123),//Stage1
			ML::Box2D(0, 0, 341, 96),
			ML::Box2D(0, 0, 585, 94),
			ML::Box2D(0, 0, 583, 116),
			ML::Box2D(0, 0, 750, 161),
			ML::Box2D(0, 0, 700, 150),
			ML::Box2D(0, 0, 564, 116),
			ML::Box2D(0, 0, 438, 105)//Finish
		};
	};
}