//-------------------------------------------------------------------
//サボりミニゲーム本編
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

		//★タスクの生成
		//プレイヤー
		for (int i = 0; i < size(controllers); ++i)
		{
			auto p = SaboriPlayer::Object::Create(true);
			p->pos = this->playerFirstPos[i];
			p->controller = this->controllers[i];
			p->playerNum = playersNum[i];
		}

		//上司
		auto j = SaboriJoushi::Object::Create(true);
		j->pos = joushiFirstPos;

		//UI管理
		SaboriUIManager::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("サボりゲーム本編");
		ge->KillAll_G("プレイヤー");
		ge->KillAll_G("ギミック");
		ge->KillAll_G("管理");

		if (!ge->QuitFlag() && nextTaskCreate) {
			//★引き継ぎタスクの生成
			Game::Object::CreateTask(5);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->gameStateChangeCount++;

		//ゲームの状態遷移
		this->GameStateTransition();
		//状態に対応する行動処理
		this->Work();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//ゲームの状態遷移
	void Object::GameStateTransition()
	{
		GameState nowState = this->gameState;	//とりあえず現在の状態を代入

		//モーションの切り替え
		switch (nowState)
		{
		case GameState::BeforeGameStart:	//ゲーム開始前
			if (gameStateChangeCount >= 60 * 5) { nowState = GameState::Game; } //ゲーム中へ
			break;

		case GameState::Game:				//ゲーム中
			if (timeLimit == 0) { nowState = GameState::Result; } //制限時間が0になったらリザルトへ
			break;
		}

		//状態更新
		this->UpdateGameState(nowState);
	}
	//-------------------------------------------------------------------
	//ゲームの状態変更時処理
	void Object::UpdateGameState(GameState nowState)
	{
		if (nowState != this->gameState)
		{
			this->gameState = nowState;
			this->gameStateChangeCount = 0;
		}
	}
	//-------------------------------------------------------------------
	//状態毎の処理
	void Object::Work()
	{
		switch (this->gameState)
		{
		case GameState::BeforeGameStart:	//ゲーム開始前
			break;

		case GameState::Game:				//ゲーム中
			//☆ゲームを開始する
			if (this->isInGame == false)
			{
				this->isInGame = true;
			}

			//☆制限時間を減らす
			this->timeLimit -= 1.f / 60; // / 60 を / GetFps()に変更してモニターFPSにゲームが依存しないようにする

			//制限時間が0未満だったら0にする
			if (this->timeLimit < 0.f)
			{
				this->timeLimit = 0.f;
			}
			break;

		case GameState::Result:
			//☆ゲームを終了させる
			if (this->isInGame == true)
			{
				this->isInGame = false;
			}

			//☆次のタスクに行くまでのカウント
			this->countToNextTask++;

			//☆統括タスク消滅申請
			if (this->countToNextTask == 60 * 10) { //60をGetFps()にする 
				ge->StartCounter("test", 0);
			}
			if (ge->getCounterFlag("test") == ge->LIMIT) {
				Kill();
			}
			break;
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
	Object::Object()
		:
		gameStateChangeCount(0),
		timeLimit(30.f), //制限時間を設定
		isInGame(false),
		countToNextTask(0),
		gameState(GameState::BeforeGameStart)
	{	}
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