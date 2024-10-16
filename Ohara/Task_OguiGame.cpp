//-------------------------------------------------------------------
//��H���~�j�Q�[���{��
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiGame.h"
#include  "Task_OguiPlayer.h"
#include  "Task_OguiFoodManager.h"
#include  "Task_OguiUIManager.h"
#include  "Task_OguiGameBG.h"

#include  "../Task_Game.h"
#include  "../randomLib.h"
#include  "../easing.h"
#include  "../sound.h"

namespace  OguiGame
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->gameRuleImage = DG::Image::Create("./data/image/OugiGameRuleSentence.png");
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
		//for(int i = 0; i < 4; ++i) // CPU�������͂��������g��
		for (int i = 0; i < playerCount; ++i)
		{
			auto p = OguiPlayer::Object::Create(true);	// �^�X�N����
			if (p) // null�`�F�b�N
			{
				p->pos = playerFirstPos[i];			// �v���C���[�̏����ʒu�ݒ�
				p->controller = useControllers[i];	// �g�p�R���g���[���ݒ�(�R���g���[�����ڑ�����Ă��Ȃ��Ă����Ȃ�)
				p->playerNum = playersNum[i];		// �v���C���[���ʔԍ��ݒ�

				////�v���C�����삩CPU���삩�̐ݒ�
				//if (i < playerCount)
				//{
				//	p->numberDecidePlayerType = 0;	// �v���C������ɐݒ�
				//}
				//else
				//{
				//	p->numberDecidePlayerType = 1;	// CPU����ɐݒ�
				//}
			}
		}

		//�����Ǘ��^�X�N�쐬
		OguiFoodManager::Object::Create(true);

		//UI�Ǘ��^�X�N�쐬
		OguiUIManager::Object::Create(true);

		//�w�i�^�X�N�쐬
		OguiGameBG::Object::Create(true);

		//��BGM
		bgm::LoadFile("OguiGameBGM", "./data/sound/bgm/��H��_harunopayapaya.mp3");
		bgm::VolumeControl("OguiGameBGM", 90); // �}�W�b�N�i���o�[�͉���(0�`100�͈̔�)

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("��H���Q�[���{��");
		ge->KillAll_G("�v���C���[");
		ge->KillAll_G("�M�~�b�N");
		ge->KillAll_G("�Ǘ�");
		ge->KillAll_G("��H���~�j�Q�[��");

		if (!ge->QuitFlag() && nextTaskCreate) {
			bgm::Stop("OguiGameBGM");
			//�������p���^�X�N�̐���			
			Game::Object::CreateTask(6);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
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
				//�Q�[�����J�n����
				this->isInGame = true;

				//BGM�X�^�[�g
				bgm::Play("OguiGameBGM");
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
		auto players = ge->GetTasks<OguiPlayer::Object>("�v���C���[");
		if (players) // null�`�F�b�N
		{
			for (auto p = players->begin(); p != players->end(); ++p)
			{
				switch ((*p)->playerNum)
				{
				case PlayerNum::Player1:
					playersInfo[0].playerNum = (*p)->playerNum;
					playersInfo[0].eatFoodCount = (*p)->eatFoodCount;
					break;

				case PlayerNum::Player2:
					playersInfo[1].playerNum = (*p)->playerNum;
					playersInfo[1].eatFoodCount = (*p)->eatFoodCount;
					break;

				case PlayerNum::Player3:
					playersInfo[2].playerNum = (*p)->playerNum;
					playersInfo[2].eatFoodCount = (*p)->eatFoodCount;
					break;

				case PlayerNum::Player4:
					playersInfo[3].playerNum = (*p)->playerNum;
					playersInfo[3].eatFoodCount = (*p)->eatFoodCount;
					break;
				}
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
			if (i > 0 && playersInfo[i].eatFoodCount != playersInfo[i - 1].eatFoodCount)
			{
				currentRank = i + 1;
			}

			//���ʂ�����
			playersInfo[i].rank = currentRank;
		}
	}
	//-------------------------------------------------------------------
	//playerInfoA��playerInfoB��eatFoodCount�Ŕ�r���AplayerInfoA�̕����傫����true��Ԃ�
	bool Object::compare(const PlayerInformation& playerInfoA, const PlayerInformation& playerInfoB)
	{
		return playerInfoA.eatFoodCount > playerInfoB.eatFoodCount;
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

	//getter�֐�----------------------------------------------------------
	// �Q�[����V�ԃv���C���[�̐l���̏���n��
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
		// �v���C���[�֌W
		playerFirstPos{ 
			{ ge->screen2DWidth / 8.f, ge->screen2DHeight / 2.f + 100.f },
			{ ge->screen2DWidth * 3.f / 8.f, ge->screen2DHeight / 2.f + 100.f },
			{ ge->screen2DWidth * 5.f / 8.f, ge->screen2DHeight / 2.f + 100.f },
			{ ge->screen2DWidth * 7.f / 8.f, ge->screen2DHeight / 2.f + 100.f } 
		}, 
		playersNum{ PlayerNum::Player1, PlayerNum::Player2, PlayerNum::Player3, PlayerNum::Player4 }, playerCount(4),
		playersInfo(),
		// ��H���Q�[���֌W
		timeLimit(15.f), isInGame(false), countToNextTask(0), gameStart(true)
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