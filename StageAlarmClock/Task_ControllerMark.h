#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../BChara.h"

namespace  ControllerMark
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�R���g���[���[�}�[�N");	//�O���[�v��
	const  string  defName("NoName");		//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()		override;
		bool  Finalize()		override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//�ύX������������������������������������������������������
		//���L����ϐ��͂����ɒǉ�����
		DG::Image::SP controllerMark;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
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
		void  UpDate()		override;	//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()	override;	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	public:
		//�ύX������������������������������������������������������
			//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
			//BChara�Ɋ܂܂�Ȃ����m�݂̂����ɒǉ�����
			//�u�ϐ��錾�������v
			//�u�ǉ����\�b�h�������v
		void Received() {};
		int animCnt;
		int animIndex;

		ML::Box2D Imageplace[4] = {
		{ ML::Box2D(1920 / 2 - 150 / 2, 50, 150, 150)},
		{ ML::Box2D(1920 / 4 - 150, 1080 / 2 - 150 / 2, 150, 150)},
		{ ML::Box2D(1920 / 4 - 300, 1080 / 4 * 3 - 150, 150, 150)},
		{ ML::Box2D(1920 / 2 - 150 / 2, 1080 / 2 - 150 / 2, 150, 150)}
		};
	};
}