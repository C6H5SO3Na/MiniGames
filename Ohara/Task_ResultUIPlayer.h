#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//���U���g�\���p�v���C���[
//-------------------------------------------------------------------
#include "OharaCharacterBase.h"
#include "Task_Result.h"

namespace  ResultUIPlayer
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("��ʕ\���p");	//�O���[�v��
	const  string  defName("�\���p�v���C���[");	//�^�X�N��
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
		DG::Image::SP playerImage[4];
		DG::Image::SP playerNumberImage;
		DG::Image::SP rankImage;
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
	private:
		//���\����
		//�`�悷��v���C���[�̏��
		struct DrawPlayerInformation
		{
			int PlayerNum;
			int rank;
			int score;
		};
		DrawPlayerInformation drawPlayerInfo = {};

		//�`�悷��ʒu
		struct DrawPos
		{
			ML::Vec2 displayPlayerPos;
			ML::Vec2 playerNumPos;
			ML::Vec2 rankPos;
			ML::Vec2 scorePos;
		};
		DrawPos drawPos = {};
		
		//�����\�b�h
		DrawInformation GetDrawImages(int imageNum); ////�`��摜�̐���@imageNum��0 = �v���C���[�A1 = �v���C���[�̔ԍ��A2 = ���ʁA3 = �X�R�A�@�̉摜����

		virtual void Think() override {}
		virtual void Move() override {}
		virtual DrawInformation GetDrawImage() override { return {}; }
		virtual void Received() override {}

	public:
		//�����\�b�h
		void setPlayerInformation(const Result::Object::PlayerInformation& playerInfo);	//�`�悷��v���C���[�̏����擾����
		void setDrawPos(const ML::Vec2& playerPos, const ML::Vec2& playerNumPos, const ML::Vec2& rankPos, const ML::Vec2& scorePos); //�`��ʒu��ݒ肷��
	};
}