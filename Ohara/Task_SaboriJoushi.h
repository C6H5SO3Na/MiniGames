#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�T�{��~�j�Q�[���̏�i
//-------------------------------------------------------------------
#include "OharaCharacterBase.h"

namespace  SaboriJoushi
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�M�~�b�N");	//�O���[�v��
	const  string  defName("��i");				//�^�X�N��
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
		void  Render2D_AF()	override;	//�u2D�`��v�P�t���[�����ɒm�点�s������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	public:
	//�ύX������������������������������������������������������
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//BChara�Ɋ܂܂�Ȃ����m�݂̂����ɒǉ�����

		//���ϐ�
		float changeStateTime;			//��Ԃ�ύX����^�C�~���O���i�[����
		int	  changeStateKinds;			//�ύX�����Ԃ̎�ނ����߂鐔�����i�[����
		bool  hasFoundPlayerSabori;		//�v���C���[�����ڂ��Ă�����true

		//�����\�b�h
		virtual void Think() override;
		virtual void Move() override;
		virtual DrawInformation GetDrawImage() override;
		virtual void Received() override {}

		void CheckPlayerSabori(); //�v���C���[�����ڂ��Ԃ��ǂ������肷��
	};
}