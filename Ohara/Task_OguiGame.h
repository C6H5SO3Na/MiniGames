#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//��H���~�j�Q�[���{��
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include "OharaPlayerNumber.h"

namespace  OguiGame
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("��H���Q�[���{��");		//�O���[�v��
	const  string  defName("��H���Q�[������");	//�^�X�N��
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
		DG::Image::SP gameRuleImage;
		DG::Image::SP fightImage;
		DG::Image::SP finishImage;
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
		void  UpDate()			override;	//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//���ϐ�
		// ��H���Q�[���֌W------------------------------------------------------------------------------------------------------------------------
		float timeLimit;	//��������
		bool  isInGame;		//�~�j�Q�[�����Atrue�ɂ���

	private:
		//���\���́E�񋓌^
		//�Q�[���̏��
		enum class GameState
		{
			BeforeGameStart,	//�Q�[���J�n�O
			Game,				//�Q�[����
			End,				//�Q�[���I��
		};

		//���ʌ��߂ɕK�v�ȃv���C���[�̏��
		struct PlayerInformation
		{
			PlayerNum playerNum;	//�v���C���[���ʗp�ԍ�
			int		  eatFoodCount;	//�H�ׂ������̐�
			int		  rank;			//����
		};

		//���ϐ�
		// �v���C���[�֌W--------------------------------------------------------------------------------------------------------------------------
		ML::Vec2				playerFirstPos[4];	// �v���C���[�̏����ʒu
		XI::GamePad::SP			controllers[4];		// �擾����R���g���[���[
		vector<XI::GamePad::SP> useControllers;		// ���ۂɎg�p����R���g���[���[���i�[����
		PlayerNum				playersNum[4];		// �v���C���[�̎��ʔԍ��ݒ�p
		int						playerCount;		// �Q�[����V�ԃv���C���[�̐l��(1�`4�͈̔͂Œl������)
		PlayerInformation		playersInfo[4];		// ���ʌ��߂ɕK�v�ȃv���C���[�̏��

		// ��H���Q�[���֌W--------------------------------------------------------------------------------------------------------------------------
		GameState	gameState;				// �Q�[���̏��
		int			countToChangeGameState;	// GameState��ύX����܂ł̃J�E���g
		int			countToNextTask;		// ���̃^�X�N�ɂ���܂ł̃J�E���g
		int			gameFps;				// �z��FPS
		bool		gameStart;				// �Q�[���J�n��true

		// �����`��֌W-----------------------------------------------------------------------------------------------------------------------------
		ML::Vec2	gameRuleImagePos;	// �Q�[���̐����̕��͂̏����ʒu
		ML::Vec2	fightImagePos;		// �uFight�v�̕����̏����ʒu
		ML::Vec2	finishImagePos;		// �uFinish�v�̕����̏����ʒu
		int			countToFightDraw;	// �uFight�v�̕�����`�悷��܂ł̃J�E���g

		//���֐�
		void GameStateTransition();					//�Q�[���̏�ԑJ��
		void UpdateGameState(GameState nowState);	//�Q�[���̏�ԕύX������
		void Work();								//��Ԗ��̏���
		void Render();								//��Ԗ��̕`��

		void Ranking();		//���ʌ��߂̏���
		bool compare(const PlayerInformation& playerInfoA, const PlayerInformation& playerInfoB);	//playerInfoA��playerInfoB��eatFoodCount�Ŕ�r���AplayerInfoA�̕����傫����true��Ԃ�
		void SendScore();	//ge->score�ɓ��_�𑗂�

	public:
		//getter�֐�
		int GetPlayerCount();	// �Q�[����V�ԃv���C���[�̐l���̏���n��
	};
}