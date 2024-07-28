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
			if (gameStateChangeCount >= 60 * 2) { nowState = GameState::Game; } //ゲーム中へ
			break;

		case GameState::Game:				//ゲーム中
			if (timeLimit == 0) { nowState = GameState::End; } //制限時間が0になったらゲーム終了へ
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

		case GameState::End:				//ゲーム終了
			//ゲームの状態がEndの時、一度だけ行う処理
			if (this->isInGame == true)
			{
				//☆ゲームを終了させる
				this->isInGame = false;

				//☆順位を決め、ge->scoreに得点を送る
				//順位を決める
				this->Ranking();
				//ge->scoreに得点を送る
				this->SendScore();
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
	//-------------------------------------------------------------------
	//順位決めの処理
	void Object::Ranking()
	{
		//プレイヤーから情報を取得する
		auto players = ge->GetTasks<SaboriPlayer::Object>("プレイヤー");
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			switch ((*p)->playerNum)
			{
			case PlayerNum::Player1:
				playersInfo[0].playerNum = (*p)->playerNum;
				playersInfo[0].totalSaboriTime = (*p)->totalSaboriTime ;
				break;

			case PlayerNum::Player2:
				playersInfo[1].playerNum = (*p)->playerNum;
				playersInfo[1].totalSaboriTime = (*p)->totalSaboriTime;
				break;

			case PlayerNum::Player3:
				playersInfo[2].playerNum = (*p)->playerNum;
				playersInfo[2].totalSaboriTime = (*p)->totalSaboriTime;
				break;

			case PlayerNum::Player4:
				playersInfo[3].playerNum = (*p)->playerNum;
				playersInfo[3].totalSaboriTime = (*p)->totalSaboriTime;
				break;
			}
		}

		//合計さぼり時間が多い順にplayersInfoを並び替え
		sort(playersInfo, playersInfo + sizeof(playersInfo) / sizeof(playersInfo[0]),
			[this](const PlayerInformation& playerInfoA, const PlayerInformation& playerInfoB) {return compare(playerInfoA, playerInfoB);});

		//順位を決める
		int currentRank = 1; //入れる順位
		for (int i = 0; i < sizeof(playersInfo) / sizeof(playersInfo[0]); ++i)
		{
			//2回目以降のループで、スコアの値が前のスコアと同じでなければ入れる順位を+1する
			if (i > 0 && playersInfo[i].totalSaboriTime != playersInfo[i - 1].totalSaboriTime)
			{
				currentRank = i + 1;
			}

			//順位を決定
			playersInfo[i].rank = currentRank;
		}
	}
	//-------------------------------------------------------------------
	//playerInfoAとplayerInfoBのtotalSaboriTimeで比較し、playerInfoAの方が大きい時trueを返す
	bool Object::compare(const PlayerInformation& playerInfoA, const PlayerInformation& playerInfoB)
	{
		return playerInfoA.totalSaboriTime > playerInfoB.totalSaboriTime;
	}
	//-------------------------------------------------------------------
	//ge->scoreに得点を送る
	void Object::SendScore()
	{
		for (int i = 0; i < sizeof(playersInfo) / sizeof(playersInfo[0]); ++i)
		{
			//何点送るか決める
			switch (playersInfo[i].rank)
			{
			case 1: //4点
				//プレイヤー識別用番号でどこに格納するか決める
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->score[0] += 4;
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->score[1] += 4;
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->score[2] += 4;
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->score[3] += 4;
					break;
				}
				break;

			case 2:	//3点
				//プレイヤー識別用番号でどこに格納するか決める
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->score[0] += 3;
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->score[1] += 3;
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->score[2] += 3;
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->score[3] += 3;
					break;
				}
				break;

			case 3:	//2点
				//プレイヤー識別用番号でどこに格納するか決める
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->score[0] += 2;
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->score[1] += 2;
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->score[2] += 2;
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->score[3] += 2;
					break;
				}
				break;

			case 4:	//1点
				//プレイヤー識別用番号でどこに格納するか決める
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->score[0] += 1;
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->score[1] += 1;
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->score[2] += 1;
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->score[3] += 1;
					break;
				}
				break;
			}
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
		gameState(GameState::BeforeGameStart),
		playersInfo{}
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