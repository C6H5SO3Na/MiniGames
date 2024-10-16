//-------------------------------------------------------------------
//�T�{��~�j�Q�[���{��
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriGame.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriJoushi.h"
#include  "Task_SaboriUIManager.h"
#include  "Task_SaboriGameBG.h"

#include  "../Task_Game.h"
#include  "../randomLib.h"
#include  "../easing.h"
#include  "../sound.h"

namespace  SaboriGame
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->gameRuleImage = DG::Image::Create("./data/image/SaboriGameRuleSentence.png");
		this->fightImage = DG::Image::Create("./data/image/Fight.gif");
		this->finishImage = DG::Image::Create("./data/image/Finish.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->gameRuleImage.reset();
		this->fightImage.reset();
		this->finishImage.reset();
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
		//�g�p����R���g���[���[�̐ݒ�
		useControllers = ge->players;

		//�v���C�l���̐ݒ�
		playerCount = static_cast<int>(useControllers.size());
		//playerCount�ɕs���Ȓl���������ꍇ4������
		if (playerCount < 1 || playerCount > 4)
		{
			playerCount = 4;
		}

		//�������Ԃ̐ݒ�
		ge->nowTimeLimit = timeLimit;

		//���^�X�N�̐���
		//�v���C���[�^�X�N�쐬
		//for (int i = 0; i < 4; ++i) // CPU�������͂��������g��
		for (int i = 0; i < playerCount; ++i)
		{
			auto p = SaboriPlayer::Object::Create(true);
			p->pos = playerFirstPos[i];			// �v���C���[�̏����ʒu�ݒ�
			p->controller = useControllers[i];	// �g�p�R���g���[���ݒ�(�R���g���[�����ڑ�����Ă��Ȃ��Ă����Ȃ�)
			p->playerNum = playersNum[i];		// �v���C���[���ʔԍ��ݒ�
		}

		//��i�^�X�N�쐬
		auto j = SaboriJoushi::Object::Create(true);
		j->pos = joushiFirstPos;

		//UI�Ǘ��^�X�N�쐬
		SaboriUIManager::Object::Create(true);

		//�w�i�^�X�N�쐬
		SaboriGameBG::Object::Create(true);

		//��BGM�^�X�N�쐬
		bgm::LoadFile("SaboriGameBGM", "./data/sound/bgm/�T�{��_Short60_�������DIY_01.mp3");
		bgm::VolumeControl("SaboriGameBGM", 90);

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
		ge->KillAll_G("�T�{��~�j�Q�[��");

		if (!ge->QuitFlag() && nextTaskCreate) {
			//BGM�I��
			bgm::Stop("SaboriGameBGM");

			//�������p���^�X�N�̐���
			Game::Object::CreateTask(5);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		ge->c->DrawFps();
		//��ԂɑΉ�����s������
		this->Work();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//�������Ŏ��������֐���
	//-------------------------------------------------------------------
	//��Ԗ��̏���
	void Object::Work()
	{
		switch (ge->gameState)
		{
		case MyPG::MyGameEngine::GameState::Game:	//�Q�[����
			//���Q�[���{�҂��n�܂����u�Ԃɍs������
			if (this->isInGame == false)
			{
				//���Q�[�����J�n����
				this->isInGame = true;

				//��BGM�X�^�[�g
				bgm::Play("SaboriGameBGM");
			}

			//���������Ԃ����炷
			ge->nowTimeLimit -= ge->c->deltaTime;

			//�������Ԃ�0�ȉ��ɂȂ�����Q�[�����I��������
			if (ge->nowTimeLimit <= 0)
			{
				ge->nowTimeLimit = 0;

				//�Q�[�����I��������
				ge->hasAllClearedGame = true;
			}
			break;

		case MyPG::MyGameEngine::GameState::Finish:	//�Q�[���I��
			//���Q�[���̏�Ԃ�End�̎��A��x�����s������
			if (this->isInGame == true)
			{
				//�Q�[�����I��������
				this->isInGame = false;

				//�S�Ă�SE���~�߂�
				se::AllStop();

				//�����ʂ����߁Age->score�ɓ��_�𑗂�
				//���ʂ����߂�
				this->Ranking();
				//ge->score�ɓ��_�𑗂�
				this->SendScore();
			}

			//�����̃^�X�N�ɍs���܂ł̃J�E���g
			if (ge->hasFinishedEasing)  //Finish�̕`�悪�I�������
			{
				this->countToNextTask++;
			}

			//�������^�X�N���Ő\��
			if (this->countToNextTask == 1) {
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
			case 1: //1��
				//�v���C���[���ʗp�ԍ��łǂ��Ɋi�[���邩���߂�
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->AddScore(0, 4);
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->AddScore(1, 4);
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->AddScore(2, 4);
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->AddScore(3, 4);
					break;
				}
				break;

			case 2:	//2��
				//�v���C���[���ʗp�ԍ��łǂ��Ɋi�[���邩���߂�
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->AddScore(0, 3);
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->AddScore(1, 3);
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->AddScore(2, 3);
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->AddScore(3, 3);
					break;
				}
				break;

			case 3:	//3��
				//�v���C���[���ʗp�ԍ��łǂ��Ɋi�[���邩���߂�
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->AddScore(0, 2);
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->AddScore(1, 2);
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->AddScore(2, 2);
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->AddScore(3, 2);
					break;
				}
				break;

			case 4:	//4��
				//�v���C���[���ʗp�ԍ��łǂ��Ɋi�[���邩���߂�
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->AddScore(0, 1);
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->AddScore(1, 1);
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->AddScore(2, 1);
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->AddScore(3, 1);
					break;
				}
				break;
			}
		}
	}
	//-------------------------------------------------------------------
	//�Q�[����V�ԃv���C���[�̐l���̏���n��
	int Object::GetPlayerCount()
	{
		return playerCount;
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
		//�T�{��Q�[���֌W
		gameStart(true), countToNextTask(0), timeLimit(30.f), isInGame(false),
		//�v���C���[�֌W
		playerFirstPos{
			{ ge->screen2DWidth / 8.f, ge->screen2DHeight - 230.f },
			{ ge->screen2DWidth * 3.f / 8.f, ge->screen2DHeight - 230.f },
			{ ge->screen2DWidth * 5.f / 8.f, ge->screen2DHeight - 230.f },
			{ ge->screen2DWidth * 7.f / 8.f, ge->screen2DHeight - 230.f } 
		},
		playersNum{ PlayerNum::Player1, PlayerNum::Player2, PlayerNum::Player3, PlayerNum::Player4 }, playersInfo(),
		playerCount(4),
		//��i�֌W
		joushiFirstPos(ML::Vec2(ge->screen2DWidth / 2.f, ge->screen2DHeight / 3.f))
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