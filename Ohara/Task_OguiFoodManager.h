#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//��H���~�j�Q�[���̗����Ǘ�
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include "OharaPlayerNumber.h"

namespace  OguiFoodManager
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�Ǘ�");		//�O���[�v��
	const  string  defName("��H�������Ǘ�");	//�^�X�N��
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
		//�����\�b�h
		void NotExistFood(PlayerNum food_PlayerNum);	//�������Ȃ��Ȃ��������擾

	private:
		//���ϐ�
		int  createCount[4];			//��������������܂ł̃J�E���g�A�v�f��0��1P�̗����A�v�f��1��2P�̗����E�E�E�Ƃ��Ďg�p����
		bool hasExistFoods[4];			//���������݂��Ă��邩���ʁA�v�f��0��1P�̗����A�v�f��1��2P�̗����E�E�E�Ƃ��Ďg�p����
		ML::Vec2 foodPositions[4] = {	//�����̏o���ʒu
			{ ge->screen2DWidth / 8.f, ge->screen2DHeight / 2.f + 100 }, 
			{ ge->screen2DWidth * 3.f / 8.f, ge->screen2DHeight / 2.f + 100 },
			{ ge->screen2DWidth * 5.f / 8.f, ge->screen2DHeight / 2.f + 100 },
			{ ge->screen2DWidth * 7.f / 8.f, ge->screen2DHeight / 2.f + 100 }
		};

		//�����\�b�h
		void CreateFood(PlayerNum food_PlayerNum);	//�����̐���
		void SendCreateFoodInformation(PlayerNum food_PlayerNum);			//�����𐶐��������𑼃^�X�N�ɑ���
	};
}