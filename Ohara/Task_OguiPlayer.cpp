//-------------------------------------------------------------------
//大食いミニゲームのプレイヤー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiPlayer.h"
#include  "Task_OguiFood.h"
#include  "Task_OguiGame.h"

#include  "../sound.h"

namespace  OguiPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->image = DG::Image::Create("./data/image/game_otsan_eat_new.png");
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
		this->state = State::PWait;	// 初期状態設定

		/*メモ*/
		/*プレイヤー操作かCPU操作かを決めるとき、TasK_OguiGameのgetter関数から情報を取得すると簡単にできる*/
		
		//★タスクの生成

		//☆SE
		se::LoadFile("eatSE", "./data/sound/se/OguiGame/Motion-Eating01-2.wav");
		se::SetVolume("eatSE", 50);

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
		if (game) // 大食いミニゲームタスクが取得できているか確認(nullチェック)
		{
			//ミニゲーム中の処理
			if (game->isInGame == true)
			{
				this->moveCnt++;
				this->animationCount++;

				//状態判断
				this->Think();
				//状態に対応する行動処理
				this->Move();
			}
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
		XI::VGamePad input;	// 入力情報を格納する
		if (controller) // nullチェック
		{
			input = this->controller->GetState(); // 入力情報の取得
		}
		State nowState = this->state; // とりあえず現在の状態を代入

		switch (playerType) // プレイヤかCPUかの判別
		{
		case PlayerType::HumanPlayer:	// プレイヤ操作
			//モーションの切り替え
			switch (nowState)
			{
			case State::PWait:	// 待機状態
				if (input.B1.down && this->existFood == true) { nowState = State::PEat; } // 食事中へ
				break;

			case State::PEat:	// 食事中状態
				if (this->moveCnt == 30 || this->existFood == false) { nowState = State::PWait; } // 待機へ
				break;
			}
			break;

		case PlayerType::CPUPlayer:		// CPU操作
			break;
		}

		//状態更新
		this->UpdateState(nowState);
	}
	//-------------------------------------------------------------------
	//状態毎の行動処理
	void Object::Move()
	{
		XI::VGamePad input;	// 入力情報を格納する
		if (controller) // nullチェック
		{
			input = this->controller->GetState(); // 入力情報の取得
		}

		switch (playerType) // プレイヤかCPUかの判別
		{
		case PlayerType::HumanPlayer:	//プレイヤ操作

			//☆行動処理
			switch (this->state)
			{
			case State::PWait:	//待機状態
				//☆ゲーム本編開始時に一度だけ行う処理
				if (this->isPlayStart == false)
				{
					//ボタンの描画を開始する
					this->buttonDrawPos = ML::Vec2(this->pos.x, this->pos.y - 400);
					this->isStartButtonDraw = true;

					this->isPlayStart = true;
				}
				break;

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
							//料理を食べたSEを鳴らす
							se::Play("eatSE");
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
			break;

		case PlayerType::CPUPlayer:		//CPU操作
			break;
		}

		
	}
	//-------------------------------------------------------------------
	//アニメーション制御
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(-374 / 2, -512 / 2, 748 / 2, 1024 / 2), ML::Box2D(0, 0, 748, 1024) },	//待機、食事中2
			{ ML::Box2D(-374 / 2, -512 / 2, 748 / 2, 1024 / 2), ML::Box2D(748, 0, 748, 1024) },	//食事中1
		};

		DrawInformation rtv;
		int animationNum; //アニメーション用
		switch (this->state)
		{
		case State::PWait:		//待機状態
			rtv = imageTable[0];
			break;

		case State::PEat:		//食事中状態
			animationNum = (8 + this->animationCount) / 8; //食事中1から描画を始める
			animationNum %= 2;
			rtv = imageTable[animationNum];
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
			case State::PWait:	//待機状態
				this->res->buttonImage_A_Outline->Draw(drawButtonImage.draw, drawButtonImage.src);
				break;

			case State::PEat:	//食事中状態
				int animationNum = this->animationCount / 5;
				animationNum %= 2;

				if (animationNum == 0)	//Aボタンの画像描画
				{
					this->res->buttonImage_A->Draw(drawButtonImage.draw, drawButtonImage.src);
				}
				else					//Aボタン外枠のみの画像描画
				{
					this->res->buttonImage_A_Outline->Draw(drawButtonImage.draw, drawButtonImage.src);
				}
				break;
			}
		}
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
		existFood(false),
		isPlayStart(false),
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
/*メモ*/
/*Releaseで入力情報が取得出来ていない場合、input宣言時に値の初期化を行っていないのが原因かも*/