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
		DG::Image::SP totalSaboriTimeImage;
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
		//�v���C���[���ʔԍ��֌W---------------------------------------------------------------------------------------
		//���\����
		struct PlayerNumberDrawInformation
		{
			ML::Box2D	draw, src;	// �`��ʒu, ���摜�؂���ʒu
			ML::Vec2	pos;		// Offset�̈ʒu
		};

		//���ϐ�
		PlayerNumberDrawInformation playerNumbersDrawInfo[4];					// �v���C���[�ԍ��̕`����
		PlayerNumberDrawInformation playerNumbersDrawInfo_TotalSaboriTime[4];	// �T�{�荇�v���Ԃ̔��ʗp�v���C���[�ԍ��̕`����
		int							playerCount;								// �Q�[����V�ԃv���C���[�̐l��(1�`4�͈̔͂Œl������)

		//�����\�b�h
		void DrawPlayerNumber();									// �v���C���[�ԍ��̕`��
		void DrawPlayerNumber_TotalSaboriTime(const int loopCount);	// �T�{�荇�v���Ԃ̔��ʗp�v���C���[�ԍ��̕`��

		//�T�{�荇�v���ԕ`��֌W---------------------------------------------------------------------------------------
		//���ϐ�
		char totalSaboriTimeText[5];	// ���v���Ԃ������_���ʂ܂ŕ\������悤�ɂ���(�z��̕K�v�v�f�� = ��������2 + �����_1 + ��������1 + �k������1 = 5)

		//���֐�
		void DrawTotalSaboriTime(const float totalSaboriTime, const int loopCount);	// ���v�T�{�莞�Ԃ̕`��
	};
}