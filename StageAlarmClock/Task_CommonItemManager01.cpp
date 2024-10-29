//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_CommonItemManager01.h"
#include "Task_Clock.h"
#include "Task_hand.h"

namespace  CommonItemManager01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->bgImg = DG::Image::Create("./data/image/heya_blue.jpg");
		this->PlayerNum = DG::Image::Create("./data/image/PlayerNumber.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->bgImg.reset();
		this->PlayerNum.reset();
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
		this->rank = 0;

		/*CTList.push_back(ge->in1);
		CTList.push_back(ge->in2);
		CTList.push_back(ge->in3);
		CTList.push_back(ge->in4);*/

		for (auto i = 0; i < ge->players.size(); ++i)
		{
			auto c = Clock::Object::Create(true, i);
			ClockList.push_back(c);
			//ClockList[i]->id = i;

			auto h = hand::Object::Create(true);
			PLhandList.push_back(h);
			PLhandList[i]->id = i;

			c->Positionalise(PlayerAreaPos[ge->players.size() - 1][i]);
			h->Positionalise(PlayerAreaPos[ge->players.size() - 1][i]);

			h->controller = ge->players[i]/*CTList[i]*/;
		}
		

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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		for (int i = 0; i < ge->players.size(); ++i) {
			//背景
			ML::Box2D draw(0, 0, 1920 / 2, 1080 / 2);
			ML::Box2D src(0, 0, 1920, 1080);
			draw.Offset(PlayerAreaPos[ge->players.size() - 1][i].x, PlayerAreaPos[ge->players.size() - 1][i].y);
			this->res->bgImg->Draw(draw, src);

			//プレイヤーナンバー
			int ToRight = (i % 2) * 770;
			ML::Box2D draw01(0 + ToRight, 1080 / 2 - 105, 715 / 4, 105);
			ML::Box2D src01 = PlayerNumIndexSrc[i];
			draw01.Offset(PlayerAreaPos[ge->players.size() - 1][i].x, PlayerAreaPos[ge->players.size() - 1][i].y);
			this->res->PlayerNum->Draw(draw01, src01);
		}
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