//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_PayOffGameMoney.h"
#include "../randomLib.h"

namespace PayOffGameMoney
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/money.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
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
		render2D_Priority[1] = 0.9f;

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
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		//draw,src,color
		DrawInfo drawInfoTable[] =
		{
			{ {  0,   0, 128, 128}, {  0,   0, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {128,   0, 128, 128}, {128,   0, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {256,   0, 128, 128}, {256,   0, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {  0, 128, 128, 128}, {  0, 128, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {128, 128, 128, 128}, {128, 128, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {256, 128, 128, 128}, {256, 128, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {  0, 256, 256, 128}, {  0, 256, 256, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {256, 256, 256, 128}, {256, 256, 256, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {  0, 384, 256, 128}, {  0, 384, 256, 128}, {1.f, 1.f, 1.f, 1.f}},
		};


		for (DrawInfo i : drawInfoTable) {
			i.draw.Offset(800, 300);
			res->img->Draw(i.draw, i.src);
		}
	}

	//-------------------------------------------------------------------
	//σ―g
	void  Object::Recieved()
	{
		int point[] = {
			1,5,10,50,100,500,1000,5000,10000
		};
		ge->isGameClear;
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
	Object::Object() {}
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