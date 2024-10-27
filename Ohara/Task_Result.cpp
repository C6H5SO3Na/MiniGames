//-------------------------------------------------------------------
//最終リザルト画面タスク
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_Result.h"
#include  "Task_ResultUIManager.h"
#include  "Task_ResultBG.h"

#include  "../Task_Title.h"
#include  "../sound.h"

namespace  Result
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
		this->res = Resource::Create();

		//★データ初期化

		for (int i = 0; i < playerNum; ++i) {
			playersInfo.push_back({ i + 1, 0, 0 });
		}

		//☆スコア計算
		Ranking();

		//★タスクの生成
		//UI管理
		ResultUIManager::Object::Create(true);

		//背景
		//2つの画像で横スクロールを作る
		for (int i = 0; i < 2; ++i)
		{
			auto BG = ResultBG::Object::Create(true);
			if (BG)
			{
				BG->pos.x = static_cast<float>(i * ge->screen2DWidth);
			}
		}

		//☆BGM
		bgm::LoadFile("ResultBGM", "./data/sound/bgm/エンディング_tanoshiibouken.mp3");
		bgm::VolumeControl("ResultBGM", 95);
		bgm::Play("ResultBGM");

		//☆SE
		se::LoadFile("PushButtonSE", "./data/sound/se/Common/決定ボタンを押す40.wav");
		se::SetVolume("PushButtonSE", 100);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("リザルト画面");
		ge->KillAll_G("管理");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//☆BGMの終了
			bgm::Stop("ResultBGM");

			//★引き継ぎタスクの生成
			auto next = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//☆次の状態に変更できるようになったら、次の状態までのカウントを始める
		if (nextStateGoIs == true)
		{
			this->countUpToStateChange++;
		}

		//状態判断
		ResultStateTransition();
		//状態に対応する処理
		ProcessEachState();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//リザルトの状態遷移
	void Object::ResultStateTransition()
	{
		ResultState nowState = this->resultState; //とりあえず現在の状態を代入

		//状態の切り替え
		switch (nowState)
		{
		case ResultState::Preface:				//前置き
			if (this->nextStateGoIs == true)
			{
				for (int i = 0; i < playerNum; ++i)
				{
					//結果発表へ
					if (ge->players[i]->GetState().B2.down)
					{
						nowState = ResultState::ResultAnnouncement; nextStateGoIs = false;
					}
				}
				if (this->countUpToStateChange >= (int)(0.5f * gameFps)) { nowState = ResultState::ResultAnnouncement; nextStateGoIs = false; } //結果発表へ
			}
			break;

		case ResultState::ResultAnnouncement:	//結果発表
			if (this->nextStateGoIs == true)
			{
				for (int i = 0; i < playerNum; ++i)
				{
					//結果発表終了へ
					if (ge->players[i]->GetState().B2.down)
					{
						//SEを鳴らす
						se::Play("PushButtonSE");

						nowState = ResultState::End; nextStateGoIs = false;
					}
				}
			}
			break;
		}

		//状態更新
		UpdateResultState(nowState);
	}
	//-------------------------------------------------------------------
	//リザルトの状態変更時処理
	void Object::UpdateResultState(ResultState nowState)
	{
		if (nowState != this->resultState)
		{
			this->resultState = nowState;
			this->countUpToStateChange = 0;

			//ResultUIManagerの方も更新を行う
			auto UImanager = ge->GetTask<ResultUIManager::Object>("管理");
			if (UImanager == nullptr)
			{
				return;
			}

			UImanager->DataChangesWhenUpdatingState();
		}
	}
	//-------------------------------------------------------------------
	//状態毎の処理
	void Object::ProcessEachState()
	{
		switch (this->resultState)
		{
		case ResultState::Preface:				//前置き
			//Task_resultUIManagerでイージングを入れたら、ボタンを押すとTask_resultUIManagerの、画面外に出ていくイージングが起動する処理を実装する
			break;

		case ResultState::End:					//結果発表終了
			//表示用プレイヤーを消す
			if (shouldKillTask == true)
			{
				ge->KillAll_G("画面表示用");
				shouldKillTask = false;
			}

			//タイトルに戻る
			if (this->nextStateGoIs == true)
			{
				//タイトル画面に遷移
				for (int i = 0; i < playerNum; ++i)
				{
					if (ge->players[i]->GetState().B2.down) {
						//SEを鳴らす
						se::Play("PushButtonSE");

						ge->StartCounter("test", 0);
					}
					if (ge->getCounterFlag("test") == ge->LIMIT) {
						Kill();
					}
				}
			}

			break;
		}
	}
	//-------------------------------------------------------------------
	//次の状態に変更できるようにする
	void Object::ChangeableToNextState()
	{
		this->nextStateGoIs = true;
	}
	//-------------------------------------------------------------------
	//順位決めの処理
	void Object::Ranking()
	{
		//MyPGからスコアを取得
		for (int i = 0; i < playersInfo.size(); ++i)
		{
			playersInfo[i].score = ge->GetScore(i);
		}

		//スコアが高い順にplayersInfoを並び替え
		sort(playersInfo.begin(), playersInfo.end(), [&](const PlayerInformation& scoreA, const PlayerInformation& scoreB) { return compare(scoreA, scoreB); });

		//順位を決める
		int currentRank = 1; //入れる順位
		for (int i = 0; i < playersInfo.size(); ++i)
		{
			//2回目以降のループで、スコアの値が前のスコアと同じでなければ入れる順位を+1する
			if (i > 0 && playersInfo[i].score != playersInfo[i - 1].score)
			{
				currentRank = i + 1;
			}

			//順位を決定
			playersInfo[i].rank = currentRank;
		}
	}
	//-------------------------------------------------------------------
	//scoreAとscoreBに入れた変数のscoreで比較し、scoreAが大きい時trueを返す
	bool Object::compare(const PlayerInformation& scoreA, const PlayerInformation& scoreB)
	{
		return scoreA.score > scoreB.score;
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
	Object::Object()
		:
		resultState(ResultState::Preface),
		nextStateGoIs(false),
		countUpToStateChange(0),
		gameFps(60),
		shouldKillTask(true),
		playerNum(ge->players.size())
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
	Resource::~Resource() { this->Finalize(); }
}