//-------------------------------------------------------------------
//大食いミニゲームのプレイヤー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiPlayer.h"
#include  "Task_OguiFood.h"
#include  "Task_OguiGame.h"

namespace  OguiPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->image = DG::Image::Create("./data/image/Ohara/testImage/testCircle.png");
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
		this->render2D_Priority[1] = 0.6f;
		this->state = State::PWait;
		
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
		DrawInformation drawImage = this->GetDrawImage();
		drawImage.draw.Offset(this->pos);

		this->res->image->Draw(drawImage.draw, drawImage.src);
	}
	//-------------------------------------------------------------------
	//現在のプレイヤーの状態制御
	void Object::Think()
	{
		auto input = this->controller->GetState();
		State nowState = this->state; //とりあえず現在の状態を代入

		//モーションの切り替え
		switch (nowState)
		{
		case State::PWait:	//待機状態
			if (input.B1.down && this->existFood == true) { nowState = State::PEat; }
			break;

		case State::PEat:	//食事中状態
			if (this->moveCnt == 30 || this->existFood == false) { nowState = State::PWait; }
			break;
		}

		//状態更新
		this->UpdateState(nowState);
	}
	//-------------------------------------------------------------------
	//状態毎の行動処理
	void Object::Move()
	{
		auto input = this->controller->GetState();

		switch (this->state)
		{
		case State::PEat:	//食事中状態
			if (input.B1.down)
			{
				//☆料理の残量を減らす
				//全ての料理を取得
				auto foods = ge->GetTasks<OguiFood::Object>("ギミック");
				for (auto f = foods->begin(); f != foods->end(); ++f)
				{
					if (this->playerNum == (*f)->playerNum)
					{
						//料理の量を減らす
						(*f)->ReduceHP(this->attack);
						//料理の量を減らしたら食事中状態を継続するようにする
						this->moveCnt = 0;
						return;
					}
				}
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//アニメーション制御
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(200, 0, 100, 100) },	//待機状態
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(100, 0, 100, 100) },	//食事中状態
		};

		DrawInformation rtv;
		switch (this->state)
		{
		case State::PWait:		//待機状態
			rtv = imageTable[0];
			break;

		case State::PEat:		//食事中状態
			rtv = imageTable[1];
			break;
		}

		return rtv;
	}
	//-------------------------------------------------------------------
	//料理の存在しているかの情報を取得
	void Object::SetExistFood(bool foodExistenceInformation)
	{
		this->existFood = foodExistenceInformation;
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
		attack(1),
		eatFoodCount(0),
		existFood(false)
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