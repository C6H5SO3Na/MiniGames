//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGamePlayer.h"
#include "../randomLib.h"

namespace TaxiPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
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
		TestFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);
		//★タスクの生成
		nowBtn = GetRandom(0, 3);
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
	//受け身
	void  Object::MatchButton()
	{
		++matchCnt;
		nowBtn = GetRandom(0, 3);
	}
	//-------------------------------------------------------------------
	//思考
	void  Object::NormalState::think()
	{
		if (owner_->matchCnt >= 10) {
			owner_->ChangeState(new ClearState(owner_));
		}
	}
	//-------------------------------------------------------------------
	//行動
	void  Object::NormalState::move()
	{
		if (owner_->input.B1.down) {
			if (owner_->nowBtn == 0) {
				owner_->MatchButton();
			}
		}
		if (owner_->input.B2.down) {
			if (owner_->nowBtn == 1) {
				owner_->MatchButton();
			}
		}
		if (owner_->input.B3.down) {
			if (owner_->nowBtn == 2) {
				owner_->MatchButton();
			}
		}
		if (owner_->input.B4.down) {
			if (owner_->nowBtn == 3) {
				owner_->MatchButton();
			}
		}
	}
	//-------------------------------------------------------------------
	//描画
	void  Object::NormalState::render()
	{
		//描画矩形
		ML::Box2D draw(
			static_cast<int>(owner_->pos.x),
			static_cast<int>(owner_->pos.y),
			ge->screen2DWidth,
			ge->screen2DHeight
		);
		owner_->TestFont->Draw(draw, owner_->btn[owner_->nowBtn]);
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
		//描画矩形
		ML::Box2D draw(
			static_cast<int>(owner_->pos.x),
			static_cast<int>(owner_->pos.y),
			ge->screen2DWidth,
			ge->screen2DHeight
		);

		owner_->TestFont->Draw(draw, owner_->btn[owner_->nowBtn]);

		owner_->TestFont->Draw(ML::Box2D(draw.x - 100, draw.y - 100, draw.w, draw.h),
			"CLEAR!", ML::Color(1.f, 1.f, 0.f, 1.f)
		);
	}
	//段階を変更
	void  Object::ChangeState(StateBase* state_)
	{
		delete state;
		state = state_;
		moveCnt = 0;
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
	Object::Object() :state(new NormalState(this)) {	}
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