//-------------------------------------------------------------------
//^NV[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGameTaxi.h"
#include "../randomLib.h"

namespace TaxiGameTaxi
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/Taxi.png");
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
		render2D_Priority[1] = 0.7f;
		src = ML::Box2D(0, 0, 460, 460);

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
		Think();
		Move();
	}
	//-------------------------------------------------------------------
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(-src.w / 2, -src.h / 2, src.w, src.h);
		draw *= 0.5f;
		draw.Offset(pos);
		res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//vl
	void  Object::Think()
	{
	}
	//-------------------------------------------------------------------
	//sฎ
	void  Object::Move()
	{
	}
	//-------------------------------------------------------------------
	//๓ฏg
	void  Object::Received()
	{
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
	Object::Object() {}
	//-------------------------------------------------------------------
	void Object::Spawn(const ML::Vec2& pos_)
	{
		auto taxi = Create(true);
		taxi->pos = pos_;
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