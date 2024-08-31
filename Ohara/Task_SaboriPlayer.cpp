//-------------------------------------------------------------------
//サボりミニゲームのプレイヤー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriGame.h"

#include  "../fpscounter.h"
#include  "../sound.h"

namespace  SaboriPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->image = DG::Image::Create("./data/image/game_otsan_working.png");
		this->buttonImage_A = DG::Image::Create("./data/image/button/Double/xbox_button_color_a.png");
		this->buttonImage_A_Outline = DG::Image::Create("./data/image/button/Double/xbox_button_color_a_outline.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->image.reset();
		this->buttonImage_A.reset();
		this->buttonImage_A_Outline.reset();
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
		DrawInformation drawImage = this->GetDrawImage();
		drawImage.draw.Offset(this->pos);
		
		this->res->image->Draw(drawImage.draw, drawImage.src);

		//☆Aボタン描画
		this->DrawButton();
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

		case State::PSabori:	//サボり状態
			if (input.B1.up) { nowState = State::PWork; } //仕事中状態へ
			if (noticedToSabori) { nowState = State::PNoticed; } //サボりばれ状態へ
			break;

		case State::PNoticed:	//サボりばれ状態
			if (this->moveCnt >= this->gameFps * 3) { nowState = State::PWork; } //仕事中状態へ モニターFPSにゲームが依存しないようにするために条件式に * GetFps() / (float)gameFps する
			break;
		}

		//状態更新
		this->UpdateState(nowState);
	}
	//-------------------------------------------------------------------
	//状態毎の行動処理
	void Object::Move()
	{
		//☆ゲーム本編開始時に一度だけ行う処理

		if (this->isPlayStartSE == false)
		{
			//SEの名前付け
			this->workSEName = "WorkSE" + to_string((int)this->playerNum);
			this->saboriSEName = "SaoriSE" + to_string((int)this->playerNum);

			//SE設定
			//仕事中状態
			se::LoadFile(workSEName, "./data/sound/se/SaboriGame/PC-Keyboard05-14(Far-Hard).wav");
			se::SetVolume(workSEName, 100);

			//サボり状態
			se::LoadFile(saboriSEName, "./data/sound/se/SaboriGame/maou_se_8bit08.wav");
			se::SetVolume(saboriSEName, 1);

			//SEを鳴らす
			se::PlayLoop(workSEName);

			//ボタンの描画を開始する
			this->buttonDrawPos = ML::Vec2(this->pos.x, this->pos.y - 400);
			this->isStartButtonDraw = true;

			this->isPlayStartSE = true;
		}

		switch (this->state)
		{
		case State::PWork:		//仕事中状態

			//☆状態変更時に1回だけ行う
			if (this->moveCnt == 0)
			{
				//SEを止める
				se::Stop(saboriSEName);

				//SEを鳴らす
				se::PlayLoop(workSEName);
			}

			break;

		case State::PSabori:	//サボり状態
			//状態変更時に1回だけ行う処理
			if (this->moveCnt == 0)
			{
				//SEを止める
				se::Stop(workSEName);

				//SEを鳴らす
				se::PlayLoop(saboriSEName);
			}
			
			this->totalSaboriTime += 1.f / gameFps; // / gameFps を / GetFps() をに変更してモニターFPSにゲームが依存しないようにする
			break;

		case State::PNoticed:
			//状態変更時に1回だけ行う処理
			if (this->moveCnt == 0)
			{
				//SEを止める
				se::Stop(saboriSEName);
			}
			this->noticedToSabori = false;
			break;
		}
	}
	//-------------------------------------------------------------------
	//アニメーション制御
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(static_cast<int>(-250 / 1.5f) , static_cast<int>(-230 / 1.5f), static_cast<int>(500 / 1.5f), static_cast<int>(460 / 1.5f)), ML::Box2D(500, 0, 500, 460)},	//仕事状態
			{ ML::Box2D(static_cast<int>(-250 / 1.5f) , static_cast<int>(-230 / 1.5f), static_cast<int>(500 / 1.5f), static_cast<int>(460 / 1.5f)), ML::Box2D(1000, 0, 500, 460) },	//サボり状態
			{ ML::Box2D(static_cast<int>(-250 / 1.5f) , static_cast<int>(-230 / 1.5f), static_cast<int>(500 / 1.5f), static_cast<int>(460 / 1.5f)), ML::Box2D(1500, 0, 500, 460) },	//サボりばれ状態
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
	//-------------------------------------------------------------------
	//ボタンの描画処理
	void Object::DrawButton()
	{
		if (this->isStartButtonDraw == true)
		{
			DrawInformation drawButtonImage = { ML::Box2D(-64, -64, 128, 128), ML::Box2D(0, 0, 128, 128) };
			drawButtonImage.draw.Offset(this->buttonDrawPos);

			switch (this->state)
			{
			case State::PWork:		//仕事中状態
				this->res->buttonImage_A_Outline->Draw(drawButtonImage.draw, drawButtonImage.src);
				break;

			case State::PSabori:	//サボり状態
				this->res->buttonImage_A->Draw(drawButtonImage.draw, drawButtonImage.src);
				break;

			case State::PNoticed:
				this->res->buttonImage_A_Outline->Draw(drawButtonImage.draw, drawButtonImage.src);
				break;
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
		totalSaboriTime(0.f),
		noticedToSabori(false),
		isPlayStartSE(false),
		saboriSEName(""),
		workSEName(""),
		isStartButtonDraw(false),
		buttonDrawPos(0, 0)
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