//-------------------------------------------------------------------
//バランスゲーム
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ClassifyGamePM.h"
#include  "Task_ClassifyGamePlayer.h"
#include  "CGBChara.h"

namespace  ClassifyGamePM
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
		this->res = Resource::Create();

		//★データ初期化
		gameCnt = 0;
		cList.push_back(ge->in1);
		cList.push_back(ge->in2);
		cList.push_back(ge->in3);
		cList.push_back(ge->in4);
		//★タスクの生成

		for (int i = 0; i < 4; ++i) {
			auto p = CGPlayer::Object::Create(true);
			pList.push_back(p);
			pList[i]->posInitialize(ML::Vec2(450 + 960 * (i % 2), 400 + 540 * (i / 2)));
			pList[i]->controller = cList[i];
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("CGPlayer");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		gameCnt++;
		if (gameCnt == 1320) {
			int winner = 0;
			for (int i = 0; i < 3; ++i) {
					if(pList[i+1]->Fb> pList[i]->Fb){
						winner = i + 1;
				}
			}
			this->Kill();
		}
		else if (gameCnt % 120 == 0) {
			int r = rand() % 3;
			switch (r)
			{
			case 0:
				for (int i = 0; i < 4; ++i) {
					if (pList[i]->GetCGState() != CGBChara::CGstate::Fail) {
						pList[i]->SetCGState(CGBChara::CGstate::PlayR);
					}
				}
				break;
			case 1:
				for (int i = 0; i < 4; ++i) {
					if (pList[i]->GetCGState() != CGBChara::CGstate::Fail) {
						pList[i]->SetCGState(CGBChara::CGstate::PlayG);
					}
				}
				break;
			case 2:
				for (int i = 0; i < 4; ++i) {
					if (pList[i]->GetCGState() != CGBChara::CGstate::Fail) {
						pList[i]->SetCGState(CGBChara::CGstate::PlayB);
					}
				}
				break;
			}
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