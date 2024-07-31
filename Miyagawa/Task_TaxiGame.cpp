//-------------------------------------------------------------------
//タクシーゲーム
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		res = Resource::Create();

		//★データ初期化
		phase = Phase::Game;
		TaxiGamePlayer::Object::playerScore = 4;

		//BGM
		bgm::LoadFile("TaxiGame", "./data/sound/bgm/タクシー_retrogamecenter3.mp3");
		bgm::Play("TaxiGame");
		//★タスクの生成
		TaxiGameBG::Object::Create(true);

		//プレイヤー毎のコントローラー登録
		vector<XI::GamePad::SP> players;
		players.push_back(ge->in1);
		players.push_back(ge->in2);
		players.push_back(ge->in3);
		players.push_back(ge->in4);

		//プレイヤー配置
		for (int i = 0; i < players.size(); ++i) {
			TaxiGamePlayer::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * (i + 1) / 5.f), players[i]);
		}

		//タクシー配置
		for (int i = 0; i < players.size(); ++i) {
			TaxiGameTaxi::Object::Spawn(ML::Vec2(200.f, ge->screenHeight * (1 + i) / 5.f));
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("本編");
		ge->KillAll_G("タクシー");

		if (!ge->QuitFlag() && nextTaskCreate) {
			bgm::AllStop();
			//★引き継ぎタスクの生成
			//最後のゲームから生成しない
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
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
		gameCnt++;//ゲーム時間のカウンター（フレーム）
		if (gameCnt == 1800) {
			ge->StartCounter("Clear", 180);
			phase = Phase::Clear;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//ゲーム本編の処理
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
			ge->StartCounter("Clear", 180);
			phase = Phase::Clear;
		}
	}
	//-------------------------------------------------------------------
	//全員クリア後の処理
	void  Object::Clear()
	{
		if (ge->getCounterFlag("Clear") == ge->LIMIT) {
			Kill();
		}
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
	Resource::~Resource() { Finalize(); }
}