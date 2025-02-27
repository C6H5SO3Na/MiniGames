//-------------------------------------------------------------------
//oXQ[ΜvC[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_BlanceGamePlayer.h"
#include  "../sound.h"

namespace  BGPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		playerImg = DG::Image::Create(string("./data/image/")+ playerImgPath[playerNum]);
		playerNumImg = DG::Image::Create("./data/image/PlayerNumber.png");
		controllerMarkL= DG::Image::Create("./data/image/LeftLeftStickDown.png");
		controllerMarkR = DG::Image::Create("./data/image/RightLeftStickDown.png");
		
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		
		playerNumImg.reset();
		controllerMarkL.reset();
		controllerMarkR.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create(playerNum);
		

		//f[^ϊ»
		direction = 0;
		animCnt = 0;
		SetBGState(BGstate::BStart);
	
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
		switch (GetBGState())
		{
		case BGstate::BStart:
			if (ge->gameState == MyPG::MyGameEngine::GameState::Start) {
				SetBGState(BGstate::Playing);
			}
			break;
		case BGstate::Playing:
			break;
		case BGstate::PlayR:
			direction += 1;
			if (in.LStick.BL.down) { SetBGState(BGstate::returnL); }
			if (direction > 45) { SetBGState(BGstate::Fail); }		
			break;
		case BGstate::PlayL:
			direction -= 1;
			if (in.LStick.BR.down) { SetBGState(BGstate::returnR); }
			if (direction < -45) { SetBGState(BGstate::Fail); }			
			break;
		case BGstate::returnL:
			direction -= 1;
			if (direction == 0) { SetBGState(BGstate::Playing); }
			break;
		case BGstate::returnR:
			direction += 1;
			if (direction == 0) { SetBGState(BGstate::Playing); }
			break;
		case BGstate::Fail:
			break;
		case BGstate::GameOver:
			pos.y = 600;
			direction = 90;
			break;
		}
		animCnt++;
		//AjXV
		if (this->animCnt >= 15)
		{
			this->animCnt = 0;
			this->animIndex++;
			if (this->animIndex >= 2)
			{
				this->animIndex = 0;
			}
		}
		
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D src(0, 0, 446, 542);
		ML::Box2D draw(-100, 0, 360, 432);
		//Rg[[}[N
		ML::Box2D Draw(0, -250 / 2, 150, 150);
		int srcX = animIndex % 2 * 128;
		int srcY = animIndex / 2 * 128;
		ML::Box2D Src(srcX, srcY, 128, 128);
		switch (GetBGState())
		{
		case BGstate::Playing:
			break;
		case BGstate::PlayR:
			src = ML::Box2D(2236, 543, 446, 542);
			this->res->controllerMarkL->Draw(Draw.OffsetCopy(pos), Src);
			break;
		case BGstate::PlayL:
			src = ML::Box2D(2236, 1086, 446, 542);
			this->res->controllerMarkR->Draw(Draw.OffsetCopy(pos), Src);
			break;
		case BGstate::returnR:
			src = ML::Box2D(2236, 543, 446, 542);
			break;
		case BGstate::returnL:
			src = ML::Box2D(2236, 1086, 446, 542);
			break;
		case BGstate::Fail:
			
			break;
		}
		res->playerImg->Rotation(ML::ToRadian(direction * 2), ML::Vec2(180, 432));
		res->playerImg->Draw(draw.OffsetCopy(pos), src);
		//playerNum
		src = playerNumUIsrc[playerNum];
		draw = playerNumUIdraw[playerNum];
		res->playerNumImg->Draw(draw.OffsetCopy(pos + ML::Vec2(0, -350)), src);
	}

	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, int playerNum_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
				ob->playerNum = playerNum_;
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
	Resource::SP  Resource::Create(int p)
	{
		auto sp = Resource::SP(new  Resource());
		if (sp) {
			sp->playerNum = p;
			sp->Initialize();
		}
		return sp;
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}