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
		se::LoadFile("Ring", "./data/sound/se/AlarmClockGame/Clock-Alarm05-7(Far-Low).wav");
		se::SetVolume("Stop", 100);

		//★データ初期化
		this->render2D_Priority[1] = -0.6f;
		this->hitBase = ML::Box2D(-70, -25, 140, 70);
		this->drawBase = ML::Box2D(-84, -53, 168, 106);
		this->pos.x = 0;
		this->pos.y = 0;
		this->speed = 30.0f;
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

		se::Stop("Ring");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		se::PlayLoop("Ring");
		this->pos += this->moveVec;

		switch (this->state)
		{
		case State::Left:
			this->moveVec = ML::Vec2(-2 * this->speed, 0);
			if (this->pos.x <= minPosX)
			{
				this->state = State::Right;
				isright = true;
			}
			break;
		case State::Right:
			this->moveVec = ML::Vec2(2 * this->speed, 0);
			if (this->pos.x >= maxPosX)
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
			if (inp.B1.down)//z
			{
				ge->KillAll_G("ステージ目覚まし時計");
			}
		}
		
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
		//ge->debugRect(me, 4);
		auto t = ge->GetTasks<Clock::Object>("目覚まし時計");
		auto com = ge->GetTask<CommonItemManager01::Object>("共通アイテムマネージャー01");
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
					ge->score[(*it)->id] += com->addscore[com->rank]; //addscore
					com->rank++;
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
	void Object::Positionalise(int PlayerNum)
	{
		ML::Box2D PlayerArea(PlayerNum % 2 * (1980 / 2), PlayerNum / 2 * (1080 / 2), (1980 / 2), (1080 / 2));
		pos.x = PlayerArea.x + (drawBase.w / 2);
		pos.y = PlayerArea.y + (drawBase.h / 2);
		minPosX = PlayerArea.x + (drawBase.w / 2);
		minPosY = PlayerArea.y + (drawBase.h / 2) + 2 * this->speed;
		maxPosX = PlayerArea.x + PlayerArea.w - drawBase.w;
		maxPosY = PlayerArea.y + PlayerArea.h / 2;
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