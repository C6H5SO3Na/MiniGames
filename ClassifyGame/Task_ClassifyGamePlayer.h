#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�o�����X�Q�[���̃v���C���[
//-------------------------------------------------------------------
#include "CGBChara.h"

namespace  CGPlayer
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("CGPlayer");	//�O���[�v��
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
		DG::Image::SP CountImg;
		DG::Image::SP bookImg;
		DG::Image::SP deskImg;
		DG::Image::SP handLImg;
		DG::Image::SP handRImg;
	};
	//-------------------------------------------------------------------
	class  Object : public  CGBChara
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
		void Received() {};
		void posInitialize(ML::Vec2 ppos);
		int workTime;//���ނ̉�
		XI::GamePad::SP controller;
		struct book
		{
			int color;
			ML::Vec2 bpos;
			book():
				color(3),
				bpos(ML::Vec2(0, 0))
			{}
		};
		book books[14];
		void toFailState(int workT);
		int Rb, Gb, Bb;//�ԁA�΁A�F�̏��ނ̐��i���ސ����j
		int Fb;//���ގ��s�̏��ނ̐�

	};
}