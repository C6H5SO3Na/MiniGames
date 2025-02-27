//-------------------------------------------------------------------
//cθΤΜo[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TimeLimitBar.h"
#include  "Task_UIManager.h"
#include  "../Task_Game.h"

namespace TimeLimitBar
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/TimeBar.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		img.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		res = Resource::Create();

		//f[^ϊ»
		render2D_Priority[1] = -0.99f;
		srcBase = ML::Box2D(0, 0, 600, 100);

		//^XNΜΆ¬
		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ


		if (!ge->QuitFlag() && nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		switch (ge->gameState) {
		case MyPG::MyGameEngine::GameState::Start:
			gaugeAmount = ge->nowTimeLimit / maxCnt;
			break;
		case MyPG::MyGameEngine::GameState::Game:
			gaugeAmount = ge->nowTimeLimit / maxCnt;
			break;

		case MyPG::MyGameEngine::GameState::Finish:
			break;
		}
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		DrawFlame();
		DrawGauge();
	}
	//-------------------------------------------------------------------
	//o[Μg`ζ
	void Object::DrawFlame() const
	{
		ML::Box2D src(0, 0, srcBase.w, srcBase.h / 2);
		ML::Box2D draw(-src.w / 2, -src.h / 2, src.w, src.h);
		draw.Offset(pos);
		res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//o[ΜQ[W`ζ
	void Object::DrawGauge() const
	{
		int gSize = static_cast<int>(srcBase.w * gaugeAmount);
		ML::Box2D src(0, srcBase.h / 2, gSize, srcBase.h / 2);
		ML::Box2D draw(-srcBase.w / 2, -src.h / 2, gSize, srcBase.h / 2);
		draw.Offset(pos);
		res->img->Draw(draw, src);
	}
	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^

			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	Object::Object() :gaugeAmount(0.f), maxCnt(0.f), minPower(0), remainingCnt(0) {}
	//-------------------------------------------------------------------
	Object::SP Object::Create(const ML::Vec2& pos)
	{
		auto gauge = Create(true);
		gauge->pos = pos;
		gauge->maxCnt = ge->nowTimeLimit;
		return gauge;
	}
	//-------------------------------------------------------------------
	//\[XNXΜΆ¬
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