//-------------------------------------------------------------------
//^NV[Q[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGame.h"

#include "../randomLib.h"

#include  "../Task_Ending.h"
#include  "Task_TaxiGamePlayer.h"
#include  "Task_TaxiGameTaxi.h"

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


		//^XNฬถฌ
		TaxiGamePlayer::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 1 / 5.f), ge->in1);
		TaxiGamePlayer::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 2 / 5.f), ge->in2);
		TaxiGamePlayer::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 3 / 5.f), ge->in3);
		TaxiGamePlayer::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 4 / 5.f), ge->in4);

		for (int i = 0; i < 4; ++i) {
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
		auto inp = ge->in1->GetState();
		if (inp.B1.down) {
			ge->CreateEffect(99,{0,0});
		}
		if (inp.B2.down) {
			ge->CreateEffect(98, { 0,0 });
		}
		int n = 0;
		auto players = ge->GetTasks<TaxiGamePlayer::Object>(TaxiGamePlayer::defGroupName);
		for_each(players->begin(), players->end(),
			[&](auto iter) {
				if (iter->IsClear()) {
					++n;
				}
			});
		if (n >= 4) {
			Kill();
		}
	}
	//-------------------------------------------------------------------
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
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