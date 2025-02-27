//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_hand.h"
#include  "Task_Clock.h"
#include  "Task_CommonItemManager01.h"
#include "../sound.h"

namespace  hand
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/hand_1.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//SE
		se::LoadFile("Stop", "./data/sound/se/AlarmClockGame/Clock-Alarm05-5(Toggle).wav");
		se::LoadFile("Ring", "./data/sound/se/AlarmClockGame/alarmRing.wav");
		se::SetVolume("Ring", 25);
		se::SetVolume("Stop", 100);

		//★データ初期化
		this->render2D_Priority[1] = -0.6f;
		this->hitBase = ML::Box2D(-70, -25, 140, 70);
		this->drawBase = ML::Box2D(-84, -53, 168, 106);
		this->pos.x = 0;
		this->pos.y = 0;
		this->speed = 32.0f;
		this->controller = ge->in1;
		this->state = State::Right;
		isright = true;

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			se::Stop("Ring");
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		
		switch (ge->gameState)
		{
		case MyPG::MyGameEngine::GameState::Game:
			se::PlayLoop("Ring");
			this->pos += this->moveVec;
			break;
		case MyPG::MyGameEngine::GameState::Finish:
			this->pos = this->pos;
			se::Stop("Ring");
			se::Play("Stop");
			break;
		}
		

		switch (this->state)
		{
		case State::Left:
			this->moveVec = ML::Vec2(-2 * this->speed, 0);
			if (this->pos.x <= minPosX +16)
			{
				this->state = State::Right;
				isright = true;
			}
			break;
		case State::Right:
			this->moveVec = ML::Vec2(2 * this->speed, 0);
			if (this->pos.x >= maxPosX -16)
			{
				this->state = State::Left;
				isright = false;
			}
			break;
		case State::Down:
			this->moveVec = ML::Vec2(0, 2 * this->speed);
			if (this->pos.y >= maxPosY)
			{
				this->state = State::Up;
			}
			break;
		case State::Up:
			this->moveVec = ML::Vec2(0, -2 * this->speed);
			if (this->pos.y <= minPosY)
			{
				if (isright)
				{
					this->state = State::Right;
				}
				else
				{
					this->state = State::Left;
				}
				
			}
			break;
		}

		if (this->controller) {
			auto inp = this->controller->GetState();
			if (inp.LStick.BD.down) { state = State::Down; }
		}
		
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
		//ge->debugRect(me, 4);
		auto t = ge->GetTasks<Clock::Object>("目覚まし時計");
		
		for (auto it = t->begin(); it != t->end(); ++it)
		{
			auto you = (*it)->hitBase.OffsetCopy((*it)->pos);
			//ge->debugRect(you, 5);
			if (you.Hit(me))
			{
				se::Stop("Ring");
				se::Play("Stop");
				this->speed = 0;
				this->moveVec = ML::Vec2(0, 0);
				(*it)->animLine = 1;
				if (!this->isClear)
				{
					this->isClear = true;
					//ge->score[(*it)->id] += com->addscore[com->rank]; //addscore
					/*ge->AddScore((*it)->id, com->addscore[com->rank]);
					com->rank++;*/
				}
				
			}
		}
		
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src(0, 0, 1184, 748);
		this->res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	void Object::Positionalise(ML::Box2D PlayerAreaPos_)
	{
		ML::Box2D PlayerArea = PlayerAreaPos_;
		pos.x = (float)PlayerArea.x + (drawBase.w / 2);
		pos.y = (float)PlayerArea.y + (drawBase.h / 2);
		minPosX = (float)PlayerArea.x + (drawBase.w / 2) + 60;
		minPosY = (float)PlayerArea.y + (drawBase.h / 2) + 2 * this->speed;
		maxPosX = (float)PlayerArea.x + PlayerArea.w - drawBase.w - 20;
		maxPosY = (float)PlayerArea.y + PlayerArea.h / 2;
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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