//-------------------------------------------------------------------
//oXQ[th}l[W[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_BlanceUIManager.h"
#include  "Task_BlanceGame.h"
#include  "Task_BlanceGamePM.h"

namespace  BlanceGUIM
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		backImg = DG::Image::Create("./data/image/train.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		backImg.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		render2D_Priority[1] = 0.5;
		pos = ML::Vec2{ 0,0 };
		SetBGState(BGstate::Start);
		for (int i = 0; i < 4; ++i) {
			failFlag[i] = false;
		}
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		auto bg = ge->GetTask<BlanceGame::Object>("oXQ[");
		if (!bg) {
			return;
		}
		if (bg->shake) {
			pos.x += 239;
		}
		else
		{
			pos.x -= 1;
		}
		auto pm = ge->GetTask<BlanceGamePM::Object>("oXQ[PM");
		for (int i = 0; i < 4; ++i) {
			if (pm->pList[i]->GetBGState() == BGstate::Fail) {
				failFlag[i] = true;
			}
		}
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D src = ML::Box2D(0, 0, 1255, 1058);
		ML::Box2D draw = ML::Box2D(0, 0, 1255, 1058);
		draw = draw.OffsetCopy(pos);
		res->backImg->Draw(src, draw);
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
	Object::Object() {	}
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
	Resource::~Resource() { this->Finalize(); }
}