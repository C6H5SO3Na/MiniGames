//-------------------------------------------------------------------
//サボりミニゲーム本編
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriGame.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriJoushi.h"
#include  "Task_SaboriUIManager.h"

#include  "../randomLib.h"

#include  "../Task_Ending.h"

namespace  SaboriGame
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
		//プレイヤー
		for (int i = 0; i < size(controllers); ++i)
		{
			auto p = SaboriPlayer::Object::Create(true);
			p->pos = this->playerFirstPos[i];
			p->controller = this->controllers[i];
			p->playerNum = playersNum[i];
		}

		//上司
		auto j = SaboriJoushi::Object::Create(true);
		j->pos = joushiFirstPos;

		//UI管理
		SaboriUIManager::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("本編");
		ge->KillAll_G("プレイヤー");
		ge->KillAll_G("ギミック");
		ge->KillAll_G("管理");

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
		//☆制限時間を減らす
		if (timeLimit > 0)
		{
			timeLimit -= 1.f / 60; // / 60 を / GetFps()に変更してモニターFPSにゲームが依存しないようにする
		}
		//制限時間が0以下だったら0にする
		if (timeLimit <= 0.f)
		{
			timeLimit = 0.f;
		}

		//☆制限時間が0になったらミニゲームを終了させる
		if (timeLimit == 0.f && cnt == 0)
		{
			isGameOver = true;
			cnt++;
		}

		auto inp = ge->in1->GetState( );
		if (isGameOver) {
			ge->StartCounter("test", 60 * 10);
			isGameOver = false;
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			Kill();
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
	Object::Object()
		:
		timeLimit(60.f), //制限時間を設定
		isGameOver(false),
		cnt(0)
	{	}
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