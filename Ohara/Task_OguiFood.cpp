//-------------------------------------------------------------------
//大食いミニゲームの料理
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiFood.h"
#include  "Task_OguiFoodManager.h"
#include  "Task_OguiPlayer.h"
#include  "Task_OguiGame.h"

#include  "../randomLib.h"

namespace  OguiFood
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		image = DG::Image::Create("./data/image/otsan_ramen.png");
		foodHPImage = DG::Image::Create("./data/image/TextImage/OguiFont.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		image.reset();
		foodHPImage.reset();
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
		this->state = State::FExist;
		
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
		if (game == nullptr) // nullチェック
		{
			return;
		}
		
		//ミニゲーム中の処理
		if (game->isInGame == true)
		{
			this->moveCnt++;

			//状態判断
			this->Think();
			//状態に対応する行動処理
			this->Move();
		}
	}

	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//☆料理描画
		DrawInformation drawImage = GetDrawImage();
		drawImage.draw.Offset(this->pos);

		this->res->image->Draw(drawImage.draw, drawImage.src);

		//☆残り料理残量描画
		drawRemainingCuisineRemainingAmount();
	}

	//-------------------------------------------------------------------
	//自分で作った関数の処理
	//-------------------------------------------------------------------
	//現在の料理の状態制御
	void Object::Think()
	{
		State nowState = this->state; //とりあえず現在の状態を代入

		//モーションの切り替え
		switch (nowState)
		{
		case State::FExist:
			if (this->hp <= 0) { nowState = State::FNotExist; }
			break;
		}

		//状態更新
		this->UpdateState(nowState);
	}
	//-------------------------------------------------------------------
	//状態毎の行動処理
	void Object::Move()
	{
		switch (this->state)
		{
		case State::FNotExist:
			//料理が無くなった情報を他タスクに送る
			SendNotExistInformation();
			this->Kill();

			//もしバグが出たらこの条件式の中に上の処理を入れる
			//☆料理が無くなった時、30フレーム目にオブジェクトを消す申請を出す
			//if (this->moveCnt >= 30) //モニターFPSにゲームが依存しないようにするために条件式を (int)(30 * GetFps() / (float)gameFps) に変更する
			//{
			//	
			//}
		}
	}
	//-------------------------------------------------------------------
	//アニメーション制御
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(-367 / 3, -233 / 3, 733 / 3, 466 / 3), ML::Box2D(0, 0, 734, 466) }
		};

		DrawInformation rtv = imageTable[0];

		return rtv;
	}
	//-------------------------------------------------------------------
	//料理の減少(受け身の処理)
	void Object::ReduceHP(int damage)
	{
		hp -= damage;
	}
	//-------------------------------------------------------------------
	//料理が無くなった情報を料理管理タスクに送る
	void Object::SendNotExistInformation()
	{
		//☆料理管理タスクに情報を送る
		auto foodManager = ge->GetTask<OguiFoodManager::Object>("管理");
		foodManager->NotExistFood(this->playerNum);

		//☆プレイヤーに情報を送る
		//全てのプレイヤーを取得
		auto players = ge->GetTasks<OguiPlayer::Object>("プレイヤー");
		if (players)
		{
			for (auto p = players->begin(); p != players->end(); ++p)
			{
				//この料理を食べたプレイヤーか判定
				if ((*p)->playerNum == this->playerNum)
				{
					//プレイヤーの食べた料理数のカウントを増やす
					(*p)->eatFoodCount++;
					//食べた料理の数が99を超え無いようにする
					if ((*p)->eatFoodCount > 99)
					{
						(*p)->eatFoodCount = 99;
					}

					//料理が無くなったのでfalseを送る
					(*p)->SetExistFood(false);
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//残り料理残量描画
	void Object::drawRemainingCuisineRemainingAmount()
	{
		//描画
		if (this->hp > 0)
		{
			//hpを分解して格納する
			sprintf(foodHPText, "%d", hp);

			//切り取り位置、表示位置を決めて描画する
			for (int i = 0; i < static_cast<int>(size(foodHPText)) - 1; ++i)
			{
				//srcのxの値設定
				int src_x = (foodHPText[i] - '0') * 32;	// ML::Box2D srcのxの値

				//描画位置と切り取り位置の変数宣言
				ML::Box2D draw(0, 0, 0, 0);			// 描画位置
				ML::Box2D src(src_x, 0, 32, 32);	// 切り取り位置

				//hpが二桁か一桁かで描画位置を変更する
				if (hp >= 10)
				{
					//描画位置設定
					draw = ML::Box2D(-src.w / 2, -src.h / 2, src.w, src.h);
					draw.Offset(pos.x + (-(src.w / 2) + (src.w * i)), pos.y);
				}
				else
				{
					//描画位置設定
					draw = ML::Box2D(-src.w / 2, -src.h / 2, src.w, src.h);
					draw.Offset(pos.x, pos.y);
				}

				//描画
				this->res->foodHPImage->Draw(draw, src);
			}

			/*testFont->Draw(ML::Box2D(static_cast<int>(this->pos.x) - 25, static_cast<int>(this->pos.y) - 25, ge->screen2DWidth, ge->screen2DHeight),
				to_string(this->hp), ML::Color(1, 0, 0, 0)
			);*/
		}
		////hpが0未満になったら
		//else
		//{
		//	//描画位置と切り取り位置の設定
		//	ML::Box2D src(0, 0, 32, 32);
		//	ML::Box2D draw(-src.w / 2, -src.h / 2, src.w, src.h);
		//	draw.Offset(pos.x, pos.y);

		//	//描画
		//	this->res->foodHPImage->Draw(draw, src);

		//	/*testFont->Draw(ML::Box2D(static_cast<int>(this->pos.x) - 25, static_cast<int>(this->pos.y) - 25, ge->screen2DWidth, ge->screen2DHeight),
		//		"0", ML::Color(1, 0, 0, 0)
		//	);*/
		//}
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
		hp(0), foodHPText()
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