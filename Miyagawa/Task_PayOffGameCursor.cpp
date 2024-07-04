//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_PayOffGameCursor.h"
#include  "Task_PayOffGameMoney.h"
#include "../randomLib.h"

namespace PayOffGameCursor
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/debugRect.png");
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
		TestFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);
		speed = 5.f;
		src = ML::Box2D(0, 0, 32, 32);
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
	//思考
	void  Object::NormalState::think()
	{
		if (owner_->isClear) {
			owner_->ChangeState(new ClearState(owner_));
		}
	}
	//-------------------------------------------------------------------
	//行動
	void  Object::NormalState::move()
	{
		owner_->input = owner_->controller->GetState();
		if (owner_->input.LStick.volume > 0) {
			owner_->moveVec = ML::Vec2(cos(owner_->input.LStick.angleDYP),
				sin(owner_->input.LStick.angleDYP)) * owner_->speed;
		}
		else {
			owner_->moveVec *= 0.9f;
		}
		owner_->pos += owner_->moveVec;
		owner_->CheckHit(ge->GetTask<PayOffGameMoney::Object>(PayOffGameMoney::defGroupName));
	}
	//-------------------------------------------------------------------
	//描画
	void  Object::NormalState::render()
	{
		//描画矩形
		ML::Box2D draw(
			-owner_->src.w,
			-owner_->src.h,
			owner_->src.w * 2,
			owner_->src.h * 2
		);

		draw.Offset(owner_->pos);

		owner_->res->img->Draw(draw, owner_->src);
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
	Object::Object() :state(new NormalState(this)), isClear(false) {	}
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