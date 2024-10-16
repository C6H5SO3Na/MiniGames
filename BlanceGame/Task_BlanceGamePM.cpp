//-------------------------------------------------------------------
//バランスゲームプレイヤマネージャー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_BlanceGame.h"
#include  "Task_BlanceGamePM.h"
#include  "Task_BlanceGamePlayer.h"
#include  "BGBChara.h"
#include  "../sound.h"
namespace  BlanceGamePM
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
		for (auto i = 0; i < ge->players.size(); ++i) {
			/*cList.push_back(ge->players[i]);*/
			auto p = BGPlayer::Object::Create(true);
			pList.push_back(p);
			pList[i]->pos = ML::Vec2(400 + 400 * i, 500);
			pList[i]->controller = ge->players[i];
			pList[i]->playerNum = i;
		}
		//cList.push_back(ge->in1);
		//cList.push_back(ge->in2);
		//cList.push_back(ge->in3);
		//cList.push_back(ge->in4);
		//se
		se::LoadFile("seBGd", "./data/sound/se/BlanceGame/Onoma-Surprise02-1(High).wav");
		se::LoadFile("seBGf", "./data/sound/se/BlanceGame/Motion-Fall06-3(Long).wav");
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
		auto bg = ge->GetTask<BlanceGame::Object>("BlanceGame");
		if (!bg) {
			return;
		}
		if (bg->shake) {
			se::Play("seBGd");
			auto p = ge->GetTasks<BGPlayer::Object>("BGPlayer");
			int r = rand() % 2;
			switch (r)
			{
			case 0:
				for (int i = 0; i < pList.size(); ++i) {
					if (pList[i]->GetBGState() == BGBChara::BGstate::Playing) {
						pList[i]->SetBGState(BGBChara::BGstate::PlayL);
					}
				}
				break;
			case 1:
				for (int i = 0; i < pList.size(); ++i) {
					if (pList[i]->GetBGState() == BGBChara::BGstate::Playing) {
						pList[i]->SetBGState(BGBChara::BGstate::PlayR);
					}
				}
				break;
			default:
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