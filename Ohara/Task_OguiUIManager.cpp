//-------------------------------------------------------------------
//大食いミニゲームのUI管理
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiUIManager.h"
#include  "Task_OguiGame.h"
#include  "Task_OguiPlayer.h"

namespace  OguiUIManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		playerNumberImage = DG::Image::Create("./data/image/PlayerNumber.png");
		eatFoodCountImage = DG::Image::Create("./data/image/TextImage/OguiFont.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		playerNumberImage.reset();
		eatFoodCountImage.reset();
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
		this->render2D_Priority[1] = 0.6f;

		//ミニゲーム統括タスクからデータを取得する
		auto game = ge->GetTask<OguiGame::Object>(OguiGame::defGroupName, OguiGame::defName);
		if (game)
		{
			playerCount = game->GetPlayerCount();
		}

		//食べた料理の数の描画位置を決める
		DecideEatFoodCountDrawPos(playerCount);

		//プレイヤー番号の描画位置を決める
		DecidePlayerNumbersDrawInfo_pos(playerCount);

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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//☆食べた料理数の描画
		//プレイヤー全てを抽出する
		auto players = ge->GetTasks<OguiPlayer::Object>("プレイヤー");
		//プレイヤーの数だけループを回す
		int loopCount = 0; //ループした回数のカウント
		if (players)
		{
			for (auto p = players->begin(); p != players->end(); ++p)
			{
				//プレイヤー判別用番号描画
				DrawPlayerNumber_eatFoodCount(loopCount);

				//食べた料理の数描画
				DrawTotalSaboriTime((*p)->eatFoodCount, loopCount);

				//ループ回数のカウント
				++loopCount;
			}
		}

		//☆プレイヤー番号の描画
		this->DrawPlayerNumber();
	}

	//-------------------------------------------------------------------
	//★自作関数★
	//-------------------------------------------------------------------
	//プレイヤー番号の描画
	void Object::DrawPlayerNumber()
	{
		//for (int i = 0; i < size(this->playerNumbersDrawInfo); ++i) // CPU実装時はこっちを使う
		for (int i = 0; i < playerCount; ++i)
		{
			ML::Box2D playerNumberDraw = this->playerNumbersDrawInfo[i].draw;
			playerNumberDraw.Offset(this->playerNumbersDrawInfo[i].pos);
			ML::Box2D playerNumberSrc = this->playerNumbersDrawInfo[i].src;

			//描画
			this->res->playerNumberImage->Draw(playerNumberDraw, playerNumberSrc);
		}
	}

	//-------------------------------------------------------------------
	//食べた料理の数の判別用プレイヤー番号の描画
	void Object::DrawPlayerNumber_eatFoodCount(const int loopCount)
	{
		ML::Box2D playerNumberDraw = playerNumbersDrawInfo_eatFoodCount[loopCount].draw;
		playerNumberDraw.Offset(playerNumbersDrawInfo_eatFoodCount[loopCount].pos);
		ML::Box2D playerNumberSrc = playerNumbersDrawInfo_eatFoodCount[loopCount].src;

		//描画
		this->res->playerNumberImage->Draw(playerNumberDraw, playerNumberSrc);
	}

	//-------------------------------------------------------------------
	//食べた料理の数描画
	void Object::DrawTotalSaboriTime(const int eatFoodCount, const int loopCount)
	{
		//合計サボり時間の小数点第一位までを分解して格納する
		sprintf(eatFoodCountText, "%d", eatFoodCount);

		//切り取り位置、表示位置を決めて描画する
		for (int i = 0; i < static_cast<int>(size(eatFoodCountText)) - 1; ++i)
		{
			//srcのx,yの値設定
			int src_x = (eatFoodCountText[i] - '0') * 32;	// ML::Box2D srcのxの値


			//プレイヤーごとのスコア表示がわかる位置に描画されるようにする
			int drawingPositionCorrectionEachPlayer_x = static_cast<int>(playerNumbersDrawInfo_eatFoodCount[loopCount].pos.x) + 100;	// プレイヤーごとの描画位置補正 75は食べた料理の数の判別用プレイヤー番号の描画情報の-50を消した上で+50するための値
			
			ML::Box2D src(src_x, 0, 32, 32);
			ML::Box2D draw(-src.w, -src.h, src.w * 2, src.h * 2);
			//x座標をプレイヤー番号描画位置の後ろ側、y座標をプレイヤー番号描画位置と同じ位置に描画されるようにする
			draw.Offset(((src.w * 2) * i) + drawingPositionCorrectionEachPlayer_x, static_cast<int>(100 + (53 / 1.5f)));

			this->res->eatFoodCountImage->Draw(draw, src);
		}
	}

	//-------------------------------------------------------------------
	//プレイヤー番号の位置を遊ぶプレイヤーの人数に応じて決める
	void Object::DecidePlayerNumbersDrawInfo_pos(const int playerCount)
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

		//playerNumbersDrawInfo.posを設定
		for (int i = 0; i < playerCount; ++i)
		{
			playerNumbersDrawInfo[i].pos = ML::Vec2(ge->screen2DWidth * (sourceNumeratorValue + (valueIncreasePerLoop * i)) / 8.f, ge->screen2DHeight / 2.f - 200.f);
		}
	}

	//-------------------------------------------------------------------
	//食べた料理の数の描画位置を決める
	void Object::DecideEatFoodCountDrawPos(const int playerCount)
	{
		//計算に使う変数宣言
		int sourceNumeratorValue = 0;	// 元になる分子の値
		int valueIncreasePerLoop = 0;	// ループごとに増やす値

		//計算に使う値を決める
		switch (playerCount)
		{
		case 1:
			sourceNumeratorValue = 6;
			break;

		case 2:
			sourceNumeratorValue = 3;
			valueIncreasePerLoop = 6;
			break;

		case 3:
			sourceNumeratorValue = 2;
			valueIncreasePerLoop = 4;
			break;

		case 4:
			sourceNumeratorValue = 1;
			valueIncreasePerLoop = 3;
			break;
		}

		//playerNumbersDrawInfo_TotalSaboriTime.posを設定
		for (int i = 0; i < playerCount; ++i)
		{
			playerNumbersDrawInfo_eatFoodCount[i].pos = ML::Vec2(ge->screen2DWidth * (sourceNumeratorValue + (valueIncreasePerLoop * i)) / 12.f - 50.f, 100.f + (53.f / 1.5f));
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
		//プレイヤー識別番号関係
		playerNumbersDrawInfo{
			{ ML::Box2D(-78, -53, 155, 105), ML::Box2D(0, 0, 155, 105), ML::Vec2(0, 0) },	//1P
			{ ML::Box2D(-96, -53, 192, 105), ML::Box2D(155, 0, 192, 105), ML::Vec2(0, 0) },	//2P
			{ ML::Box2D(-88, -53, 175, 105), ML::Box2D(347, 0, 175, 105), ML::Vec2(0, 0) },	//3P
			{ ML::Box2D(-97, -53, 193, 105), ML::Box2D(522, 0, 193, 105), ML::Vec2(0, 0) }	//4P
		},
		playerNumbersDrawInfo_eatFoodCount{
			{ ML::Box2D(static_cast<int>(-78 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(155 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(0, 0, 155, 105), ML::Vec2(0, 0)},	// 1P
			{ ML::Box2D(static_cast<int>(-96 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(192 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(155, 0, 192, 105), ML::Vec2(0, 0)},	// 2P
			{ ML::Box2D(static_cast<int>(-88 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(175 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(347, 0, 175, 105), ML::Vec2(0, 0)},	// 3P
			{ ML::Box2D(static_cast<int>(-97 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(193 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(522, 0, 193, 105), ML::Vec2(0, 0)}	// 4P
		},
		playerCount(1),
		//食べた料理の数描画関係
		eatFoodCountText()
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