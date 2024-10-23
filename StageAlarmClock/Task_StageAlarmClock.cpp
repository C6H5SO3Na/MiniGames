//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_StageAlarmClock.h"
#include  "Task_Clock.h"
#include  "Task_hand.h"
#include  "Task_CommonItemManager01.h"
#include  "../Task_Game.h"
#include "../sound.h"


namespace  StageAlarmClock
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//this->bgImg = DG::Image::Create("./data/image/heya_blue.jpg");
		this->controllerMark = DG::Image::Create("./data/image/LeftStickDown_new.png");
		//this->PlayerNum = DG::Image::Create("./data/image/PlayerNumber.png");
		ge->debugRectLoad();
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		//this->bgImg.reset();
		this->controllerMark.reset();
		//this->PlayerNum.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//BGM
		bgm::LoadFile("stage1_bgm", "./data/sound/bgm/stage1_yutarisanbo2.mp3");
		bgm::Play("stage1_bgm");

		//���f�[�^������
		this->render2D_Priority[1] = 0.9f;
		//this->state = Phase::Game;
		this->timeCnt = 10 * 60;
		ge->nowTimeLimit = this->timeCnt;
		/*this->animCnt = 0;*/

		//���^�X�N�̐���
		/*auto alarmclock = Clock::Object::Create(true);
		auto hand = hand::Object::Create(true);*/
		auto commonmanager = CommonItemManager01::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		//Game::Object::CreateTask(1);

		ge->KillAll_G("�ڊo�܂����v");
		ge->KillAll_G("��");
		ge->KillAll_G("���ʃA�C�e���}�l�[�W���[01");
		ge->KillAll_G("�X�e�[�W�ڊo�܂����v");

		bgm::Stop("stage1_bgm");
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			Game::Object::CreateTask(1);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		ge->nowTimeLimit = this->timeCnt;
		animCnt++;
		//�A�j���X�V
		if (this->animCnt >= 15)
		{
			this->animCnt = 0;
			this->animIndex++;
			if (this->animIndex >= 2)
			{
				this->animIndex = 0;
			}
		}

		switch (ge->gameState) {
		case MyPG::MyGameEngine::GameState::Game:
			timeCnt--;
			CheckClear();
			if (timeCnt <= 0)
			{
				MarkCount();
				ge->hasAllClearedGame = true;
			}
			break;

		case MyPG::MyGameEngine::GameState::Finish:
			if (ge->hasFinishedEasing)
			{
				Kill();
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		////�w�i
		//ML::Box2D draw (0, 0, 1920/2, 1080/2);
		//ML::Box2D src (0, 0, 1920, 1080);
		//this->res->bgImg->Draw(draw, src);

		//ML::Box2D draw2(1920 / 2, 0, 1920 / 2, 1080 / 2);
		//this->res->bgImg->Draw(draw2, src);

		//ML::Box2D draw3(0, 1080 / 2, 1920 / 2, 1080 / 2);
		//this->res->bgImg->Draw(draw3, src);

		//ML::Box2D draw4(1920 / 2, 1080 / 2, 1920 / 2, 1080 / 2);
		//this->res->bgImg->Draw(draw4, src);

		switch (ge->gameState)
		{
		case MyPG::MyGameEngine::GameState::Game:
			//�R���g���[���[�}�[�N
			ML::Box2D Draw(1920 / 2 - 150 / 2, 1080 / 2 - 150 / 2, 150, 150);
			int srcX = animIndex % 2 * 128;
			int srcY = animIndex / 2 * 128;
			ML::Box2D Src(srcX, srcY, 128, 128);
			this->res->controllerMark->Draw(Draw, Src);
			break;
		}
		

		////�v���C���[�i���o�[
		//ML::Box2D draw01(0, 1080/2 - 105, 715/4, 105);
		//ML::Box2D src01(0, 0, 715/4 - 20, 105);
		//this->res->PlayerNum->Draw(draw01, src01);

		//ML::Box2D draw02(1920 - 715 / 4 -10, 1080 / 2 - 105, 715 / 4, 105);
		//ML::Box2D src02(715 / 4 - 20, 0, 715 / 4, 105);
		//this->res->PlayerNum->Draw(draw02, src02);

		//ML::Box2D draw03(0, 1080 - 105 - 10, 715 / 4, 105);
		//ML::Box2D src03(715 / 2 - 10, 0, 715 / 4, 105);
		//this->res->PlayerNum->Draw(draw03, src03);

		//ML::Box2D draw04(1920 - 715 / 4 - 10, 1080 - 105 - 10, 715 / 4, 105);
		//ML::Box2D src04(715 / 4 * 3 - 10, 0, 715 / 4, 105);
		//this->res->PlayerNum->Draw(draw04, src04);

		ge->debugRectDraw();
	}
	//-------------------------------------------------------------------
	//�Q�[���{�҂̏���
	void  Object::CheckClear()
	{
		int clearNum = 0;
		auto players = ge->GetTasks <hand::Object> ("��");
		auto com = ge->GetTask<CommonItemManager01::Object>("���ʃA�C�e���}�l�[�W���[01");
		auto c = ge->GetTasks<Clock::Object>("�ڊo�܂����v");
		for_each(players->begin(), players->end(), [&](auto iter) {
			if (iter->IsClear()) {
				++clearNum;
				if (!iter->isScoreAdd) {
					ge->AddScore(iter->id, com->addscore[com->rank]);
					com->rank++;
					iter->isScoreAdd = true;
				}
			}
		});
		if (clearNum == ge->players.size()) {
			ge->hasAllClearedGame = true;
			//state = Phase::Clear;
		}
		else
		{
			clearNum = 0;// if not all players were clear this frame, reset clearcount
		}
	}
	//-------------------------------------------------------------------
	//�S���N���A��̏���
	void  Object::MarkCount()
	{
		auto players = ge->GetTasks <hand::Object>("��");
		auto com = ge->GetTask<CommonItemManager01::Object>("���ʃA�C�e���}�l�[�W���[01");
		auto c = ge->GetTasks<Clock::Object>("�ڊo�܂����v");
		for_each(players->begin(), players->end(), [&](auto iter) {
				if (iter->isScoreAdd == false) {
					ge->AddScore(iter->id, com->addscore[com->rank]);
					iter->isScoreAdd = true;
				}
			}
		);
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
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
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
	Resource::~Resource() { this->Finalize(); }
}