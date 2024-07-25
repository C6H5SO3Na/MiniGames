//-------------------------------------------------------------------
//プレイヤー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGame.h"
#include  "Task_TaxiGamePlayer.h"
#include "../randomLib.h"
#include "../easing.h"
#include "../sound.h"

namespace TaxiGamePlayer
{
	Resource::WP  Resource::instance;
	int Object::playerScore;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//プレイヤー画像
		imgPlayer = DG::Image::Create("./data/image/chara02.png");

		//クリア画像
		imgClear = DG::Image::Create("./data/image/clearImage.png");

		//ボタンの画像(アニメーション込みのため二次元配列)
		imgBtn[0][0] = DG::Image::Create("./data/image/button/default/xbox_button_color_a.png");
		imgBtn[1][0] = DG::Image::Create("./data/image/button/default/xbox_button_color_a_outline.png");
		imgBtn[0][1] = DG::Image::Create("./data/image/button/default/xbox_button_color_b.png");
		imgBtn[1][1] = DG::Image::Create("./data/image/button/default/xbox_button_color_b_outline.png");
		imgBtn[0][2] = DG::Image::Create("./data/image/button/default/xbox_button_color_x.png");
		imgBtn[1][2] = DG::Image::Create("./data/image/button/default/xbox_button_color_x_outline.png");
		imgBtn[0][3] = DG::Image::Create("./data/image/button/default/xbox_button_color_y.png");
		imgBtn[1][3] = DG::Image::Create("./data/image/button/default/xbox_button_color_y_outline.png");
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
		res = Resource::Create();

		//★データ初期化
		render2D_Priority[1] = 0.5f;
		nowBtn = GetRandom(0, 3);

