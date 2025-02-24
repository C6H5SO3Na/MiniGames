//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../randomLib.h"
#include  "../MyPG.h"
#include  "Task_stain.h"
#include  "Task_StainManager.h"
#define _USE_MATH_DEFINES
#include  "math.h"
#include "algorithm"

namespace  stain
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XÌú»
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/game_dirt.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XÌðú
	bool  Resource::Finalize()
	{
		this->img.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//uú»v^XN¶¬ÉPñ¾¯s¤
	bool  Object::Initialize()
	{
		//X[p[NXú»
		__super::Initialize(defGroupName, defName, true);
		//\[XNX¶¬or\[X¤L
		this->res = Resource::Create();

		//f[^ú»
		this->render2D_Priority[1] = -0.5f;
		this->pos.x = 0;
		this->pos.y = 0;
		this->hitBase = ML::Box2D(-20, -20, 40, 40);
		this->timeCnt = 0;
		this->animCnt = 0;
		this->animIndex = 0;
		this->speed = GetRandom(1.0f, 7.0f);
		dx = 0;
		dy = 0;
		//^XNÌ¶¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uI¹v^XNÁÅÉPñ¾¯s¤
	bool  Object::Finalize()
	{
		//f[^^XNðú


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ø«p¬^XNÌ¶¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[És¤
	void  Object::UpDate()
	{
		this->timeCnt++;
		this->animCnt++;
		//AjXV
		if (this->animCnt >= 5)
		{
			this->animCnt = 0;
			this->animIndex++;
			if (this->animIndex >= 3)
			{
				this->animIndex = 0;
			}
		}

		//_Å^Á¼®âÜéÅ®­
		if (rand() % 100 < 30) { // StateÏíé
			State = (State == MovementState::Forward) ? MovementState::Circular : MovementState::Forward;
		}

		switch (ge->gameState)
		{
		case MyPG::MyGameEngine::GameState::Game:
			if (State == MovementState::Forward) {
				//_Å®­
				//float direction = GetRandom(0.f, 2 * (float)M_PI); // WAÅûüðvZ·é
				//dx = speed * cos(direction);
				//dy = speed * sin(direction);
				float dx = (GetRandom(-1, 1) * speed); //E¶
				float dy = (GetRandom(-1, 1) * speed); //ãº
			}
			else if (State == MovementState::Circular) {
				//ÜéÅ®­t
				float radius = GetRandom(1.f, 8.f);
				angle += 0.1f;
				dx = radius * cos(angle);
				dy = radius * sin(angle);
			}

			pos.x += dx;
			pos.y += dy;

			this->pos.x = max(minX, min(this->pos.x, maxX));
			this->pos.y = max(minY, min(this->pos.y, maxY));
			break;
		}

	}
	//-------------------------------------------------------------------
	//uQc`ævPt[És¤
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src(animIndex * 256, 0, 256, 256);

		this->res->img->Draw(draw, src);
	}


	//
	//ÈºÍî{IÉÏXsvÈ\bh
	//
	//-------------------------------------------------------------------
	//^XN¶¬û
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWÉo^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYÉ¸sµ½çKill
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
	//\[XNXÌ¶¬
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