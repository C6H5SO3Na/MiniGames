//-------------------------------------------------------------------
//バランスゲームＵＩマネージャー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_BlanceUIManager.h"
#include  "Task_BlanceGame.h"
#include  "Task_BlanceGamePM.h"
#include  "../sound.h"

namespace  BlanceGUIM
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		backImg = DG::Image::Create("./data/image/game_trainBG.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		backImg.reset();
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
		render2D_Priority[1] = 0.5;
		failtime = 0;
		pos = ML::Vec2{ 0,0 };
		SetBGState(BGstate::Start);
		for (int i = 0; i < 4; ++i) {
			failFlag[i] = false;
		}
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		for (int i = 0; i < 4; ++i) {
			if (failFlag[i] == false) {
				ge->AddScore(i, 4);
			}
		}
		int j = 0;
		for (auto i = rank.begin(); i != rank.end(); ++i) {
			ge->AddScore(*i, failPlayerNum[j]);
			if (failPlayerNum[j] == 3) {
				ge->AddScore(*i, 1);
			}
			j++;
			//ge->score[(*i)] += ((*i) + 1);
		}
		//★データ＆タスク解放
		ge->KillAll_G("BGPlayer");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto bg = ge->GetTask<BlanceGame::Object>("BlanceGame");
		if (!bg) {
			return;
		}
		if (bg->shake) {
			moveCnt++;
			if (moveCnt % 5 != 0) {
				pos.x += 3;
			}
			else
			{
				pos.x -= 12;
			}
		}
		
		auto pm = ge->GetTask<BlanceGamePM::Object>("blanceGamePM");
		int j = 0;
		for (int i = 0; i < 4; ++i) {
			if (pm->pList[i]->GetBGState() == BGstate::Fail&& failFlag[i] == false) {
				se::Play("seBGf");
				failPlayerNum.push_back(failtime);
				rank.push_back(i);
				failFlag[i] = true;
				j++;
				pm->pList[i]->SetBGState(BGstate::GameOver);
			}
		}
		failtime += j;
		if (failFlag[0]&& failFlag[1]&& failFlag[2]&& failFlag[3]) {	
			Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D src = ML::Box2D(0, 0, 1920, 1080);
		ML::Box2D draw = ML::Box2D(0, 0, 2112, 1188);
		draw = draw.OffsetCopy(pos);
		res->backImg->Draw(src, draw);
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