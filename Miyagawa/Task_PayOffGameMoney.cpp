//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_PayOffGameMoney.h"
#include "../randomLib.h"

namespace PayOffGameMoney
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/money.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		res = Resource::Create();

		//★データ初期化
		render2D_Priority[1] = 0.9f;

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && nextTaskCreate) {
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
		//draw,src,color
		DrawInfo drawInfoTable[] =
		{
			{ {  0,   0, 128, 128}, {  0,   0, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {128,   0, 128, 128}, {128,   0, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {256,   0, 128, 128}, {256,   0, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {  0, 128, 128, 128}, {  0, 128, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {128, 128, 128, 128}, {128, 128, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {256, 128, 128, 128}, {256, 128, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {  0, 256, 256, 128}, {  0, 256, 256, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {256, 256, 256, 128}, {256, 256, 256, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {  0, 384, 256, 128}, {  0, 384, 256, 128}, {1.f, 1.f, 1.f, 1.f}},
		};


		for (DrawInfo i : drawInfoTable) {
			i.draw.Offset(800, 300);
			res->img->Draw(i.draw, i.src);
		}
	}

	//-------------------------------------------------------------------
	//受け身
	void  Object::Recieved()
	{
		int point[] = {
			1,5,10,50,100,500,1000,5000,10000
		};
		ge->isGameClear;
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
	Object::Object() {}
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
	Resource::~Resource() { Finalize(); }
}