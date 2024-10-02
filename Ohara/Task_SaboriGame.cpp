//-------------------------------------------------------------------
//サボりミニゲーム本編
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriGame.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriJoushi.h"
#include  "Task_SaboriUIManager.h"
#include  "Task_SaboriGameBG.h"

#include  "../Task_Game.h"
#include  "../randomLib.h"
#include  "../easing.h"
#include  "../sound.h"

namespace  SaboriGame
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->gameRuleImage = DG::Image::Create("./data/image/SaboriGameRuleSentence.png");
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
		//playerCountに不正な値が入った場合4を入れる
		if (playerCount < 1 || playerCount > 4)
		{
			playerCount = 4;
		}

		//使用するコントローラーの設定
		for (int i = 0; i < playerCount; ++i)
		{
			useControllers.push_back(controllers[i]);
		}

		//★タスクの生成
		//プレイヤー
		//for (int i = 0; i < 4; ++i) // CPU実装時はこっちを使う
		for (int i = 0; i < useControllers.size(); ++i)
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

		//背景
		SaboriGameBG::Object::Create(true);

		//☆イージング
		//文字画像移動用
		//Ready移動用
		easing::Set("GameRuleStart", easing::CIRCOUT, ge->screen2DWidth + 756.f, ge->screen2DWidth / 2.f, this->gameFps, "GameRuleEnd");
		easing::Set("GameRuleEnd", easing::CIRCIN, ge->screen2DWidth / 2.f, -756.f, this->gameFps);

		//Finish移動用
		easing::Set("FinishStart", easing::CIRCOUT, ge->screen2DWidth + 438.f * 2.f, ge->screen2DWidth / 2.f, this->gameFps, "FinishEnd");
		easing::Set("FinishEnd", easing::CIRCIN, ge->screen2DWidth / 2.f, -438.f * 2.f, this->gameFps);

		//☆BGM
		bgm::LoadFile("SaboriGameBGM", "./data/sound/bgm/サボる_Short60_ゆったりDIY_01.mp3");
		bgm::VolumeControl("SaboriGameBGM", 90);

		//☆SE
		//Fight描画時に鳴らす
		se::LoadFile("StartSE", "./data/sound/se/Common/試合開始のゴング.wav");
		se::SetVolume("StartSE", 100);

		//ゲーム終了時に鳴らす
		se::LoadFile("FinishSE", "./data/sound/se/Common/試合終了のゴング.wav");
		se::SetVolume("FinishSE", 100);

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
		ge->KillAll_G("サボりミニゲーム");

		if (!ge->QuitFlag() && nextTaskCreate) {
			//BGM終了
			bgm::Stop("SaboriGameBGM");

			//★引き継ぎタスクの生成
			Game::Object::CreateTask(5);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//ゲームの状態遷移
		this->GameStateTransition();
		//状態に対応する行動処理
		this->Work();
		//イージングを動かす
		easing::UpDate();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//状態に対応する描画処理
		this->Render();
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
			if (this->countToChangeGameState >= 60) { nowState = GameState::Game; } //ゲーム中へ
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
		}
	}
	//-------------------------------------------------------------------
	//状態毎の処理
	void Object::Work()
	{
		switch (this->gameState)
		{
		case GameState::BeforeGameStart:	//ゲーム開始前
			//☆ゲームが始まった瞬間に行う処理
			if (this->gameStart == true)
			{
				//☆イージング開始
				easing::Start("GameRuleStart");

				this->gameStart = false;
			}

			//☆イージングで座標移動
			//Readyを動かす
			this->gameRuleImagePos.x = easing::GetPos("GameRuleStart");
			if (easing::GetState("GameRuleStart") == easing::EQ_STATE::EQ_END) //イージング「GameRuleStart」が終わったら
			{
				this->gameRuleImagePos.x = easing::GetPos("GameRuleEnd");
			}

			//☆Fight描画用処理
			if (easing::GetState("GameRuleEnd") == easing::EQ_STATE::EQ_END) //イージング「GameRuleEnd」が終わったら
			{
				this->countToFightDraw++;
			}

			//☆Fightの描画時に行う処理
			if (countToFightDraw == this->gameFps)
			{
				//ゲーム開始のSEを鳴らす
				se::Play("StartSE");
			}

			//☆状態を遷移するための処理
			if (countToFightDraw >= this->gameFps) //Fightの描画と同時に
			{
				this->countToChangeGameState++;
			}

			break;

		case GameState::Game:				//ゲーム中
			//☆ゲーム本編が始まった瞬間に行う処理
			if (this->isInGame == false)
			{
				//☆ゲームを開始する
				this->isInGame = true;

				//☆BGMスタート
				bgm::Play("SaboriGameBGM");
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
			//☆ゲームの状態がEndの時、一度だけ行う処理
			if (this->isInGame == true)
			{
				//ゲームを終了させる
				this->isInGame = false;

				//全てのSEを消す
				se::AllStop();

				//終了したらSEを鳴らす
				se::Play("FinishSE");

				//☆順位を決め、ge->scoreに得点を送る
				//順位を決める
				this->Ranking();
				//ge->scoreに得点を送る
				this->SendScore();

				//☆イージング開始
				easing::Start("FinishStart");
			}

			//☆イージングで座標移動
			//Finishを動かす
			this->finishImagePos.x = easing::GetPos("FinishStart");
			if (easing::GetState("FinishStart") == easing::EQ_STATE::EQ_END) //イージング「FinishStart」が終わったら
			{
				this->finishImagePos.x = easing::GetPos("FinishEnd");
			}

			//☆次のタスクに行くまでのカウント
			if (easing::GetState("FinishEnd") == easing::EQ_STATE::EQ_END)  //イージング「FinishEnd」が終わったら
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
	//状態毎の描画
	void Object::Render()
	{
		ML::Box2D src = {};
		ML::Box2D draw = {};

		switch (this->gameState)
		{
		case GameState::BeforeGameStart:	//ゲーム開始前
			//☆「Ready」描画
			//描画情報設定
			src = ML::Box2D(0, 0, 756, 87);
			draw = ML::Box2D(-src.w, -src.h, src.w * 2, src.h * 2);
			draw.Offset(this->gameRuleImagePos);

			this->res->gameRuleImage->Draw(draw, src);

			//☆「Fight」描画
			if (this->countToFightDraw >= this->gameFps)
			{
				//描画情報設定
				src = ML::Box2D(0, 0, 219, 95);
				draw = ML::Box2D(-110 * 3, -48 * 3, src.w * 3, src.h * 3);
				draw.Offset(this->fightImagePos);

				this->res->fightImage->Draw(draw, src);
			}
			break;

		case GameState::End:				//ゲーム終了
			//☆「Finish」描画
			src = ML::Box2D(0, 0, 438, 105);
			draw = ML::Box2D(-src.w, -48, src.w * 2, src.h * 2);
			draw.Offset(this->finishImagePos);

			this->res->finishImage->Draw(draw, src);
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

			case 2:	//3点
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

			case 3:	//2点
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

			case 4:	//1点
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
	//-------------------------------------------------------------------
	//ゲームを遊ぶプレイヤーの人数の情報を渡す
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
		//サボりゲーム関係
		gameState(GameState::BeforeGameStart), gameStart(true), countToNextTask(0), gameFps(60), countToChangeGameState(0), timeLimit(30.f), isInGame(false),
		//プレイヤー関係
		playerFirstPos{
			{ ge->screen2DWidth / 8.f, ge->screen2DHeight - 230.f },
			{ ge->screen2DWidth * 3.f / 8.f, ge->screen2DHeight - 230.f },
			{ ge->screen2DWidth * 5.f / 8.f, ge->screen2DHeight - 230.f },
			{ ge->screen2DWidth * 7.f / 8.f, ge->screen2DHeight - 230.f } 
		},
		controllers{ ge->in1, ge->in2, ge->in3, ge->in4 }, playersNum{ PlayerNum::Player1, PlayerNum::Player2, PlayerNum::Player3, PlayerNum::Player4 }, playersInfo(),
		playerCount(4),
		//上司関係
		joushiFirstPos(ML::Vec2(ge->screen2DWidth / 2.f, ge->screen2DHeight / 3.f)),
		//文字描画関係
		gameRuleImagePos(ML::Vec2(0.f, ge->screen2DHeight / 2.f)), fightImagePos(ML::Vec2(ge->screen2DWidth / 2.f, ge->screen2DHeight / 2.f)),
		finishImagePos(ML::Vec2(0.f, ge->screen2DHeight / 2.f)), countToFightDraw(0)
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