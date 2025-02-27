//-------------------------------------------------------------------
//Q[ΜΕΙoιw¦
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_EasingLogo.h"
#include  "Task_GameMessage.h"

namespace EasingLogo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		img.push_back(DG::Image::Create("./data/image/PopUpWord_Stage1.png"));
		img.push_back(DG::Image::Create("./data/image/PopUpWord_Stage2.png"));
		img.push_back(DG::Image::Create("./data/image/PopUpWord_Stage3.png"));
		img.push_back(DG::Image::Create("./data/image/PopUpWord_Stage4.png"));
		img.push_back(DG::Image::Create("./data/image/PopUpWord_Stage5.png"));
		img.push_back(DG::Image::Create("./data/image/PopUpWord_Stage6.png"));
		img.push_back(DG::Image::Create("./data/image/PopUpWord_Stage7.png"));
		img.push_back(DG::Image::Create("./data/image/Finish.png"));
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		for (auto& i : img) {
			i.reset();
		}
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
		if (!isCreated) {
			if (isFinish) {
				logo = GameMessage::Object::Create(res->img[7], srcTable[7], "FinishSE");
			}
			else {
				logo = GameMessage::Object::Create(res->img[ge->nowStage], srcTable[ge->nowStage], "StartSE");
			}
			isCreated = true;
		}
		if (logo.lock() == nullptr) {
			Kill();
		}
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
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
	Object::Object():isFinish(false) {}
	//-------------------------------------------------------------------
	Object::SP Object::Spawn(const bool& isFinish)
	{
		auto logo = Create(true);
		logo->isFinish = isFinish;
		return logo;
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