//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "StageAlarmClock/Task_StageAlarmClock.h"
#include  "StageBrushTeeth/Task_StageBrushTeeth.h"
#include  "BlanceGame/Task_BlanceGame.h"
#include  "ClassifyGame/Task_ClassifyGame.h"
#include  "Ohara/Task_OguiGame.h"
#include  "Ohara/Task_SaboriGame.h"
#include  "Miyagawa/Task_TaxiGame.h"

#include  "Miyagawa/Task_UIManager.h"

#include  <chrono>
#include  <thread>

#include  "randomLib.h"
#include  "easing.h"
#include  "Ohara/Task_Result.h"


namespace  Game
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
		//reset score
		for (int i = 0; i < 4; ++i) {
			//ge->score[i] = 0;
			ge->SetScore(i, 0);
		}
		//�f�o�b�O�p�t�H���g�̏���
		TestFont = DG::Font::Create("�l�r �S�V�b�N", 30, 30);

		//���^�X�N�̐���
		CreateTask(5);//�����ɐ���������ƁA1:�������Q�[���A2:�d�ԃQ�[���c�Ƃ������ɂȂ�
		UIManager::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�{��");

		if (!ge->QuitFlag() && nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto next = Result::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		easing::UpDate();
		if (ge->hasAllClearedGame) {
			ge->gameState = MyPG::MyGameEngine::GameState::Finish;
		}
	}
	//-------------------------------------------------------------------
	//�^�X�N�̐���
	//�����Ȃ��Ȃ�ŏ��̃^�X�N�𐶐�
	void  Object::CreateTask() {
		StageAlarmClock::Object::Create(true);
		UIManager::Object::CreateTimeLimitBar(ML::Vec2(1000.f, 1000.f), 20 * 60);
	}
	void  Object::CreateTask(int nextTask) {
		ge->nowStage = nextTask;

		int timeLimitTable[] = {
			20 * 60, 20 * 60, 22 * 60, 22 * 60, 30 * 60, 15 * 60, 30 * 60,
		};
		//auto gsUI=gamesetUI::Object::Create(true);  //�Q�[���Z�b�gUI�����p
		/*std::this_thread::sleep_for(std::chrono::seconds(2));*/
		auto g = ge->GetTask<Game::Object>("�{��");
		switch (ge->nowStage) {
		case 1:
			StageBrushTeeth::Object::Create(true);
			ge->gameState = MyPG::MyGameEngine::GameState::Start;
			UIManager::Object::CreateTimeLimitBar(ML::Vec2(1000.f, 1000.f), timeLimitTable[ge->nowStage]);
			break;
		case 2:
			BlanceGame::Object::Create(true);
			ge->gameState = MyPG::MyGameEngine::GameState::Start;
			UIManager::Object::CreateTimeLimitBar(ML::Vec2(1000.f, 1000.f), timeLimitTable[ge->nowStage]);
			break;
		case 3:
			ClassifyGame::Object::Create(true);
			ge->gameState = MyPG::MyGameEngine::GameState::Start;
			UIManager::Object::CreateTimeLimitBar(ML::Vec2(1000.f, 1000.f), timeLimitTable[ge->nowStage]);
			break;
		case 4:
			SaboriGame::Object::Create(true);
			ge->gameState = MyPG::MyGameEngine::GameState::Start;
			UIManager::Object::CreateTimeLimitBar(ML::Vec2(1000.f, 1000.f), timeLimitTable[ge->nowStage]);
			break;
		case 5:
			OguiGame::Object::Create(true);
			ge->gameState = MyPG::MyGameEngine::GameState::Start;
			UIManager::Object::CreateTimeLimitBar(ML::Vec2(1000.f, 1000.f), timeLimitTable[ge->nowStage]);
			break;
		case 6:
			TaxiGame::Object::Create(true);
			ge->gameState = MyPG::MyGameEngine::GameState::Start;
			UIManager::Object::CreateTimeLimitBar(ML::Vec2(1000.f, 1000.f), timeLimitTable[ge->nowStage]);
			break;
		default:
			g->Kill();
			break;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

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