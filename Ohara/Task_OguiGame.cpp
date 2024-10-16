//-------------------------------------------------------------------
//大食いミニゲーム本編
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiGame.h"
#include  "Task_OguiPlayer.h"
#include  "Task_OguiFoodManager.h"
#include  "Task_OguiUIManager.h"
#include  "Task_OguiGameBG.h"

#include  "../Task_Game.h"
#include  "../randomLib.h"
#include  "../easing.h"
#include  "../sound.h"

namespace  OguiGame
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->gameRuleImage = DG::Image::Create("./data/image/OugiGameRuleSentence.png");
		this->fightImage = DG::Image::Create("./data/image/Fight.gif");
		this->finishImage = DG::Image::Create("./data/image/Finish.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->gameRuleImage.reset();
		this->fightImage.reset();
		this->finishImage.reset();
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
		//使用するコントローラーの設定
		useControllers = ge->players;

		//プレイ人数の設定
		playerCount = static_cast<int>(useControllers.size());
		//playerCountに不正な値が入った場合4を入れる
		if (playerCount < 1 || playerCount > 4)
		{
			playerCount = 4;
		}

		//制限時間の設定
		ge->nowTimeLimit = timeLimit;

		//★タスクの生成
		//プレイヤータスク作成
		//for(int i = 0; i < 4; ++i) // CPU実装時はこっちを使う
		for (int i = 0; i < playerCount; ++i)
		{
			auto p = OguiPlayer::Object::Create(true);	// タスク生成
			if (p) // nullチェック
			{
				p->pos = playerFirstPos[i];			// プレイヤーの初期位置設定
				p->controller = useControllers[i];	// 使用コントローラ設定(コントローラが接続されていなくても問題ない)
				p->playerNum = playersNum[i];		// プレイヤー識別番号設定

				////プレイヤ操作かCPU操作かの設定
				//if (i < playerCount)
				//{
				//	p->numberDecidePlayerType = 0;	// プレイヤ操作に設定
				//}
				//else
				//{
				//	p->numberDecidePlayerType = 1;	// CPU操作に設定
				//}
			}
		}

		//料理管理タスク作成
		OguiFoodManager::Object::Create(true);

		//UI管理タスク作成
		OguiUIManager::Object::Create(true);

		//背景タスク作成
		OguiGameBG::Object::Create(true);

		//☆BGM
		bgm::LoadFile("OguiGameBGM", "./data/sound/bgm/大食い_harunopayapaya.mp3");
		bgm::VolumeControl("OguiGameBGM", 90); // マジックナンバーは音量(0～100の範囲)

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("大食いゲーム本編");
		ge->KillAll_G("プレイヤー");
		ge->KillAll_G("ギミック");
		ge->KillAll_G("管理");
		ge->KillAll_G("大食いミニゲーム");

		if (!ge->QuitFlag() && nextTaskCreate) {
			bgm::Stop("OguiGameBGM");
			//★引き継ぎタスクの生成			
			Game::Object::CreateTask(6);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//状態に対応する行動処理
		this->Work();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//★自分で実装した関数★
	//-------------------------------------------------------------------
	//状態毎の処理
	void Object::Work()
	{
		switch (ge->gameState)
		{
		case MyPG::MyGameEngine::GameState::Game:	//ゲーム中
			//☆ゲーム本編が始まった瞬間に行う処理
			if (this->isInGame == false)
			{
				//ゲームを開始する
				this->isInGame = true;

				//BGMスタート
				bgm::Play("OguiGameBGM");
			}

			//☆制限時間を減らす
			ge->nowTimeLimit -= ge->c->deltaTime;

			//制限時間が0以下になったらゲームを終了させる
			if (ge->nowTimeLimit <= 0)
			{
				ge->nowTimeLimit = 0;

				//ゲームを終了させる
				ge->hasAllClearedGame = true;
			}
			break;

		case MyPG::MyGameEngine::GameState::Finish:	//ゲーム終了
			//☆ゲームの状態がEndの時、一度だけ行う処理
			if (this->isInGame == true)
			{
				//ゲームを終了させる
				this->isInGame = false;

				//全てのSEを止める
				se::AllStop();

				//☆順位を決め、ge->scoreに得点を送る
				//順位を決める
				this->Ranking();
				//ge->scoreに得点を送る
				this->SendScore();
			}

			//☆次のタスクに行くまでのカウント
			if (ge->hasFinishedEasing)  //Finishの描画が終わったら
			{
				this->countToNextTask++;
			}

			//☆統括タスク消滅申請
			if (this->countToNextTask == 1) {  
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
		auto players = ge->GetTasks<OguiPlayer::Object>("プレイヤー");
		if (players) // nullチェック
		{
			for (auto p = players->begin(); p != players->end(); ++p)
			{
				switch ((*p)->playerNum)
				{
				case PlayerNum::Player1:
					playersInfo[0].playerNum = (*p)->playerNum;
					playersInfo[0].eatFoodCount = (*p)->eatFoodCount;
					break;

				case PlayerNum::Player2:
					playersInfo[1].playerNum = (*p)->playerNum;
					playersInfo[1].eatFoodCount = (*p)->eatFoodCount;
					break;

				case PlayerNum::Player3:
					playersInfo[2].playerNum = (*p)->playerNum;
					playersInfo[2].eatFoodCount = (*p)->eatFoodCount;
					break;

				case PlayerNum::Player4:
					playersInfo[3].playerNum = (*p)->playerNum;
					playersInfo[3].eatFoodCount = (*p)->eatFoodCount;
					break;
				}
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
			if (i > 0 && playersInfo[i].eatFoodCount != playersInfo[i - 1].eatFoodCount)
			{
				currentRank = i + 1;
			}

			//順位を決定
			playersInfo[i].rank = currentRank;
		}
	}
	//-------------------------------------------------------------------
	//playerInfoAとplayerInfoBのeatFoodCountで比較し、playerInfoAの方が大きい時trueを返す
	bool Object::compare(const PlayerInformation& playerInfoA, const PlayerInformation& playerInfoB)
	{
		return playerInfoA.eatFoodCount > playerInfoB.eatFoodCount;
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
			case 1: //1位
				//プレイヤー識別用番号でどこに格納するか決める
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->AddScore(0, 4);
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->AddScore(1, 4);
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->AddScore(2, 4);
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->AddScore(3, 4);
					break;
				}
				break;

			case 2:	//2位
				//プレイヤー識別用番号でどこに格納するか決める
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->AddScore(0, 3);
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->AddScore(1, 3);
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->AddScore(2, 3);
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->AddScore(3, 3);
					break;
				}
				break;

			case 3:	//3位
				//プレイヤー識別用番号でどこに格納するか決める
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->AddScore(0, 2);
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->AddScore(1, 2);
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->AddScore(2, 2);
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->AddScore(3, 2);
					break;
				}
				break;

			case 4:	//4位
				//プレイヤー識別用番号でどこに格納するか決める
				switch (playersInfo[i].playerNum)
				{
				case PlayerNum::Player1:	//ge->score[0]
					ge->AddScore(0, 1);
					break;

				case PlayerNum::Player2:	//ge->score[1]
					ge->AddScore(1, 1);
					break;

				case PlayerNum::Player3:	//ge->score[2]
					ge->AddScore(2, 1);
					break;

				case PlayerNum::Player4:	//ge->score[3]
					ge->AddScore(3, 1);
					break;
				}
				break;
			}
		}
	}

	//getter関数----------------------------------------------------------
	// ゲームを遊ぶプレイヤーの人数の情報を渡す
	int Object::GetPlayerCount()
	{
		return playerCount;
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
		// プレイヤー関係
		playerFirstPos{ 
			{ ge->screen2DWidth / 8.f, ge->screen2DHeight / 2.f + 100.f },
			{ ge->screen2DWidth * 3.f / 8.f, ge->screen2DHeight / 2.f + 100.f },
			{ ge->screen2DWidth * 5.f / 8.f, ge->screen2DHeight / 2.f + 100.f },
			{ ge->screen2DWidth * 7.f / 8.f, ge->screen2DHeight / 2.f + 100.f } 
		}, 
		playersNum{ PlayerNum::Player1, PlayerNum::Player2, PlayerNum::Player3, PlayerNum::Player4 }, playerCount(4),
		playersInfo(),
		// 大食いゲーム関係
		timeLimit(15.f), isInGame(false), countToNextTask(0), gameStart(true)
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