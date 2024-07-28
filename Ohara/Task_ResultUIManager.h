#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//���U���g��ʂ�UI�Ǘ�
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include  "Task_ResultUIPlayer.h"
#include  "Task_Result.h"

namespace  ResultUIManager
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�Ǘ�");	//�O���[�v��
	const  string  defName("UI�Ǘ�");		//�^�X�N��
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
		DG::Image::SP prefaceImage;
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
		void DataChangesWhenUpdatingState(); //��ԍX�V���ɂ���f�[�^(�ϐ�)�̕ύX
		 
	private:
		//���ϐ�
		ML::Vec2 playerRankEachPlayerPosition[4] = {	//�\���p�v���C���[�̈ʒu
			ML::Vec2(ge->screen2DWidth / 5.f, ge->screen2DHeight / 2.f),		//��ԍ��̈ʒu
			ML::Vec2(ge->screen2DWidth * 2.f / 5.f, ge->screen2DHeight / 2.f),	//�������Ԗڂ̈ʒu
			ML::Vec2(ge->screen2DWidth * 3.f / 5.f, ge->screen2DHeight / 2.f),	//������O�Ԃ̈ʒu
			ML::Vec2(ge->screen2DWidth * 4.f / 5.f, ge->screen2DHeight / 2.f)	//��ԉE�̈ʒu
		};

		ML::Vec2 playerNumberPosition[4] = {			//�v���C���[�ԍ��̕\���ʒu
			ML::Vec2(ge->screen2DWidth / 5.f, ge->screen2DHeight / 2.f - 100),			//��ԍ��̈ʒu
			ML::Vec2(ge->screen2DWidth * 2.f / 5.f, ge->screen2DHeight / 2.f - 100),	//�������Ԗڂ̈ʒu
			ML::Vec2(ge->screen2DWidth * 3.f / 5.f, ge->screen2DHeight / 2.f - 100),	//������O�Ԃ̈ʒu
			ML::Vec2(ge->screen2DWidth * 4.f / 5.f, ge->screen2DHeight / 2.f - 100)		//��ԉE�̈ʒu
		};

		ML::Vec2 rankSentencePosition[4] = {			//��Ђ̊K���̕\���ʒu
			ML::Vec2(ge->screen2DWidth / 5.f, ge->screen2DHeight / 2.f + 100),			//��ԍ��̈ʒu
			ML::Vec2(ge->screen2DWidth * 2.f / 5.f, ge->screen2DHeight / 2.f + 100),	//�������Ԗڂ̈ʒu
			ML::Vec2(ge->screen2DWidth * 3.f / 5.f, ge->screen2DHeight / 2.f + 100),	//������O�Ԃ̈ʒu
			ML::Vec2(ge->screen2DWidth * 4.f / 5.f, ge->screen2DHeight / 2.f + 100)		//��ԉE�̈ʒu
		};

		ML::Vec2 scoreDrawPosition[4] = {				//�X�R�A�̕`��ʒu
			ML::Vec2(ge->screen2DWidth / 5.f, ge->screen2DHeight / 2.f - 200),			//��ԍ��̈ʒu
			ML::Vec2(ge->screen2DWidth * 2.f / 5.f, ge->screen2DHeight / 2.f - 200),	//�������Ԗڂ̈ʒu
			ML::Vec2(ge->screen2DWidth * 3.f / 5.f, ge->screen2DHeight / 2.f - 200),	//������O�Ԃ̈ʒu
			ML::Vec2(ge->screen2DWidth * 4.f / 5.f, ge->screen2DHeight / 2.f - 200)		//��ԉE�̈ʒu
		};

		int  srcValues[1][4] = { { 0, 0, 567, 157 } //���ʔ��\�̉摜
		};	//�摜�̐؂��蕔��
		int  drawValues[1][4] = { { static_cast<int>(ge->screen2DWidth) / 2 - srcValues[0][2] / 2, static_cast<int>(ge->screen2DHeight) / 2 - srcValues[0][3] / 2, srcValues[0][2], srcValues[0][3] } //���ʔ��\�̉摜
		};	//�摜�̕`��ʒu

		int  drawUpToCount;						//�`�悷��܂ł̃J�E���g
		int  gameFps;							//�Q�[���v���C�����őz�肵�Ă���FPS�l��ݒ�
		bool hasEndedDrawing;					//�`�悪�I��������true
		bool isChangedFalse_hasEndedDrawing;	//hasEndedDrawing��false�ɕς�����true
		int	 displayPattern;					//���ʔ��\���̕\���p�^�[��

		//�����\�b�h
		void ProcessEachResultState();				//Task_Result�̏�Ԗ��̏���
		void DrawingProcessingEachResultState();	//Task_Result�̏�Ԗ��̕`�揈��

		int  DecideDisplayPattern();	//���ʔ��\���̕\���p�^�[�������߂�
		void GenerateDisplayPlayer();   //�\���p�v���C���[�̐���
	};
}