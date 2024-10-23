//-------------------------------------------------------------------
//サボりミニゲームのUI管理
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriUIManager.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriGame.h"

namespace  SaboriUIManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		playerNumberImage = DG::Image::Create("./data/image/PlayerNumber.png");
		totalSaboriTimeImage = DG::Image::Create("./data/image/TextImage/TotalSaboriTimeFont.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		playerNumberImage.reset();
		totalSaboriTimeImage.reset();
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
		testFont = DG::Font::Create("ＭＳ ゴシック", 30, 50);

		//ミニゲーム統括タスクからデータを取得する
		auto game = ge->GetTask<SaboriGame::Object>(SaboriGame::defGroupName, SaboriGame::defName);
		if (game)
		{
			playerCount = game->GetPlayerCount();
		}
		
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
		//☆サボり合計時間の描画
		//プレイヤー全てを抽出する
		auto players = ge->GetTasks<SaboriPlayer::Object>("プレイヤー");
		//プレイヤーの数だけループを回す
		int loopCount = 0; //ループした回数のカウント
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			//合計時間用プレイヤー番号描画
			DrawPlayerNumber_TotalSaboriTime(loopCount);

			//合計サボり時間描画
			DrawTotalSaboriTime((*p)->totalSaboriTime, loopCount);

			//ループ回数のカウント
			++loopCount;
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
	//サボり合計時間の判別用プレイヤー番号の描画
	void Object::DrawPlayerNumber_TotalSaboriTime(const int loopCount)
	{
		ML::Box2D playerNumberDraw = playerNumbersDrawInfo_TotalSaboriTime[loopCount].draw;
		playerNumberDraw.Offset(playerNumbersDrawInfo_TotalSaboriTime[loopCount].pos);
		ML::Box2D playerNumberSrc = playerNumbersDrawInfo_TotalSaboriTime[loopCount].src;

		//描画
		this->res->playerNumberImage->Draw(playerNumberDraw, playerNumberSrc);
	}

	//-------------------------------------------------------------------
	//合計サボり時間の描画
	void Object::DrawTotalSaboriTime(const float totalSaboriTime, const int loopCount)
	{
		//小数点を描画したかを判定する変数をfalseに戻す
		bool shouldDrewDecimalPoint = false;

		//合計サボり時間の小数点第一位までを分解して格納する
		sprintf(totalSaboriTimeText, "%.1f", totalSaboriTime); 

		//切り取り位置、表示位置を決めて描画する
		for (int i = 0; i < static_cast<int>(size(totalSaboriTimeText)) - 1; ++i)
		{
			//srcのx,yの値設定
			int src_x = (totalSaboriTimeText[i] - '0') * 32;	// ML::Box2D srcのxの値
			int src_y = 0;										// ML::Box2D srcのyの値
			//もしi番目の配列の中身が「.」だったらyの値を変更する
			if (totalSaboriTimeText[i] == '.')
			{
				src_x = 0;
				src_y = 32;
			}

			//小数点の描画位置設定
			int offValue_x = 0;	// 「.」の描画でずれるx座標の値

			//「.」を描画した後の描画位置をずらす値設定
			if (shouldDrewDecimalPoint)
			{
				offValue_x = -32; // 32はsrcのw値
			}

			//もしi番目の配列の中身が「.」の時、描画位置をずらす値を設定
			if (totalSaboriTimeText[i] == '.')
			{
				offValue_x = -(32 / 2); // 32はsrcのw値
				shouldDrewDecimalPoint = true;
			}

			//プレイヤーごとのスコア表示がわかる位置に描画されるようにする
			int drawingPositionCorrectionEachPlayer_x = playerNumbersDrawInfo_TotalSaboriTime[loopCount].pos.x + 75;	// プレイヤーごとの描画位置補正 50はサボり合計時間の判別用プレイヤー番号の描画情報の-50を消すための値

			ML::Box2D src(src_x, src_y, 32, 32);
			ML::Box2D draw((src.w * 2) * i + offValue_x + drawingPositionCorrectionEachPlayer_x, 100, src.w * 2, src.h * 2); // x座標はプレイヤー番号描画位置の後ろ側、y座標はプレイヤー番号描画位置と同じ位置に描画されるようにする

			this->res->totalSaboriTimeImage->Draw(draw, src);
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
		//プレイヤー識別番号関係---------------------------------------------------------------------------------------
		playerNumbersDrawInfo{
			{ ML::Box2D(-78, -53, 155, 105), ML::Box2D(0, 0, 155, 105), ML::Vec2(ge->screen2DWidth / 8.f, ge->screen2DHeight - 480.f) },			// 1P
			{ ML::Box2D(-96, -53, 192, 105), ML::Box2D(155, 0, 192, 105), ML::Vec2(ge->screen2DWidth * 3.f / 8.f, ge->screen2DHeight - 480.f) },	// 2P
			{ ML::Box2D(-88, -53, 175, 105), ML::Box2D(347, 0, 175, 105), ML::Vec2(ge->screen2DWidth * 5.f / 8.f, ge->screen2DHeight - 480.f) },	// 3P
			{ ML::Box2D(-97, -53, 193, 105), ML::Box2D(522, 0, 193, 105), ML::Vec2(ge->screen2DWidth * 7.f / 8.f, ge->screen2DHeight - 480.f) }		// 4P
		},
		playerNumbersDrawInfo_TotalSaboriTime{
			{ ML::Box2D(static_cast<int>(-78 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(155 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(0, 0, 155, 105), ML::Vec2(ge->screen2DWidth / 12.f - 50.f, 100.f + (53.f / 1.5f))},			// 1P
			{ ML::Box2D(static_cast<int>(-96 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(192 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(155, 0, 192, 105), ML::Vec2(ge->screen2DWidth * 4.f / 12.f - 50.f, 100.f + (53.f / 1.5f))},	// 2P
			{ ML::Box2D(static_cast<int>(-88 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(175 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(347, 0, 175, 105), ML::Vec2(ge->screen2DWidth * 7.f / 12.f - 50.f, 100.f + (53.f / 1.5f))},	// 3P
			{ ML::Box2D(static_cast<int>(-97 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(193 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(522, 0, 193, 105), ML::Vec2(ge->screen2DWidth * 10.f / 12.f - 50.f, 100.f + (53.f / 1.5f))}	// 4P
		},
		playerCount(1),
		//サボり合計時間描画関係---------------------------------------------------------------------------------------
		totalSaboriTimeText()
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