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
			if (gameStateChangeCount >= 60 * 5) { nowState = GameState::Game; } //�Q�[������
			break;

		case GameState::Game:				//�Q�[����
			if (timeLimit == 0) { nowState = GameState::Result; } //�������Ԃ�0�ɂȂ����烊�U���g��
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

		case GameState::Result:
			//���Q�[�����I��������
			if (this->isInGame == true)
			{
				this->isInGame = false;
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
		gameState(GameState::BeforeGameStart)
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