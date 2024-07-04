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
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/debugRect.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[Xฬ๐๚
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//u๚ปv^XNถฌษP๑พฏsค
	bool  Object::Initialize()
	{
		//X[p[NX๚ป
		__super::Initialize(defGroupName, defName, true);
		//\[XNXถฌor\[XคL
		res = Resource::Create();

		//f[^๚ป
		render2D_Priority[1] = 0.5f;
		TestFont = DG::Font::Create("lr SVbN", 30, 30);
		speed = 5.f;
		src = ML::Box2D(0, 0, 32, 32);
		//^XNฬถฌ

		return  true;
	}
	//-------------------------------------------------------------------
	//uIนv^XNมลษP๑พฏsค
	bool  Object::Finalize()
	{
		//f[^^XN๐๚


		if (!ge->QuitFlag() && nextTaskCreate) {
			//๘ซpฌ^XNฬถฌ
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[ษsค
	void  Object::UpDate()
	{
		input = controller->GetState();
		Think();
		Move();
	}
	//-------------------------------------------------------------------
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
		state->render();
	}
	//-------------------------------------------------------------------
	//vl
	void  Object::Think()
	{
		state->think();
	}
	//-------------------------------------------------------------------
	//sฎ
	void  Object::Move()
	{
		state->move();
	}
	//-------------------------------------------------------------------
	//๓ฏg
	void  Object::Recieved()
	{
	}
	//-------------------------------------------------------------------
	//vl
	void  Object::NormalState::think()
	{
		if (owner_->isClear) {
			owner_->ChangeState(new ClearState(owner_));
		}
	}
	//-------------------------------------------------------------------
	//sฎ
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
	//`ๆ
	void  Object::NormalState::render()
	{
		//`ๆ้`
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
	//vl
	void  Object::ClearState::think()
	{
	}
	//-------------------------------------------------------------------
	//sฎ
	void  Object::ClearState::move()
	{
	}
	//-------------------------------------------------------------------
	//`ๆ
	void  Object::ClearState::render()
	{
	}
	//iK๐ฯX
	void  Object::ChangeState(StateBase* state_)
	{
		delete state;
		state = state_;
		moveCnt = 0;
	}
	//
	//ศบอ๎{IษฯXsvศ\bh
	//
	//-------------------------------------------------------------------
	//^XNถฌ๛
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWษo^

			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYษธsตฝ็Kill
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
	//\[XNXฬถฌ
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