//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiPlayer.h"
#include "../randomLib.h"

namespace TaxiPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
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
		TestFont = DG::Font::Create("lr SVbN", 30, 30);
		//^XNฬถฌ
		nowBtn = GetRandom(0, 3);
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
	//๓ฏg
	void  Object::MatchButton()
	{
		++matchCnt;
		nowBtn = GetRandom(0, 3);
	}
	//-------------------------------------------------------------------
	//vl
	void  Object::NormalState::think()
	{
		if (owner_->matchCnt >= 10) {
			owner_->ChangeState(new ClearState(owner_));
		}
	}
	//-------------------------------------------------------------------
	//sฎ
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
	//`ๆ
	void  Object::NormalState::render()
	{
		owner_->TestFont->Draw(ML::Box2D(owner_->pos.x, owner_->pos.y, ge->screen2DWidth, ge->screen2DHeight),
			owner_->btn[owner_->nowBtn]
		);
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
		owner_->TestFont->Draw(ML::Box2D(owner_->pos.x, owner_->pos.y, ge->screen2DWidth, ge->screen2DHeight),
			owner_->btn[owner_->nowBtn]
		);

		owner_->TestFont->Draw(ML::Box2D(owner_->pos.x - 100.f, owner_->pos.y - 100.f, ge->screen2DWidth, ge->screen2DHeight),
			"CLEAR!", ML::Color(1.f, 1.f, 0.f, 1.f)
		);
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
	Object::Object():state(new NormalState(this)) {	}
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