//-------------------------------------------------------------------
//ゲームの最初に出る指示
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_EasingLogo.h"
#include  "Task_GameMessage.h"

namespace EasingLogo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		img.push_back(DG::Image::Create("./data/image/SaboriGameRuleSentence.png"));
		img.push_back(DG::Image::Create("./data/image/bar.png"));
		img.push_back(DG::Image::Create("./data/image/bar.png"));
		img.push_back(DG::Image::Create("./data/image/bar.png"));
		img.push_back(DG::Image::Create("./data/image/SaboriGameRuleSentence.png"));
		img.push_back(DG::Image::Create("./data/image/OugiGameRuleSentence.png"));
		img.push_back(DG::Image::Create("./data/image/bar.png"));
		img.push_back(DG::Image::Create("./data/image/Finish.png"));
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		for (auto& i : img) {
			i.reset();
		}
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
		render2D_Priority[1] = -0.99f;

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
		if (!isCreated) {
			if (isFinish) {
				logo = GameMessage::Object::Create(res->img[7], srcTable[7], "FinishSE");
			}
			else {
				logo = GameMessage::Object::Create(res->img[ge->nowStage], srcTable[ge->nowStage]);
			}
			isCreated = true;
		}
		if (logo.lock() == nullptr) {
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
	Object::Object():isFinish(false) {}
	//-------------------------------------------------------------------
	Object::SP Object::Spawn(const bool& isFinish)
	{
		auto logo = Create(true);
		logo->isFinish = isFinish;
		return logo;
	}
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