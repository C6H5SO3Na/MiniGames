//-------------------------------------------------------------------
//c่ิฬo[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TimeLimitBar.h"
#include  "Task_UIManager.h"
#include  "../Task_Game.h"

namespace TimeLimitBar
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/bar.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[Xฬ๐๚
	bool  Resource::Finalize()
	{
		img.reset();
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
		render2D_Priority[1] = 0.01f;
		srcBase = ML::Box2D(0, 0, 96, 32);

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
		--remainingCnt;
		remainingCnt = max(remainingCnt, 0);
		if (remainingCnt == 0) {
			auto g = ge->GetTask<Game::Object>("{า");
			g->gameState = Game::Object::GameState::Finish;
		}
		gaugeAmount = static_cast<float>(remainingCnt) / maxCnt;
	}
	//-------------------------------------------------------------------
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
		DrawFlame();
		DrawGauge();
	}
	//-------------------------------------------------------------------
	//o[ฬg`ๆ
	void Object::DrawFlame() const
	{
		ML::Box2D src(0, 0, srcBase.w, srcBase.h);
		ML::Box2D draw(-srcBase.w * 6 / 2, -srcBase.h * 2 / 2, srcBase.w * 6, srcBase.h * 2);
		draw.Offset(pos);
		res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//o[ฬQ[W`ๆ
	void Object::DrawGauge() const
	{
		int gSize = static_cast<int>(srcBase.w * gaugeAmount);
		ML::Box2D src(0, srcBase.h, gSize, srcBase.h);
		ML::Box2D draw(-srcBase.w * 6 / 2, -src.h * 2 / 2, gSize * 6, srcBase.h * 2);
		draw.Offset(pos);
		res->img->Draw(draw, src);
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
	Object::Object() :gaugeAmount(0.f), maxCnt(0), minPower(0), remainingCnt(0) {}
	//-------------------------------------------------------------------
	Object::SP Object::Create(const ML::Vec2& pos, const int& time)
	{
		auto gauge = Create(true);
		gauge->pos = pos;
		gauge->remainingCnt = time;
		gauge->maxCnt = gauge->remainingCnt;
		return gauge;
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