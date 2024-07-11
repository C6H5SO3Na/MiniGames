//-------------------------------------------------------------------
//サボりミニゲームのプレイヤー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriGame.h"

#include  "../fpscounter.h"

namespace  SaboriPlayer
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
		this->state = State::PWork;
		
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
		auto game = ge->GetTask<SaboriGame::Object>("本編");

		//さぼりミニゲームタスクが取得できているか確認
		if (game == nullptr)
		{
			return;
		}

		//ミニゲーム中の処理
		if (game->isGameOver == false)
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
		State nowState = this->state;	//とりあえず現在の状態を代入

		//モーションの切り替え
		switch (nowState)
		{
		case State::PWork:		//仕事中状態
			if (input.B1.on) { nowState = State::PSabori; } //サボり状態へ
			break;

		case State::PSabori:		//サボり状態
			if (input.B1.up) { nowState = State::PWork; } //仕事中状態へ
			if (noticedToSabori) { nowState = State::PNoticed; } //サボりばれ状態へ
			break;

		case State::PNoticed:	//サボりばれ状態
			if (this->moveCnt >= this->gameFps * 6) { nowState = State::PWork; } //仕事中状態へ モニターFPSにゲームが依存しないようにするために条件式に * GetFps() / (float)gameFps する
			break;
		}

		//状態更新
		this->UpdateState(nowState);
	}
	//-------------------------------------------------------------------
	//状態毎の行動処理
	void Object::Move()
	{
		//fpscounterをインスタンス化する

		switch (this->state)
		{
		case State::PSabori:	//サボり状態
			this->totalSaboriTime += 1.f / gameFps; // / gameFps を / GetFps() をに変更してモニターFPSにゲームが依存しないようにする
			break;

		case State::PNoticed:
			this->noticedToSabori = false;
			break;
		}
	}
	//-------------------------------------------------------------------
	//アニメーション制御
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(100, 0, 100, 100) },	//仕事状態
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(200, 0, 100, 100) },	//サボり状態
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(0, 0, 100, 100) },	//サボりばれ状態
		};

		DrawInformation rtv;
		switch (this->state)
		{
		case State::PWork:		//仕事中状態
			rtv = imageTable[0];
			break;

		case State::PSabori:		//サボり状態
			rtv = imageTable[1];
			break;

		case State::PNoticed:	//サボりばれ状態
			rtv = imageTable[2];
			break;
		}

		return rtv;
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
		totalSaboriTime(0.f),
		noticedToSabori(false)
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