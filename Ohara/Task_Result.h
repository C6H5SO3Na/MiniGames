#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�ŏI���U���g��ʃ^�X�N
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"

namespace  Result
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("���U���g���");	//�O���[�v��
	const  string  defName("���U���g����");	//�^�X�N��
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

		//���N���X�E�\���́E�񋓌^
		enum class ResultState
		{
			Preface,			//�O�u��
			ResultAnnouncement,	//���ʔ��\
			End,				//���ʔ��\�I��
		};
		ResultState resultState;

		struct PlayerInformation
		{
			int PlayerNum;
			int rank;
			int score;
		};
		PlayerInformation playersInfo[4] = { {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 0} };
		
		//�����\�b�h
		void ChangeableToNextState();	//���̏�ԂɕύX�ł���悤�ɂ���

	private:		
		//���ϐ�
		XI::GamePad::SP controllers[4] = { ge->in1, ge->in2, ge->in3, ge->in4 }; //���͂���R���g���[���[�̏����i�[
		
		bool nextStateGoIs;			//true�ɂȂ����玟�̏�ԂɕύX�o����悤�ɂ���
		int  countUpToStateChange;	//��ԕύX�܂ł̃J�E���g
		int  gameFps;				//�Q�[���v���C�����őz�肵�Ă���FPS�l��ݒ�
		bool shouldKillTask;		//UpDate��������Task�����ł����鎞�Ɏg�p true�̎��ɏ��ł�����

		//�����\�b�h
		void ResultStateTransition();					//���U���g�̏�ԑJ��
		void UpdateResultState(ResultState nowState);	//���U���g�̏�ԕύX������
		void ProcessEachState();						//��Ԗ��̏���

		void Ranking();	//���ʌ��߂̏���
		bool compare(const PlayerInformation& scoreA, const PlayerInformation& scoreB);	//scoreA��scoreB�ɓ��ꂽ�ϐ���score�Ŕ�r���AscoreA���傫����true��Ԃ�
	};
}