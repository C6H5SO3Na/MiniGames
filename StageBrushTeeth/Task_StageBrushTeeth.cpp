//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_StageBrushTeeth.h"
#include  "Task_brush.h"
#include  "Task_StainManager.h"
#include  "Task_CommonItemManager02.h"
#include  "../Task_Game.h"
#include  "../sound.h"

namespace  StageBrushTeeth
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->bgImg = DG::Image::Create("./data/image/mirror.png");
		this->teethImg = DG::Image::Create("./data/image/mouth.png");
		this->controllerMark = DG::Image::Create("./data/image/LeftStickAllDirection.png");
		ge->debugRectLoad();
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
		//BGM
		bgm::LoadFile("stage2_bgm", "./data/sound/bgm/Stage2_Short60_yuttariDIY_02.mp3");
		bgm::Play("stage2_bgm");


		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->render2D_Priority[1] = 0.9f;
		this->state = Phase::Game;
		this->timeCnt = 0;
		this->clearCount = 0;
		this->animCnt = 0;

		//★タスクの生成
		/*auto brush = brush::Object::Create(true);*/
		/*auto stainmanager = StainManager::Object::Create(true);*/
		auto commonmanager = CommonItemManager02::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("ブラシュ");
		ge->KillAll_G("よごれマネージャー");
		ge->KillAll_G("よごれ");
		ge->KillAll_G("共通アイテムマネージャー02");
		ge->KillAll_G("ステージ歯磨き");

		bgm::Stop("stage2_bgm");
		se::Stop("Kirakira");
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			Game::Object::CreateTask(2);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		timeCnt++;
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
		switch (this->state) {
		case Phase::Game:
			CheckClear();
			if (timeCnt >= 20 * 60)
			{
				this->state = Phase::Clear;
			}

			break;

		case Phase::Clear:
			Clear();
			break;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D src(0, 0, 3300, 2550);
		this->res->bgImg->Draw(draw, src);


		ML::Box2D draw_2(1920 / 2, 0, 1920 / 2, 1080 / 2);
		this->res->bgImg->Draw(draw_2, src);

		ML::Box2D draw_3(0, 1080 / 2, 1920 / 2, 1080 / 2);
		this->res->bgImg->Draw(draw_3, src);

		ML::Box2D draw_4(1920 / 2, 1080 / 2, 1920 / 2, 1080 / 2);
		this->res->bgImg->Draw(draw_4, src);

		ML::Box2D draw2(1920 / 10, 10, 1280/2, 1080/2 - 10 * 2);
		ML::Box2D src2(0, 0, 3300, 2550);
		this->res->teethImg->Draw(draw2, src2);

		ML::Box2D draw3(1920 / 2 + 1920 / 10, 10, 1280/2, 1080/2 - 10 * 2);
		this->res->teethImg->Draw(draw3, src2);

		ML::Box2D draw4(1920 / 10, 1080/2 + 10, 1280/2, 1080/2 - 10 * 2);
		this->res->teethImg->Draw(draw4, src2);

		ML::Box2D draw5(1920 / 2 + 1920 / 10, 1080/2 + 10, 1280/2, 1080/2 - 10 * 2);
		this->res->teethImg->Draw(draw5, src2);
		ge->debugRectDraw();

		ML::Box2D Draw(1920 / 2 - 150 / 2, 1080 / 2 - 150 / 2, 150, 150);
		int srcX = animIndex % 2 * 128;
		int srcY = animIndex / 2 * 128;
		ML::Box2D Src(srcX, srcY, 128, 128);
		this->res->controllerMark->Draw(Draw, Src);

	}
	//-------------------------------------------------------------------
	//ゲーム本編の処理
	void  Object::CheckClear()
	{
		auto stainManager = ge->GetTasks<StainManager::Object>("よごれマネージャー");
		for (auto it = stainManager->begin(); it != stainManager->end(); it++)
		{
			if ((*it)->IsClear()) {
				clearCount++;
			}
		}
		if (clearCount == 4) {// if all players were clear
			state = Phase::Clear;
		}
		else
		{
			clearCount = 0;// if not all players were clear this frame, reset clearcount
		}
	}
	//-------------------------------------------------------------------
	//全員クリア後の処理
	void  Object::Clear()
	{
		Kill();
		/*if (ge->getCounterFlag("Clear") == ge->LIMIT) {
			Kill();
		}*/
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