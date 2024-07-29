//-------------------------------------------------------------------
//サボりミニゲームの上司
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriJoushi.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriGame.h"

#include  "../randomLib.h"

namespace  SaboriJoushi
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
		this->render2D_Priority[1] = 0.6f;
		this->state = State::JWork;
		this->changeStateTime = GetRandom(2.f, 4.f); //仕事中状態からサボり確認の予備動作状態に変更するまでの時間の設定
		
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
		auto game = ge->GetTask<SaboriGame::Object>(SaboriGame::defGroupName, SaboriGame::defName);

		//さぼりミニゲームタスクが取得できているか確認
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
	//上司の状態制御
	void Object::Think()
	{
		State nowState = this->state; //とりあえず現在の状態を代入

		//モーションの切り替え
		switch (nowState)
		{
		case State::JWork:						//仕事中状態
			if (this->moveCnt >= (int)(this->changeStateTime * this->gameFps)) { nowState = State::JBeforeCheckingForSabori; } //サボり確認の予備動作状態へ モニターFPSにゲームが依存しないようにするために条件式に * GetFps() / (float)gameFps を追加する
			break;

		case State::JBeforeCheckingForSabori:	//サボり確認の予備動作状態
			if (this->moveCnt >= (int)(this->changeStateTime * this->gameFps)) //モニターFPSにゲームが依存しないようにするために条件式に * GetFps() / (float)gameFps を追加する
			{
				//☆ランダムで遷移する状態を変更する
				//遷移する状態を決定
				this->changeStateKinds = GetRandom(0, 3);

				//1/4で仕事中状態、3/4でサボり確認状態へ
				if (this->changeStateKinds == 0) { nowState = State::JWork; } //仕事中状態へ
				else{ nowState = State::JCheckForSabori; } //サボり確認状態へ
			}
			break;

		case State::JCheckForSabori:			//サボり確認状態
			if (this->moveCnt >= (int)(this->changeStateTime * this->gameFps)) { nowState = State::JWork; } //仕事中状態へ モニターFPSにゲームが依存しないようにするために条件式に * GetFps() / (float)gameFps を追加する
			if (this->hasFoundPlayerSabori) { nowState = State::JFoundSabori; } //サボり発見状態へ
			break;

		case State::JFoundSabori:				//サボり発見状態
			if (this->moveCnt >= 1 * this->gameFps) { nowState = State::JWork; } //仕事中状態へ モニターFPSにゲームが依存しないようにするために条件式に * GetFps() / (float)gameFps を追加する
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
		case State::JWork:						//仕事中状態
			//☆仕事中状態からサボり確認の予備動作状態に変更するまでの時間の設定
			if (this->moveCnt == 0) //状態が切り替わった時のみ処理する
			{
				//時間設定
				this->changeStateTime = GetRandom(2.f, 4.f); //2〜4秒
			}
			break;

		case State::JBeforeCheckingForSabori:	//サボり確認の予備動作状態
			//☆サボり確認の予備動作状態からサボり確認状態または仕事中状態に変更するまでの時間の設定
			if (this->moveCnt == 0) //状態が切り替わった時のみ処理する
			{
				//時間設定
				this->changeStateTime = GetRandom(1.f, 2.f); //1〜2秒
			}
			break;

		case State::JCheckForSabori:			//サボり確認状態
			//☆サボり確認状態から仕事中状態に変更するまでの時間の設定
			if (this->moveCnt == 0) //状態が切り替わった時のみ処理する
			{
				//時間設定
				this->changeStateTime = GetRandom(1.f, 2.f); //1〜3秒
			}

			//☆プレイヤーがさぼり状態かどうか判定する
			CheckPlayerSabori();
			break;

		case State::JFoundSabori:				//サボり発見状態
			//☆状態変化時に一度だけ音を鳴らす
			if (this->hasFoundPlayerSabori)
			{
				//ここに音を鳴らす処理を書く
				this->hasFoundPlayerSabori = false;
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//アニメーション制御
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(200, 0, 100, 100) },	//仕事状態
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(100, 0, 100, 100) },	//サボり確認の予備動作状態
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(300, 0, 100, 100) },	//サボり確認状態
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(0, 0, 100, 100)},	//サボり発見状態
		};

		DrawInformation rtv;
		switch (this->state)
		{
		case State::JWork:						//仕事中状態
			rtv = imageTable[0];
			break;

		case State::JBeforeCheckingForSabori:	//サボり確認の予備動作状態
			rtv = imageTable[1];
			break;

		case State::JCheckForSabori:			//サボり確認状態
			rtv = imageTable[2];
			break;

		case State::JFoundSabori:				//サボり発見状態
			rtv = imageTable[3];
			break;
		}

		return rtv;
	}
	//-------------------------------------------------------------------
	//プレイヤーがさぼり状態かどうか判定する
	void Object::CheckPlayerSabori()
	{
		//プレイヤー全てを抽出する
		auto players = ge->GetTasks<SaboriPlayer::Object>("プレイヤー");
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			if ((*p)->state == State::PSabori)
			{
				//上司がさぼりを発見
				this->hasFoundPlayerSabori = true;
				//プレイヤーがさぼりに気づかれる
				(*p)->noticedToSabori = true;
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
		changeStateTime(0),
		changeStateKinds(0),
		hasFoundPlayerSabori(false)
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