		//SE
		se::LoadFile("Miss", "./data/sound/se/ClassifyGame/maou_se_onepoint33.wav");
		se::LoadFile("Clear", "./data/sound/se/TaxiGame/シャキーン1.wav");
		se::LoadFile("Walk", "./data/sound/se/TaxiGame/Footsteps02-2L.wav");

		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		input = controller->GetState();
		Think();
		Move();
		easing::UpDate();
		++animCnt;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		state->render();
	}
	//-------------------------------------------------------------------
	//思考
	void  Object::Think()
	{
		state->think();
	}
	//-------------------------------------------------------------------
	//行動
	void  Object::Move()
	{
		state->move();
	}
	//-------------------------------------------------------------------
	//受け身
	void  Object::Recieved()
	{
	}
	//-------------------------------------------------------------------
	//ボタンが一致したときの処理
	void  Object::MatchButton()
	{
		++matchCnt;
		if (matchCnt >= 10) {
			isClear = true;
			return;
		}
		nowBtn = GetRandom(0, 3);
	}
	//-------------------------------------------------------------------
	//ボタンの描画
	void  Object::DrawButton()
	{
		//描画矩形
		ML::Box2D src(0, 0, 64, 64);
		ML::Box2D draw(
			static_cast<int>(pos.x),
			static_cast<int>(pos.y),
			src.w,
			src.h
		);
		res->imgBtn[animCnt / 10 % 2][nowBtn]->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//思考
	void  Object::IdleState::think()
	{
		if (owner_->BUTTON(0) == 0) { return; }
		if (owner_->BUTTON(0) == pow(2, 4 + owner_->nowBtn)) {//ビット単位のための計算
			owner_->ChangeState(new MoveState(owner_));
			easing::Set("move", easing::QUADINOUT, 0, -150, 30);
			easing::Start("move");
			se::Play("Walk");
		}
		else {
			owner_->ChangeState(new MissState(owner_));
			se::Play("Miss");
		}
	}
	//-------------------------------------------------------------------
	//行動
	void  Object::IdleState::move()
	{
	}
	//-------------------------------------------------------------------
	//描画
	void  Object::IdleState::render()
	{
		//プレイヤ描画
		ML::Box2D src(0, 0, 32, 80);
		ML::Box2D draw(-src.w, -src.h, -src.w * 2, src.h * 2);
		draw.Offset(owner_->pos);
		owner_->res->imgPlayer->Draw(draw, src);

		owner_->DrawButton();
	}

	//-------------------------------------------------------------------
	//思考
	void  Object::MoveState::think()
	{
		if (easing::GetState("move") == easing::EQ_STATE::EQ_END) {
			owner_->MatchButton();
			if (owner_->isClear) {
				owner_->AddScore(playerScore, owner_->controller);
				owner_->ChangeState(new ClearState(owner_));
				se::Play("Clear");
				return;
			}
			owner_->ChangeState(new IdleState(owner_));
		}
	}
	//-------------------------------------------------------------------
	//行動
	void  Object::MoveState::move()
	{
		owner_->pos.x = easing::GetPos("move") + owner_->prePos.x;
	}
	//-------------------------------------------------------------------
	//描画
	void  Object::MoveState::render()
	{
		ML::Box2D animTable[] = {
			ML::Box2D(32, 0, 32, 80),
			ML::Box2D(64, 0, 48, 80),
			ML::Box2D(112, 0, 48, 80),
		};
		ML::Box2D src = animTable[owner_->animCnt / 30 % size(animTable)];
		ML::Box2D draw(-src.w, -src.h, -src.w * 2, src.h * 2);
		draw.Offset(owner_->pos);
		owner_->res->imgPlayer->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//思考
	void  Object::ClearState::think()
	{
	}
	//-------------------------------------------------------------------
	//行動
	void  Object::ClearState::move()
	{
	}
	//-------------------------------------------------------------------
	//描画
	void  Object::ClearState::render()
	{
		{
			//描画矩形
			ML::Box2D src(0, 0, 32, 80);
			ML::Box2D draw(-src.w, -src.h, -src.w * 2, src.h * 2);
			draw.Offset(owner_->pos);
			owner_->res->imgPlayer->Draw(draw, src);
		}

		//描画矩形
		ML::Box2D src(0, 0, 97, 25);
		ML::Box2D draw(-src.w / 2, -src.h / 2, src.w, src.h);
		draw.Offset(owner_->pos);
		owner_->res->imgClear->Draw(draw, src);
	}

	//-------------------------------------------------------------------
	//思考
	void  Object::MissState::think()
	{
		if (owner_->moveCnt == 120) {
			owner_->ChangeState(new IdleState(owner_));
		}
	}
	//-------------------------------------------------------------------
	//行動
	void  Object::MissState::move()
	{
		++owner_->moveCnt;
	}
	//-------------------------------------------------------------------
	//描画
	void  Object::MissState::render()
	{
		{
			//描画矩形
			ML::Box2D src(176, 0, 48, 80);
			ML::Box2D draw(-src.w, -src.h, -src.w * 2, src.h * 2);
			draw.Offset(owner_->pos);
			//振動
			draw.Offset(GetRandom(0, 10), GetRandom(0, 10));
			owner_->res->imgPlayer->Draw(draw, src);
		}

		owner_->DrawButton();
	}
	//段階を変更
	void  Object::ChangeState(StateBase* state_)
	{
		delete state;
		state = state_;
		moveCnt = 0;
		prePos = pos;
	}

	//ボタン(入力をビットで入手)
	int  Object::BUTTON(int state)
	{
		//それぞれのボタン判定
		struct Input {
			bool A = false;
			bool B = false;
			bool X = false;
			bool Y = false;
		};
		Input inp[4] = {
			{input.B1.down, input.B2.down, input.B3.down, input.B4.down},
			{input.B1.on, input.B2.on, input.B3.on, input.B4.on},
			{input.B1.up, input.B2.up, input.B3.up, input.B4.up},
			{input.B1.off, input.B2.off, input.B3.off, input.B4.off}
		};

		int rtv = 0;
		if (inp[state].A) {
			rtv |= A;
		}
		if (inp[state].B) {
			rtv |= B;
		}
		if (inp[state].X) {
			rtv |= X;
		}
		if (inp[state].Y) {
			rtv |= Y;
		}
		return rtv;
	}

	//スコア加算
	void Object::AddScore(int& score, XI::GamePad::SP con)
	{
		if (con == ge->in1) {
			ge->score[0] += score;
		}
		if (con == ge->in2) {
			ge->score[1] += score;
		}
		if (con == ge->in3) {
			ge->score[2] += score;
		}
		if (con == ge->in4) {
			ge->score[3] += score;
		}
		--score;
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
		return  Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() :state(new IdleState(this)), isClear(false) {	}
	//-------------------------------------------------------------------
	void Object::Spawn(const ML::Vec2& pos_, XI::GamePad::SP controller_)
	{
		auto player = Create(true);
		player->pos = pos_;
		player->controller = controller_;
	}
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
	Resource::~Resource() { Finalize(); }
}