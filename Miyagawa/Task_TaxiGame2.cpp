//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGame2.h"

#include "../randomLib.h"

#include  "../Task_Ending.h"
#include  "Task_TaxiGamePlayer2.h"
#include  "Task_TaxiGameTaxi.h"

namespace  TaxiGame2
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
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


		//★タスクの生成
		TaxiGamePlayer2::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 1 / 5.f), ge->in1);
		TaxiGamePlayer2::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 2 / 5.f), ge->in2);
		TaxiGamePlayer2::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 3 / 5.f), ge->in3);
		TaxiGamePlayer2::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 4 / 5.f), ge->in4);

		for (int i = 0; i < 4; ++i) {
			TaxiGameTaxi::Object::Spawn(ML::Vec2(200.f, ge->screenHeight * (1 + i) / 5.f));
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("本編");
		ge->KillAll_G("プレイヤー");

		if (!ge->QuitFlag() && nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//auto inp = ge->in1->GetState( );
		//if (inp.ST.down) {
		//	ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
		//	ge->CreateEffect(99, ML::Vec2(0, 0));
		//}
		//if (ge->getCounterFlag("test") == ge->LIMIT) {
		//	Kill();
		//}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
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
	Resource::~Resource() { Finalize(); }
}