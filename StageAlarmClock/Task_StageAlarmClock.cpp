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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->bgImg = DG::Image::Create("./data/image/heya_blue.jpg");
		ge->debugRectLoad();
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->bgImg.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//BGM
		bgm::LoadFile("stage1_bgm", "./data/sound/bgm/stage1_yutarisanbo2.mp3");
		bgm::Play("stage1_bgm");

		//★データ初期化
		this->render2D_Priority[1] = 0.9f;
		this->state = Phase::Game;
		this->timeCnt = 0;

		//★タスクの生成
		/*auto alarmclock = Clock::Object::Create(true);
		auto hand = hand::Object::Create(true);*/
		auto commonmanager = CommonItemManager01::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		//Game::Object::CreateTask(1);

		ge->KillAll_G("目覚まし時計");
		ge->KillAll_G("手");
		ge->KillAll_G("共通アイテムマネージャー01");

		bgm::Stop("stage1_bgm");
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			Game::Object::CreateTask(1);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		timeCnt++;
		switch (this->state) {
		case Phase::Game:
			CheckClear();
			if (timeCnt >= 20 * 60)
			{
				this->state = Phase::Clear;
			}
			break;

		case Phase::Clear:
			Clear();
			break;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw (0, 0, 1920/2, 1080/2);
		ML::Box2D src (0, 0, 1920, 1080);
		this->res->bgImg->Draw(draw, src);

		ML::Box2D draw2(1920 / 2, 0, 1920 / 2, 1080 / 2);
		this->res->bgImg->Draw(draw2, src);

		ML::Box2D draw3(0, 1080 / 2, 1920 / 2, 1080 / 2);
		this->res->bgImg->Draw(draw3, src);

		ML::Box2D draw4(1920 / 2, 1080 / 2, 1920 / 2, 1080 / 2);
		this->res->bgImg->Draw(draw4, src);

		ge->debugRectDraw();
	}
	//-------------------------------------------------------------------
	//ゲーム本編の処理
	void  Object::CheckClear()
	{
		int clearNum = 0;
		auto players = ge->GetTasks <hand::Object> ("手");
		for_each(players->begin(), players->end(), [&](auto iter) {
				if (iter->IsClear()) {

					++clearNum;
				}
			});
		if (clearNum >= 4) {
			//ge->StartCounter("Clear", 180);
			state = Phase::Clear;
		}
	}
	//-------------------------------------------------------------------
	//全員クリア後の処理
	void  Object::Clear()
	{
		Kill();
	/*	if (ge->getCounterFlag("Clear") == ge->LIMIT) {
			Kill();
		}*/
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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