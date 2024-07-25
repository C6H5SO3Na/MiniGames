//-------------------------------------------------------------------
//大食いミニゲームの料理
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiFood.h"
#include  "Task_OguiFoodManager.h"
#include  "Task_OguiPlayer.h"
#include  "Task_OguiGame.h"

namespace  OguiFood
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->image = DG::Image::Create("./data/image/Ohara/testImage/testSquare.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->image.reset();
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
		if (game == nullptr)
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
		//☆描画
		DrawInformation drawImage = GetDrawImage();
		drawImage.draw.Offset(this->pos);

		this->res->image->Draw(drawImage.draw, drawImage.src);
	}
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
			//☆料理が無くなった時、30フレーム目にオブジェクトを消す申請を出す
			if (this->moveCnt >= 30) //モニターFPSにゲームが依存しないようにするために条件式を (int)(30 * GetFps() / (float)gameFps) に変更する
			{
				//料理が無くなった情報を他タスクに送る
				SendNotExistInformation();
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//アニメーション制御
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(200, 0, 100, 100) },	//残りが半分より多い
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(100, 0, 100, 100) },	//残り半分以下
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(300, 0, 100, 100) },	//残り1/3以下
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(0, 0, 100, 100)},	//完食
		};

		DrawInformation rtv;

		if (hp <= 0)				//完食
		{
			rtv = imageTable[3];
		}
		else if (hp <= maxHP / 3)	//残り1/3以下
		{
			rtv = imageTable[2];
		}
		else if (hp <= maxHP / 2)	//残り半分以下
		{
			rtv = imageTable[1];
		}
		else						//残りが半分より多い
		{
			rtv = imageTable[0];
		}

		return rtv;
	}
	//-------------------------------------------------------------------
	////料理の減少(受け身の処理)
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
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			//この料理を食べたプレイヤーか判定
			if ((*p)->playerNum == this->playerNum)
			{
				//プレイヤーの食べた料理数のカウントを増やす
				(*p)->eatFoodCount++;
				//料理が無くなったのでfalseを送る
				(*p)->SetExistFood(false);
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
		hp(0)
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