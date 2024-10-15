//-------------------------------------------------------------------
//�^�N�V�[�Q�[��
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGame.h"

#include "../randomLib.h"

#include  "Task_TaxiGamePlayer.h"
#include  "Task_TaxiGameTaxi.h"
#include  "Task_TaxiGameBG.h"
#include  "../Task_Game.h"
#include  "../sound.h"

namespace  TaxiGame
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
		TaxiGamePlayer::Object::playerScore = 4;
		ge->nowTimeLimit = 20 * 60;

		//BGM
		bgm::LoadFile("TaxiGame", "./data/sound/bgm/�^�N�V�[_retrogamecenter3.mp3");
		bgm::Play("TaxiGame");
		//���^�X�N�̐���
		TaxiGameBG::Object::Create(true);

		//�v���C���[���̃R���g���[���[�o�^
		vector<XI::GamePad::SP> players;
		players.push_back(ge->in1);
		players.push_back(ge->in2);
		//players.push_back(ge->in3);
		//players.push_back(ge->in4);

		//�v���C���[�z�u
		for (int i = 0; i < players.size(); ++i) {
			TaxiGamePlayer::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * (i + 1) / 5.f), players[i]);
		}

		//�^�N�V�[�z�u
		for (int i = 0; i < players.size(); ++i) {
			TaxiGameTaxi::Object::Spawn(ML::Vec2(200.f, ge->screenHeight * (1 + i) / 5.f));
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�{��");
		ge->KillAll_G("�^�N�V�[");

		if (!ge->QuitFlag() && nextTaskCreate) {
			bgm::AllStop();
			//�������p���^�X�N�̐���
			//�Ō�̃Q�[�����琶�����Ȃ�
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		switch (ge->gameState) {
		case MyPG::MyGameEngine::GameState::Start:
			break;

		case MyPG::MyGameEngine::GameState::Game:
			Game();
			break;

		case MyPG::MyGameEngine::GameState::Finish:
			Clear();
			break;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//�Q�[���{�҂̏���
	void  Object::Game()
	{
		--ge->nowTimeLimit;
		if (ge->nowTimeLimit == 0) {
			ge->hasAllClearedGame = true;
		}
		int clearNum = 0;
		auto players = ge->GetTasks<TaxiGamePlayer::Object>(TaxiGamePlayer::defGroupName, TaxiGamePlayer::defName);
		for_each(players->begin(), players->end(),
			[&](auto iter) {
				if (iter->IsClear()) {
					++clearNum;
				}
			});
		//�N���A
		if (clearNum >= players->size()) {
			ge->hasAllClearedGame = true;
		}
	}
	//-------------------------------------------------------------------
	//�S���N���A��̏���
	void  Object::Clear()
	{
		if (ge->hasFinishedEasing) {
			Kill();
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
	Object::Object() {	}
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