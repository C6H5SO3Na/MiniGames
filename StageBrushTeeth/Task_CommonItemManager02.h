#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include "Task_StainManager.h"
#include "Task_brush.h"

namespace  CommonItemManager02
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("���ʃA�C�e���}�l�[�W���[02");	//�O���[�v��
	const  string  defName("�}�l�[�W���[");	//�^�X�N��
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
		//�u�ϐ��錾�������v
		DG::Image::SP bgImg;
		DG::Image::SP teethImg;
		DG::Image::SP PlayerNum;
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
		//�u�ϐ��錾�������v
		//�u�ǉ����\�b�h�������v
		vector<StainManager::Object::SP> CreateStatinList;
		vector<brush::Object::SP> PLBrushList;
		vector<XI::GamePad::SP> CTList;

		int addscore[4] = { 4, 2, 1, 0 };
		int rank = 0;

		ML::Box2D PlayerAreaPos[4][4] = {
			{ ML::Box2D(480, 270, 960, 540)},
			{ ML::Box2D(480, 0, 960, 540), ML::Box2D(480, 540, 960, 540)},
			{ ML::Box2D(0, 0, 960, 540), ML::Box2D(960, 0, 960, 540), ML::Box2D(480, 540, 960, 540)},
			{ ML::Box2D(0, 0, 960, 540), ML::Box2D(960, 0, 960, 540), ML::Box2D(0, 540, 960, 540), ML::Box2D(960, 540, 960, 540) }
		};

		ML::Box2D PlayerNumIndexSrc[4] = {
			{ ML::Box2D(0, 0, 715 / 4 - 20, 105)},
			{ ML::Box2D(715 / 4 - 20, 0, 715 / 4, 105)},
			{ ML::Box2D(715 / 2 - 10, 0, 715 / 4, 105)},
			{ ML::Box2D(715 / 4 * 3 - 10, 0, 715 / 4, 105)}
		};
	};
}