//-------------------------------------------------------------------
//バランスゲーム
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ClassifyGame.h"
#include  "Task_ClassifyGamePM.h"

#include  "../Task_Game.h"
#include  "../sound.h"
//#include  "Task_ClassifyGUIManager.h"

namespace  ClassifyGame
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		controllerMark = DG::Image::Create("./data/image/clasLeftStickAllDirection.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		controllerMark.reset();
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

		gameCnt = 0;
		ge->nowTimeLimit = 1320;
		animCnt = 0;
		//BGM
		bgm::LoadFile("bgmCG", "./data/sound/bgm/tanoshiimugibatake.mp3");
		bgm::Play("bgmCG");
		//★タスクの生成
		auto cgpm = ClassifyGamePM::Object::Create(true); //プレイヤマネージャーを生成
		//auto bguim = ClassifyGUIM::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		bgm::Stop("bgmCG");
		//★データ＆タスク解放
		ge->KillAll_G("ClassifyGamePM");
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			ge->GetScore(0);
			Game::Object::CreateTask(4);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		switch (ge->gameState)
		{
		case MyPG::MyGameEngine::GameState::Start:
			break;
		case MyPG::MyGameEngine::GameState::Game:
			gameCnt++;
			if (gameCnt == 1319) {
				ge->hasAllClearedGame = true;
			}
			animCnt++;
			//アニメ更新
			if (this->animCnt >= 15)
			{
				this->animCnt = 0;
				this->animIndex++;
				if (this->animIndex >= 2)
				{
					this->animIndex = 0;
				}
			}
			break;
		case MyPG::MyGameEngine::GameState::Finish:
			if (ge->hasFinishedEasing) {
				this->Kill();
			}
			break;
		}
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if (ge->gameState == MyPG::MyGameEngine::GameState::Game)
		{
			//コントローラーマーク
			ML::Box2D Draw(1920 / 2 - 150 / 2, 1080 / 2 - 150 / 2, 150, 150);
			int srcX = animIndex % 2 * 128;
			int srcY = animIndex / 2 * 128;
			ML::Box2D Src(srcX, srcY, 128, 128);
			if (ge->players.size() < 4) {
				Draw.x -= 100;
				Draw.y += ge->players.size() / 2 * 300 - 200;
			}
			this->res->controllerMark->Draw(Draw, Src);
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