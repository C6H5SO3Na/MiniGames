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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/game_dirt.png");
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

		//★データ初期化
		this->render2D_Priority[1] = -0.5f;
		this->pos.x = 0;
		this->pos.y = 0;
		this->hitBase = ML::Box2D(-20, -20, 40, 40);
		this->timeCnt = 0;
		this->animCnt = 0;
		this->animIndex = 0;
		this->speed = GetRandom(1.0f, 5.0f);
		dx = 0;
		dy = 0;
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
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->timeCnt++;
		this->animCnt++;
		//アニメ更新
		if (this->animCnt >= 5)
		{
			this->animCnt = 0;
			this->animIndex++;
			if (this->animIndex >= 3)
			{
				this->animIndex = 0;
			}
		}

		//ランダムで真っ直ぐやまるで動く
		if (rand() % 100 < 20) { // State変わる％
			State = (State == MovementState::Forward) ? MovementState::Circular : MovementState::Forward;
		}

		switch (ge->gameState)
		{
		case MyPG::MyGameEngine::GameState::Start:
			if (State == MovementState::Forward) {
				//ランダムで動く
				//float direction = GetRandom(0.f, 2 * (float)M_PI); // ラジアンで方向を計算する
				//dx = speed * cos(direction);
				//dy = speed * sin(direction);
				float dx = (GetRandom(-1, 1) * speed); //右左
				float dy = (GetRandom(-1, 1) * speed); //上下
			}
			else if (State == MovementState::Circular) {
				//まるで動くt
				float radius = GetRandom(1.f, 5.f);
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
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src(animIndex * 256, 0, 256, 256);

		this->res->img->Draw(draw, src);
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