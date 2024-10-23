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
		//this->bgImg = DG::Image::Create("./data/image/heya_blue.jpg");
		this->controllerMark = DG::Image::Create("./data/image/LeftStickDown_new.png");
		//this->PlayerNum = DG::Image::Create("./data/image/PlayerNumber.png");
		ge->debugRectLoad();
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		//this->bgImg.reset();
		this->controllerMark.reset();
		//this->PlayerNum.reset();
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
		//this->state = Phase::Game;
		this->timeCnt = 10 * 60;
		ge->nowTimeLimit = this->timeCnt;
		/*this->animCnt = 0;*/

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
		ge->KillAll_G("ステージ目覚まし時計");

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
		ge->nowTimeLimit = this->timeCnt;
		animCnt++;
		//アニメ更新
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
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		////背景
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
			//コントローラーマーク
			ML::Box2D Draw(1920 / 2 - 150 / 2, 1080 / 2 - 150 / 2, 150, 150);
			int srcX = animIndex % 2 * 128;
			int srcY = animIndex / 2 * 128;
			ML::Box2D Src(srcX, srcY, 128, 128);
			this->res->controllerMark->Draw(Draw, Src);
			break;
		}
		

		////プレイヤーナンバー
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
	//ゲーム本編の処理
	void  Object::CheckClear()
	{
		int clearNum = 0;
		auto players = ge->GetTasks <hand::Object> ("手");
		auto com = ge->GetTask<CommonItemManager01::Object>("共通アイテムマネージャー01");
		auto c = ge->GetTasks<Clock::Object>("目覚まし時計");
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
	//全員クリア後の処理
	void  Object::MarkCount()
	{
		auto players = ge->GetTasks <hand::Object>("手");
		auto com = ge->GetTask<CommonItemManager01::Object>("共通アイテムマネージャー01");
		auto c = ge->GetTasks<Clock::Object>("目覚まし時計");
		for_each(players->begin(), players->end(), [&](auto iter) {
				if (iter->isScoreAdd == false) {
					ge->AddScore(iter->id, com->addscore[com->rank]);
					iter->isScoreAdd = true;
				}
			}
		);
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