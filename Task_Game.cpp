//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "StageAlarmClock/Task_StageAlarmClock.h"
#include  "StageBrushTeeth/Task_StageBrushTeeth.h"
#include  "BlanceGame/Task_BlanceGame.h"
#include  "ClassifyGame/Task_ClassifyGame.h"
#include  "Ohara/Task_OguiGame.h"
#include  "Ohara/Task_SaboriGame.h"
#include  "Miyagawa/Task_TaxiGame.h"

#include  <chrono>
#include  <thread>

#include  "randomLib.h"
#include  "Ohara/Task_Result.h"


namespace  Game
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
		//reset score
		for (int i = 0; i < 4; ++i) {
			//ge->score[i] = 0;
			ge->SetScore(i, 0);
		}
		//デバッグ用フォントの準備
		TestFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);

		//★タスクの生成
		CreateTask();
    
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("本編");

		if (!ge->QuitFlag() && nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto next = Result::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		
	}
	//-------------------------------------------------------------------
	//タスクの生成
	//引数なしなら最初のタスクを生成
	void  Object::CreateTask() {
		StageAlarmClock::Object::Create(true);
	}
	void  Object::CreateTask(int nextTask) {
		int i = 0;
		i = nextTask;
		//auto gsUI=gamesetUI::Object::Create(true);  //ゲームセットUI生成用
		/*std::this_thread::sleep_for(std::chrono::seconds(2));*/
		switch (i) {
		case 1:
			StageBrushTeeth::Object::Create(true);
			break;
		case 2:
			BlanceGame::Object::Create(true);
			break;
		case 3:
			ClassifyGame::Object::Create(true);
			break;
		case 4:
			SaboriGame::Object::Create(true);
			break;
		case 5:
			OguiGame::Object::Create(true);
			break;
		case 6:
			TaxiGame::Object::Create(true);
			break;
		default:
			auto g = ge->GetTask < Game::Object >("本編");
			g->Kill();
			break;
		}
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