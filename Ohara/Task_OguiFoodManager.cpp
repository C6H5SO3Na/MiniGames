//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiFoodManager.h"
#include  "Task_OguiFood.h"
#include  "Task_OguiPlayer.h"
#include  "Task_OguiGame.h"

namespace  OguiFoodManager
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
		//ミニゲーム統括タスクからデータを取得する
		auto game = ge->GetTask<OguiGame::Object>(OguiGame::defGroupName, OguiGame::defName);
		if (game)
		{
			playerCount = game->GetPlayerCount();
		}
		
		for (int i = 0; i < playerCount; ++i)
		{
			hasExistFoods.push_back(false);
		}

		//プレイヤーの初期位置を決める
		DecideFoodFirstPos(playerCount);

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto game = ge->GetTask<OguiGame::Object>(OguiGame::defGroupName, OguiGame::defName);

		//大食いミニゲームタスクが取得できているか確認
		if (game == nullptr)
		{
			return;
		}

		//ミニゲーム中の処理
		if (game->isInGame == true)
		{
			//☆料理の生成
			//料理があるか確認
			//for(int i = 0; i < size(hasExistFoods); ++i) // CPU実装時はこっちを使う
			for (int i = 0; i < hasExistFoods.size(); ++i)
			{
				//料理が無かったら30フレーム待った後、料理が無いプレイヤーの場所に料理を配置
				if (this->hasExistFoods[i] == false)
				{
					this->createCount[i]++;
					if (this->createCount[i] >= 10) //モニターFPSにゲームが依存しないようにするために条件式に * GetFps() / (float)gameFps を追加する
					{
						this->createCount[i] = 0; //生成カウントをゼロクリアする
						CreateFood((PlayerNum)(i + 1)); //i+1でどのプレイヤーの料理を生成するか示す
						this->hasExistFoods[i] = true;
					}
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//料理の生成
	void Object::CreateFood(PlayerNum food_PlayerNum)
	{
		//☆料理の生成
		auto f = OguiFood::Object::Create(true);
		f->hp = f->maxHP;
		f->playerNum = food_PlayerNum;
		f->pos = this->foodPositions[(int)food_PlayerNum - 1]; //受け取ったプレイヤー識別番号-1でどのプレイヤーの位置に料理を出現させるか示す

		//☆料理が生成された情報を送る
		SendCreateFoodInformation(food_PlayerNum);
	}
	//-------------------------------------------------------------------
	//料理がなくなった情報を取得
	void Object::NotExistFood(PlayerNum food_PlayerNum)
	{
		this->hasExistFoods[(int)food_PlayerNum - 1] = false; //受け取ったプレイヤー識別番号-1でどのプレイヤーに料理が無いかを示す
	}
	//-------------------------------------------------------------------
	//料理を生成した情報を他タスクに送る
	void Object::SendCreateFoodInformation(PlayerNum food_PlayerNum)
	{
		//☆プレイヤーに情報を送る
		//プレイヤーを全て取得
		auto players = ge->GetTasks<OguiPlayer::Object>("プレイヤー");
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			//料理が生成されたプレイヤーか判定
			if ((*p)->playerNum == food_PlayerNum)
			{
				//料理が生成されたのでtrueを送る
				(*p)->SetExistFood(true);
			}
		}
	}

	//-------------------------------------------------------------------
	//料理の生成位置を遊ぶプレイヤーの人数に応じて決める
	void Object::DecideFoodFirstPos(const int playerCount)
	{
		//計算に使う変数宣言
		int sourceNumeratorValue = 0;	// 元になる分子の値
		int valueIncreasePerLoop = 0;	// ループごとに増やす値

		//計算に使う値を決める
		switch (playerCount)
		{
		case 1:
			sourceNumeratorValue = 4;
			break;

		case 2:
			sourceNumeratorValue = 2;
			valueIncreasePerLoop = 4;
			break;

		case 3:
			sourceNumeratorValue = 1;
			valueIncreasePerLoop = 3;
			break;

		case 4:
			sourceNumeratorValue = 1;
			valueIncreasePerLoop = 2;
			break;
		}

		//playerFirstPosを設定
		for (int i = 0; i < playerCount; ++i)
		{
			foodPositions.push_back(ML::Vec2(ge->screen2DWidth * (sourceNumeratorValue + (valueIncreasePerLoop * i)) / 8.f, ge->screen2DHeight / 2.f + 200));
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
		//料理生成関係
		createCount{30, 30, 30, 30}, playerCount(4)
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