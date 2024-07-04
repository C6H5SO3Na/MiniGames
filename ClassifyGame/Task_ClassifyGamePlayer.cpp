//-------------------------------------------------------------------
//oXQ[ΜvC[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ClassifyGamePlayer.h"

namespace  CGPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		playerImg = DG::Image::Create("./data/image/chara02.png");
		for (int i = 0; i < 10; ++i) {
			bookImg[i] = DG::Image::Create("./data/image/debugrect.png");
		}
	
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		playerImg.reset();
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
		workTime = 0;
		SetCGState(CGstate::BStart);
		for (int i = 0; i < 10; ++i) {
			books[i].bpos = pos + ML::Vec2(0, -40);
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
		auto in = controller->GetState();
		switch (GetCGState())
		{
		case CGstate::Playing:
			break;
		case CGstate::PlayR:
			if (FirstIntoState()) {
				books[workTime].color = 0;
				workTime++;
			}
			if (in.LStick.BL.down) {
				moveCnt = 0;
				books[workTime].bpos += ML::Vec2(-30, -100);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt = 119) {
				toAnotherState(workTime);
			}
			break;
		case CGstate::PlayG:
			if (FirstIntoState()) {
				books[workTime].color = 2;
				workTime++;
			}
			if (in.LStick.BU.down) {
				moveCnt = 0;
				books[workTime].bpos += ML::Vec2(0, -100);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt = 119) {
				toAnotherState(workTime);
			}
			break;
		case CGstate::PlayB:
			if (FirstIntoState()) {
				books[workTime].color = 1;
				workTime++;
			}
			if (in.LStick.BR.down) {
				moveCnt = 0;
				books[workTime].bpos += ML::Vec2(30, 100);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt = 119) {
				toAnotherState(workTime);
			}
			break;
		case CGstate::Fail:
			
			break;
		}
		
		
	}
	void  Object::toAnotherState(int workT) {
		books[workT].color = 3;
		moveCnt = 0;
		books[workT].bpos += ML::Vec2(0, -200);
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D src(0, 0, 32, 80);
		ML::Box2D draw(0, 0, 32, 80);
		
		res->playerImg->Draw(draw.OffsetCopy(pos), src);
		src = ML::Box2D(0, 0, 32, 32);
		draw = ML::Box2D(0, 0, 32, 32);
		for (int i = 0; i < 10; ++i) {
			if (workTime > i) {
				src.x = books[i].color * 32;
				ML::Box2D draw0 = draw.OffsetCopy(books[i].bpos);
				res->bookImg[i]->Draw(draw0, src);
			}
		}
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