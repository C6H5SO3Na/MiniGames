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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create(string("./data/image/") + ClockImageList[PlayerNumId]);
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
		this->res = Resource::Create(PlayerNumId);

		//★データ初期化
		this->render2D_Priority[1] = -0.5f;
		this->hitBase = ML::Box2D(-65, -70, 130, 140);
		this->drawBase = ML::Box2D(-75, -75, 150, 150);
		this->pos.x = 0;
		this->pos.y = 0;
		this->animCnt = 0;
		this->animLine = 0;
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
		switch (ge->gameState)
		{
		case MyPG::MyGameEngine::GameState::Game:
			animCnt++;
			//アニメ更新
			if (this->animCnt >= 5)
			{
				this->animCnt = 0;
				this->animIndex++;
				if (this->animIndex >= 5)
				{
					this->animIndex = 0;
				}
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
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
	void Object::Positionalise(ML::Box2D PlayerAreaPos_)
	{
		ML::Box2D PlayerArea = PlayerAreaPos_;
		pos.x = PlayerArea.x + (PlayerArea.w / 2);
		pos.y = PlayerArea.y + (PlayerArea.h / 4) * 3;
	}
	//-------------------------------------------------------------------
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, int PlayerNumId_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				ob->PlayerNumId = PlayerNumId_;
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
	Resource::SP  Resource::Create(int PlayerNumId_)
	{
		/*if (auto sp = instance.lock()) {
			return sp;
		}
		else {*/
		auto sp = Resource::SP(new  Resource());
		if (sp) {
			sp->PlayerNumId = PlayerNumId_;
			sp->Initialize();
			instance = sp;
		}
		return sp;
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}