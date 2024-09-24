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
		float timeLimit;	//��������
		bool  isInGame;		//�~�j�Q�[�����Atrue�ɂ���

	private:
		//���ϐ�
		ML::Vec2 playerFirstPos[4] = {	//�v���C���[�̏����ʒu
			{ ge->screen2DWidth / 8.f, ge->screen2DHeight / 2.f + 100.f }, 
			{ ge->screen2DWidth * 3.f / 8.f, ge->screen2DHeight / 2.f + 100.f },
			{ ge->screen2DWidth * 5.f / 8.f, ge->screen2DHeight / 2.f + 100.f },
			{ ge->screen2DWidth * 7.f / 8.f, ge->screen2DHeight / 2.f + 100.f }
		};
		XI::GamePad::SP controllers[4] = { ge->in1, ge->in2, ge->in3, ge->in4 };	//�擾����R���g���[���[
		PlayerNum playersNum[4] = { PlayerNum::Player1, PlayerNum::Player2, PlayerNum::Player3, PlayerNum::Player4 }; //�v���C���[�̎��ʔԍ��ݒ�p

		ML::Vec2 gameRuleImagePos = ML::Vec2(0.f, ge->screen2DHeight / 2.f);
		ML::Vec2 fightImagePos = ML::Vec2(ge->screen2DWidth / 2.f, ge->screen2DHeight / 2.f);
		ML::Vec2 finishImagePos = ML::Vec2(0.f, ge->screen2DHeight / 2.f);

		int   countToFightDraw;			//�uFight�v�̕�����`�悷��܂ł̃J�E���g
		int   countToChangeGameState;	//GameState��ύX����܂ł̃J�E���g
		int   countToNextTask;			//���̃^�X�N�ɂ���܂ł̃J�E���g
		int   gameFps;					//�z��FPS
		bool  gameStart;				//�Q�[���J�n��true

		//���\���́E�񋓌^
		//�Q�[���̏��
		enum class GameState
		{
			BeforeGameStart,	//�Q�[���J�n�O
			Game,				//�Q�[����
			End,				//�Q�[���I��
		};
		GameState gameState;
		
		//���ʌ��߂ɕK�v�ȃv���C���[�̏��
		struct PlayerInformation
		{
			PlayerNum playerNum;	//�v���C���[���ʗp�ԍ�
			int		  eatFoodCount;	//�H�ׂ������̐�
			int		  rank;			//����
		};
		PlayerInformation playersInfo[4];

		//���֐�
		void GameStateTransition();					//�Q�[���̏�ԑJ��
		void UpdateGameState(GameState nowState);	//�Q�[���̏�ԕύX������
		void Work();								//��Ԗ��̏���
		void Render();								//��Ԗ��̕`��

		void Ranking();		//���ʌ��߂̏���
		bool compare(const PlayerInformation& playerInfoA, const PlayerInformation& playerInfoB);	//playerInfoA��playerInfoB��eatFoodCount�Ŕ�r���AplayerInfoA�̕����傫����true��Ԃ�
		void SendScore();	//ge->score�ɓ��_�𑗂�
	};
}