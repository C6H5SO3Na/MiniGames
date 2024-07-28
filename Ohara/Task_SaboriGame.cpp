//-------------------------------------------------------------------
//�T�{��~�j�Q�[���{��
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriGame.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriJoushi.h"
#include  "Task_SaboriUIManager.h"
#include  "../Task_Game.h"
#include  "../randomLib.h"

namespace  SaboriGame
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		res = Resource::Create();

		//���f�[�^������

		//���^�X�N�̐���
		//�v���C���[
		for (int i = 0; i < size(controllers); ++i)
		{
			auto p = SaboriPlayer::Object::Create(true);
			p->pos = this->playerFirstPos[i];
			p->controller = this->controllers[i];
			p->playerNum = playersNum[i];
		}

		//��i
		auto j = SaboriJoushi::Object::Create(true);
		j->pos = joushiFirstPos;

		//UI�Ǘ�
		SaboriUIManager::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�T�{��Q�[���{��");
		ge->KillAll_G("�v���C���[");
		ge->KillAll_G("�M�~�b�N");
		ge->KillAll_G("�Ǘ�");

		if (!ge->QuitFlag() && nextTaskCreate) {
			//�������p���^�X�N�̐���
			Game::Object::CreateTask(5);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->gameStateChangeCount++;

		//�Q�[���̏�ԑJ��
		this->GameStateTransition();
		//��ԂɑΉ�����s������
		this->Work();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//�Q�[���̏�ԑJ��
	void Object::GameStateTransition()
	{
		GameState nowState = this->gameState;	//�Ƃ肠�������݂̏�Ԃ���

		//���[�V�����̐؂�ւ�
		switch (nowState)
		{
		case GameState::BeforeGameStart:	//�Q�[���J�n�O
			if (gameStateChangeCount >= 60 * 2) { nowState = GameState::Game; } //�Q�[������
			break;

		case GameState::Game:				//�Q�[����
			if (timeLimit == 0) { nowState = GameState::End; } //�������Ԃ�0�ɂȂ�����Q�[���I����
			break;
		}

		//��ԍX�V
		this->UpdateGameState(nowState);
	}
	//-------------------------------------------------------------------
	//�Q�[���̏�ԕύX������
	void Object::UpdateGameState(GameState nowState)
	{
		if (nowState != this->gameState)
		{
			this->gameState = nowState;
			this->gameStateChangeCount = 0;
		}
	}
	//-------------------------------------------------------------------
	//��Ԗ��̏���
	void Object::Work()
	{
		switch (this->gameState)
		{
		case GameState::BeforeGameStart:	//�Q�[���J�n�O
			break;

		case GameState::Game:				//�Q�[����
			//���Q�[�����J�n����
			if (this->isInGame == false)
			{
				this->isInGame = true;
			}

			//���������Ԃ����炷
			this->timeLimit -= 1.f / 60; // / 60 �� / GetFps()�ɕύX���ă��j�^�[FPS�ɃQ�[�����ˑ����Ȃ��悤�ɂ���

			//�������Ԃ�0������������0�ɂ���
			if (this->timeLimit < 0.f)
			{
				this->timeLimit = 0.f;
			}
			break;

		case GameState::End:				//�Q�[���I��
			//�Q�[���̏�Ԃ�End�̎��A��x�����s������
			if (this->isInGame == true)
			{
				//���Q�[�����I��������
				this->isInGame = false;

				//�����ʂ����߁Age->score�ɓ��_�𑗂�
				//���ʂ����߂�
				this->Ranking();
				//ge->score�ɓ��_�𑗂�
				this->SendScore();
			}

			//�����̃^�X�N�ɍs���܂ł̃J�E���g
			this->countToNextTask++;

			//�������^�X�N���Ő\��
			if (this->countToNextTask == 60 * 10) { //60��GetFps()�ɂ��� 
				ge->StartCounter("test", 0);
			}
			if (ge->getCounterFlag("test") == ge->LIMIT) {
				Kill();
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//���ʌ��߂̏���
	void Object::Ranking()
	{
		//�v���C���[��������擾����
		auto players = ge->GetTasks<SaboriPlayer::Object>("�v���C���[");
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			switch ((*p)->playerNum)
			{
			case PlayerNum::Player1:
				playersInfo[0].playerNum = (*p)->playerNum;
				playersInfo[0].totalSaboriTime = (*p)->totalSaboriTime ;
				break;

			case PlayerNum::Player2:
				playersInfo[1].playerNum = (*p)->playerNum;
				playersInfo[1].totalSaboriTime = (*p)->totalSaboriTime;
				break;

			case PlayerNum::Player3:
				playersInfo[2].playerNum = (*p)->playerNum;
				playersInfo[2].totalSaboriTime = (*p)->totalSaboriTime;
				break;

			case PlayerNum::Player4:
				playersInfo[3].playerNum = (*p)->playerNum;
				playersInfo[3].totalSaboriTime = (*p)->totalSaboriTime;
				break;
			}
		}

		//���v���ڂ莞�Ԃ���������playersInfo����ёւ�
		sort(playersInfo, playersInfo + sizeof(playersInfo) / sizeof(playersInfo[0]),
			[this](const PlayerInformation& playerInfoA, const PlayerInformation& playerInfoB) {return compare(playerInfoA, playerInfoB);});

		//���ʂ����߂�
		int currentRank = 1; //����鏇��
		for (int i = 0; i < sizeof(playersInfo) / sizeof(playersInfo[0]); ++i)
		{
			//2��ڈȍ~�̃��[�v�ŁA�X�R�A�̒l���O�̃X�R�A�Ɠ����łȂ���Γ���鏇�ʂ�+1����
			if (i > 0 && playersInfo[i].totalSaboriTime != playersInfo[i - 1].totalSaboriTime)
			{
				currentRank = i + 1;
			}

			//���ʂ�����
			playersInfo[i].rank = currentRank;
		}
	}
	//-------------------------------------------------------------------
	//playerInfoA��playerInfoB��totalSaboriTime�Ŕ�r���AplayerInfoA�̕����傫����true��Ԃ�
	bool Object::compare(const PlayerInformation& playerInfoA, const PlayerInformation& playerInfoB)
	{
		return playerInfoA.totalSaboriTime > playerInfoB.totalSaboriTime;
	}
	//-------------------------------------------------------------------
	//ge->score�ɓ��_�𑗂�
	void Object::SendScore()
	{
		for (int i = 0; i < sizeof(playersInfo) / sizeof(playersInfo[0]); ++i)
		{
			//���_���邩���߂�
			switch (playersInfo[i].rank)
			{
			case 1: //4�_
				//�v���C���[���ʗp�ԍ��łǂ��Ɋi�[���邩���߂�
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->score[0] += 4;
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->score[1] += 4;
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->score[2] += 4;
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->score[3] += 4;
					break;
				}
				break;

			case 2:	//3�_
				//�v���C���[���ʗp�ԍ��łǂ��Ɋi�[���邩���߂�
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->score[0] += 3;
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->score[1] += 3;
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->score[2] += 3;
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->score[3] += 3;
					break;
				}
				break;

			case 3:	//2�_
				//�v���C���[���ʗp�ԍ��łǂ��Ɋi�[���邩���߂�
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->score[0] += 2;
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->score[1] += 2;
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->score[2] += 2;
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->score[3] += 2;
					break;
				}
				break;

			case 4:	//1�_
				//�v���C���[���ʗp�ԍ��łǂ��Ɋi�[���邩���߂�
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->score[0] += 1;
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->score[1] += 1;
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->score[2] += 1;
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->score[3] += 1;
					break;
				}
				break;
			}
		}
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object()
		:
		gameStateChangeCount(0),
		timeLimit(30.f), //�������Ԃ�ݒ�
		isInGame(false),
		countToNextTask(0),
		gameState(GameState::BeforeGameStart),
		playersInfo{}
	{	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { Finalize(); }
}