//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_Clock.h"
#include  "Task_hand.h"

namespace  Clock
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/game_clock.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		this->render2D_Priority[1] = -0.5f;
		this->hitBase = ML::Box2D(-65, -70, 130, 140);
		this->drawBase = ML::Box2D(-75, -75, 150, 150);
		this->pos.x = 0;
		this->pos.y = 0;
		this->animCnt = 0;
		this->animLine = 0;
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
		animCnt++;
		//AjXV
		if (this->animCnt >= 5)
		{
			this->animCnt = 0;
			this->animIndex++;
			if (this->animIndex >= 5)
			{
				this->animIndex = 0;
			}
		}

		/*auto players = ge->GetTasks <hand::Object>("θ");
		for_each(players->begin(), players->end(), [&](auto iter) {
			if (iter->IsClear()) {
				animLine = 1;
			}
			})*/;

		/*auto pl = ge->GetTask<hand::Object>("θ");
		if (pl->isClear)
		{
			animLine = 1;
		}*/
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);

		int animTable[2][5] =
		{
			{ 0, 1, 2, 3, 4},	//ringing 0
			{ 0, 0, 0, 0, 0},	//stop 1
		};

		int animNum = animTable[animLine][animIndex];

		int srcX = animNum % 5 * 764;
		int srcY = animNum / 5 * 764;


		ML::Box2D src(srcX, srcY, 764, 764);
		this->res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	void Object::Positionalise(int PlayerNum)
	{
		ML::Box2D PlayerArea(PlayerNum % 2 * (1980 / 2), PlayerNum / 2 * (1080 / 2), (1980 / 2), (1080 / 2));
		pos.x = PlayerArea.x + (PlayerArea.w / 2);
		pos.y = PlayerArea.y + (PlayerArea.h / 4) * 3;
	}
	//-------------------------------------------------------------------
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