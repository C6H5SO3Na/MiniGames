//-------------------------------------------------------------------
//大食いミニゲームのUI管理
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiUIManager.h"
#include  "Task_OguiGame.h"
#include  "Task_OguiPlayer.h"

namespace  OguiUIManager
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
		testFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);
		
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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//☆食べた料理数の描画
		//プレイヤー全てを抽出する
		auto players = ge->GetTasks<OguiPlayer::Object>("プレイヤー");
		//プレイヤーの数だけループを回す
		int loopCount = 0; //ループした回数のカウント
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			//描画
			testFont->Draw(ML::Box2D(45 + ge->screen2DWidth * loopCount / 4, 65, ge->screen2DWidth, ge->screen2DHeight),
				to_string((int)(*p)->playerNum) + "P:" + to_string((*p)->eatFoodCount)
			);
			//ループ回数のカウント
			++loopCount;
		}

		//☆制限時間の描画
		//サボりゲームの統括の情報を取得
		auto game = ge->GetTask<OguiGame::Object>("本編");
		//描画
		testFont->Draw(ML::Box2D(ge->screen2DWidth / 2, 0, ge->screen2DWidth, ge->screen2DHeight),
			to_string(game->timeLimit)
		);
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