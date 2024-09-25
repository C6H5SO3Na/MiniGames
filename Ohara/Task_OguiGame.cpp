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

		//���^�X�N�̐���
		//�v���C���[�^�X�N�쐬
		for (int i = 0; i < size(controllers); ++i)
		{
			auto p = OguiPlayer::Object::Create(true);	// �^�X�N����
			if (p) // null�`�F�b�N
			{
				p->pos = this->playerFirstPos[i];		// �v���C���̏����ʒu�ݒ�
				p->controller = this->controllers[i];	// �g�p�R���g���[���ݒ�(�R���g���[�����ڑ�����Ă��Ȃ��Ă����Ȃ�)
				//�v���C�����삩CPU���삩�̐ݒ�
				if (i < playerCount)
				{
					p->numberDecidePlayerType = 0;	// �v���C������ɐݒ�
				}
				else
				{
					p->numberDecidePlayerType = 1;	// CPU����ɐݒ�
				}
				p->playerNum = playersNum[i];	// �v���C���[���ʔԍ��ݒ�
			}
		}

		//�����Ǘ��^�X�N�쐬
		OguiFoodManager::Object::Create(true);

		//UI�Ǘ��^�X�N�쐬
		OguiUIManager::Object::Create(true);

		//�w�i�^�X�N�쐬
		OguiGameBG::Object::Create(true);

		//���C�[�W���O
		//�����摜�ړ��p
		//Ready�ړ��p
		easing::Set("GameRuleStart", easing::CIRCOUT, ge->screen2DWidth + 687.f, ge->screen2DWidth / 2.f, this->gameFps, "GameRuleEnd");
		easing::Set("GameRuleEnd", easing::CIRCIN, ge->screen2DWidth / 2.f, -687.f, this->gameFps);

		//Finish�ړ��p
		easing::Set("FinishStart", easing::CIRCOUT, ge->screen2DWidth + 438.f * 2.f, ge->screen2DWidth / 2.f, this->gameFps, "FinishEnd");
		easing::Set("FinishEnd", easing::CIRCIN, ge->screen2DWidth / 2.f, -438.f * 2.f, this->gameFps);

		//��BGM
		bgm::LoadFile("OguiGameBGM", "./data/sound/bgm/��H��_harunopayapaya.mp3");
		bgm::VolumeControl("OguiGameBGM", 90); // �}�W�b�N�i���o�[�͉���(0�`100�͈̔�)

		//��SE
		//Fight�`�掞�ɖ炷
		se::LoadFile("StartSE", "./data/sound/se/Common/�����J�n�̃S���O.wav");
		se::SetVolume("StartSE", 100); // �}�W�b�N�i���o�[�͉���(0�`100�͈̔�)

		//�Q�[���I�����ɖ炷
		se::LoadFile("FinishSE", "./data/sound/se/Common/�����I���̃S���O.wav");
		se::SetVolume("FinishSE", 100); // �}�W�b�N�i���o�[�͉���(0�`100�͈̔�)

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
		//�Q�[���̏�ԑJ��
		this->GameStateTransition();
		//��ԂɑΉ�����s������
		this->Work();
		//�C�[�W���O�𓮂���
		easing::UpDate();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//��ԂɑΉ�����`�揈��
		this->Render();
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
			if (this->countToChangeGameState >= gameFps) { nowState = GameState::Game; } //�Q�[������
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
		}
	}
	//-------------------------------------------------------------------
	//��Ԗ��̏���
	void Object::Work()
	{
		switch (this->gameState)
		{
		case GameState::BeforeGameStart:	//�Q�[���J�n�O
			//���Q�[�����n�܂����u�Ԃɍs������
			if (this->gameStart == true)
			{
				//���C�[�W���O�J�n
				easing::Start("GameRuleStart");

				this->gameStart = false;
			}

			//���C�[�W���O�ō��W�ړ�
			//Ready�𓮂���
			this->gameRuleImagePos.x = easing::GetPos("GameRuleStart");
			if (easing::GetState("GameRuleStart") == easing::EQ_STATE::EQ_END) //�C�[�W���O�uGameRuleStart�v���I�������
			{
				this->gameRuleImagePos.x = easing::GetPos("GameRuleEnd");
			}

			//��Fight�`��p����
			if (easing::GetState("GameRuleEnd") == easing::EQ_STATE::EQ_END) //�C�[�W���O�uGameRuleEnd�v���I�������
			{
				this->countToFightDraw++;
			}

			//��Fight�̕`�掞�ɍs������
			if (countToFightDraw == this->gameFps)
			{
				//�Q�[���J�n��SE��炷
				se::Play("StartSE");
			}

			//����Ԃ�J�ڂ��邽�߂̏���
			if (countToFightDraw >= this->gameFps) //Fight�̕`��Ɠ�����
			{
				this->countToChangeGameState++;
			}
			break;

		case GameState::Game:				//�Q�[����
			//���Q�[���{�҂��n�܂����u�Ԃɍs������
			if (this->isInGame == false)
			{
				//�Q�[�����J�n����
				this->isInGame = true;

				//BGM�X�^�[�g
				bgm::Play("OguiGameBGM");
			}

			//���������Ԃ����炷
			this->timeLimit -= 1.f / gameFps;

			//�������Ԃ�0������������0�ɂ���
			if (this->timeLimit < 0.f)
			{
				this->timeLimit = 0.f;
			}
			break;

		case GameState::End:				//�Q�[���I��
			//���Q�[���̏�Ԃ�End�̎��A��x�����s������
			if (this->isInGame == true)
			{
				//���Q�[�����I��������
				this->isInGame = false;

				//��SE���~�߂�
				se::AllStop();

				//�I��������SE��炷
				se::Play("FinishSE");

				//�����ʂ����߁Age->score�ɓ��_�𑗂�
				//���ʂ����߂�
				this->Ranking();
				//ge->score�ɓ��_�𑗂�
				this->SendScore();

				//���C�[�W���O�J�n
				easing::Start("FinishStart");//���C�[�W���O�J�n
				easing::Start("FinishStart");
			}

			//���C�[�W���O�ō��W�ړ�
			//Finish�𓮂���
			this->finishImagePos.x = easing::GetPos("FinishStart");
			if (easing::GetState("FinishStart") == easing::EQ_STATE::EQ_END) //�C�[�W���O�uFinishStart�v���I�������
			{
				this->finishImagePos.x = easing::GetPos("FinishEnd");
			}

			//�����̃^�X�N�ɍs���܂ł̃J�E���g
			if (easing::GetState("FinishEnd") == easing::EQ_STATE::EQ_END)  //�C�[�W���O�uFinishEnd�v���I�������
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
	//��Ԗ��̕`��
	void Object::Render()
	{
		ML::Box2D src = {};
		ML::Box2D draw = {};

		switch (this->gameState)
		{
		case GameState::BeforeGameStart:	//�Q�[���J�n�O
			//���uReady�v�`��
			//�`����ݒ�
			src = ML::Box2D(0, 0, 687, 88);
			draw = ML::Box2D(-src.w, -src.h, src.w * 2, src.h * 2);
			draw.Offset(this->gameRuleImagePos);

			this->res->gameRuleImage->Draw(draw, src);

			//���uFight�v�`��
			if (this->countToFightDraw >= this->gameFps)
			{
				//�`����ݒ�
				src = ML::Box2D(0, 0, 219, 95);
				draw = ML::Box2D(-110 * 3, -48 * 3, src.w * 3, src.h * 3);
				draw.Offset(this->fightImagePos);

				this->res->fightImage->Draw(draw, src);
			}
			break;

		case GameState::End:				//�Q�[���I��
			//���uFinish�v�`��
			src = ML::Box2D(0, 0, 438, 105);
			draw = ML::Box2D(-src.w, -48, src.w * 2, src.h * 2);
			draw.Offset(this->finishImagePos);

			this->res->finishImage->Draw(draw, src);
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
		gameState(GameState::BeforeGameStart),
		countToChangeGameState(0),
		timeLimit(15.f), //�������Ԃ�ݒ�
		isInGame(false),
		countToNextTask(0),
		playersInfo{},
		gameStart(true),
		countToFightDraw(0),
		gameFps(60),
		playerCount(0)
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