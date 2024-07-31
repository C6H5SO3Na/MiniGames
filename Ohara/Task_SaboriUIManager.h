#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�T�{��~�j�Q�[����UI�Ǘ�
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"

namespace  SaboriUIManager
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�Ǘ�");	//�O���[�v��
	const  string  defName("UI�Ǘ�");	//�^�X�N��
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
		DG::Image::SP playerNumberImage;
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
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����

	private:
		//���\����
		struct PlayerNumberDrawInformation
		{
			ML::Box2D draw, src;
			ML::Vec2 pos;
		};

		//���ϐ�
		DG::Font::SP testFont;

		PlayerNumberDrawInformation playerNumbersDrawInfo[4] = {	//�v���C���[�ԍ��̕`����
			{ ML::Box2D(-78, -53, 155, 105), ML::Box2D(0, 0, 155, 105), ML::Vec2(ge->screen2DWidth / 8.f, ge->screen2DHeight - 480.f) },			//1P
			{ ML::Box2D(-96, -53, 192, 105), ML::Box2D(155, 0, 192, 105), ML::Vec2(ge->screen2DWidth * 3.f / 8.f, ge->screen2DHeight - 480.f) },	//2P
			{ ML::Box2D(-88, -53, 175, 105), ML::Box2D(347, 0, 175, 105), ML::Vec2(ge->screen2DWidth * 5.f / 8.f, ge->screen2DHeight - 480.f) },	//3P
			{ ML::Box2D(-97, -53, 193, 105), ML::Box2D(522, 0, 193, 105), ML::Vec2(ge->screen2DWidth * 7.f / 8.f, ge->screen2DHeight - 480.f) }		//4P
		};

		//�����\�b�h
		void DrawPlayerNumber();	//�v���C���[�ԍ��̕`��
	};
}