//-------------------------------------------------------------------
//UIマネージャー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "../Task_Game.h"
#include  "Task_UIManager.h"
#include  "Task_EasingLogo.h"
#include  "Task_FightLogo.h"
#include  "Task_TimeLimitBar.h"

namespace UIManager
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
		phase = Phase::RuleShow;

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
		auto g = ge->GetTask<Game::Object>("本編");
		switch (ge->gameState) {
		case MyPG::MyGameEngine::GameState::Start:

			switch (phase) {
			case Phase::RuleShow:
				ShowRule();
				phase = Phase::RuleEasing;
				break;

			case Phase::RuleEasing:
				if (ge->GetTask<EasingLogo::Object>(EasingLogo::defGroupName, EasingLogo::defName) == nullptr) {
					phase = Phase::StartShow;
				}
				break;

			case Phase::StartShow:
				CreateFightLogo();
				phase = Phase::StartEasing;
				break;

			case Phase::StartEasing:
				if (ge->GetTask<FightLogo::Object>(FightLogo::defGroupName, FightLogo::defName) == nullptr) {
					phase = Phase::FinishShow;
					ge->gameState = MyPG::MyGameEngine::GameState::Game;
				}
				break;
			}
			break;

		case MyPG::MyGameEngine::GameState::Game:

			break;

		case MyPG::MyGameEngine::GameState::Finish:
			switch (phase) {
			case Phase::FinishShow:
				ShowFinish();
				phase = Phase::FinishEasing;
				break;

			case Phase::FinishEasing:
				if (ge->GetTask<EasingLogo::Object>(EasingLogo::defGroupName, EasingLogo::defName) == nullptr) {
					phase = Phase::RuleShow;//使いまわし
					auto bar = ge->GetTask<TimeLimitBar::Object>(TimeLimitBar::defGroupName, TimeLimitBar::defName);
					bar->Kill();
				}
				break;
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//ステージ説明を描画
	EasingLogo::Object::SP Object::ShowRule()
	{
		//シングルトン
		if (ge->GetTask<EasingLogo::Object>(EasingLogo::defGroupName, EasingLogo::defName) == nullptr) {
			return EasingLogo::Object::Spawn(false);
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//Finishを描画
	EasingLogo::Object::SP  Object::ShowFinish()
	{
		//シングルトン
		if (ge->GetTask<EasingLogo::Object>(EasingLogo::defGroupName, EasingLogo::defName) == nullptr) {
			return EasingLogo::Object::Spawn(true);
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//タイムリミットのゲージを描画
	TimeLimitBar::Object::SP  Object::CreateTimeLimitBar(const ML::Vec2& pos, const int& time)
	{
		//シングルトン
		if (ge->GetTask<TimeLimitBar::Object>(TimeLimitBar::defGroupName, TimeLimitBar::defName) == nullptr) {
			return TimeLimitBar::Object::Create(pos, time);
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//「Fight」描画
	FightLogo::Object::SP  Object::CreateFightLogo()
	{
		//シングルトン
		if (ge->GetTask<FightLogo::Object>(FightLogo::defGroupName, FightLogo::defName) == nullptr) {
			return FightLogo::Object::Create(true);
		}
		return nullptr;
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