#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//��H���~�j�Q�[���̃v���C���[
//-------------------------------------------------------------------
#include "OharaCharacterBase.h"

namespace  OguiPlayer
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�v���C���[");	//�O���[�v��
	const  string  defName("��H���v���C���[");	//�^�X�N��
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
		DG::Image::SP image[4];
		DG::Image::SP buttonImage_A;
		DG::Image::SP buttonImage_A_Outline;
	};
	//-------------------------------------------------------------------
	class  Object : public  OCharaBase
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
		//���ϐ�
		XI::GamePad::SP controller;		//���͏����󂯎�肽���R���g���[���[�̃f�[�^���i�[
		int				eatFoodCount;	//�H�ׂ������̐�

		//�����\�b�h
		void SetExistFood(bool FoodExistenceInformation);	//�����̑��݂��Ă��邩�̏����擾

		virtual void Received() override {}

	private:
		//���ϐ�
		int	 attack;		//�H���ʁA1��{�^���������Č��闿���̗ʂ�\��
		bool existFood;		//���������邩���f����A��������true

		bool isPlayStart;		//�Q�[���{�҂��J�n���ꂽ��true

		bool isStartButtonDraw;	//�{�^���̕`�������Ƃ�true
		ML::Vec2 buttonDrawPos;	//�{�^���̕`��ʒu

		//�����\�b�h
		virtual void Think() override;
		virtual void Move() override;
		virtual DrawInformation GetDrawImage();

		void DrawButton();	//�{�^���̕`�揈��
	};
}