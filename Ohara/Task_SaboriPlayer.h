#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�T�{��~�j�Q�[���̃v���C���[
//-------------------------------------------------------------------
#include "OharaCharacterBase.h"

namespace  SaboriPlayer
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�v���C���[");	//�O���[�v��
	const  string  defName("�T�{��v���C���[");		//�^�X�N��
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
		DG::Image::SP image;
		DG::Image::SP buttonImage_A;
		DG::Image::SP buttonImage_A_Outline;
	};
	//-------------------------------------------------------------------
	class  Object : public OCharaBase
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
		XI::GamePad::SP controller;			//���͏����󂯎�肽���R���g���[���[�̃f�[�^���i�[
		float			totalSaboriTime;	//�T�{�������Ԃ̍��v��ۑ����Ă���
		bool			noticedToSabori;	//��i�ɂ��ڂ���C�Â��ꂽ��true
		

		//�����\�b�h
		virtual void Think() override;
		virtual void Move() override;
		virtual DrawInformation GetDrawImage() override;
		virtual void Received() override {}

	private:
		//���ϐ�
		bool			isPlayStartSE;		//�ŏ��Ƀv���C���[���炷SE��Play���ꂽ��true
		string			saboriSEName;		//�T�{���Ԃ�SE�̖��O
		string			workSEName;			//�d������Ԃ�SE�̖��O

		bool isStartButtonDraw;	//�{�^���̕`�������Ƃ�true
		ML::Vec2 buttonDrawPos;	//�{�^���̕`��ʒu

		//�����\�b�h
		void DrawButton();	//�{�^���̕`�揈��
	};
}