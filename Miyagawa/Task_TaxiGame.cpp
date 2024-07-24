//-------------------------------------------------------------------
//^NV[Q[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGame.h"

#include "../randomLib.h"

#include  "../Task_Ending.h"
#include  "Task_TaxiGamePlayer.h"
#include  "Task_TaxiGameTaxi.h"
#include  "Task_TaxiGameBG.h"

namespace  TaxiGame
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//\[Xฬ๐๚
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//u๚ปv^XNถฌษP๑พฏsค
	bool  Object::Initialize()
	{
		//X[p[NX๚ป
		__super::Initialize(defGroupName, defName, true);
		//\[XNXถฌor\[XคL
		res = Resource::Create();

		//f[^๚ป
		phase = Phase::Game;

		//^XNฬถฌ
		TaxiGameBG::Object::Create(true);

		//vC[ฬRg[[o^
		vector<XI::GamePad::SP> players;
		players.push_back(ge->in1);
		players.push_back(ge->in2);
		players.push_back(ge->in3);
		players.push_back(ge->in4);

		//vC[zu
		for (int i = 0; i < players.size(); ++i) {
			TaxiGamePlayer::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * (i + 1) / 5.f), players[i]);
		}

		//^NV[zu
		for (int i = 0; i < players.size(); ++i) {
			TaxiGameTaxi::Object::Spawn(ML::Vec2(200.f, ge->screenHeight * (1 + i) / 5.f));
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//uIนv^XNมลษP๑พฏsค
	bool  Object::Finalize()
	{
		//f[^^XN๐๚
		ge->KillAll_G("{า");
		ge->KillAll_G("^NV[");

		if (!ge->QuitFlag() && nextTaskCreate) {
			//๘ซpฌ^XNฬถฌ
			auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[ษsค
	void  Object::UpDate()
	{
		switch (phase) {
		case Phase::Game:
			Game();
			break;

		case Phase::Clear:
			Clear();
			break;
		}
	}
	//-------------------------------------------------------------------
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//Q[{าฬ
	void  Object::Game()
	{
		int clearNum = 0;
		auto players = ge->GetTasks<TaxiGamePlayer::Object>(TaxiGamePlayer::defGroupName, TaxiGamePlayer::defName);
		for_each(players->begin(), players->end(),
			[&](auto iter) {
				if (iter->IsClear()) {
					++clearNum;
				}
			});
		if (clearNum >= 4) {
			ge->StartCounter("Clear", 120);
			phase = Phase::Clear;
		}
	}
	//-------------------------------------------------------------------
	//S๕NAใฬ
	void  Object::Clear()
	{
		if (ge->getCounterFlag("Clear") == ge->LIMIT) {
			Kill();
		}
	}
	//
	//ศบอ๎{IษฯXsvศ\bh
	//
	//-------------------------------------------------------------------
	//^XNถฌ๛
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWษo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYษธsตฝ็Kill
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
	//\[XNXฬถฌ
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