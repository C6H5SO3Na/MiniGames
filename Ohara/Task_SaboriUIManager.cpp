//-------------------------------------------------------------------
//サボりミニゲームのUI管理
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriUIManager.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriGame.h"

namespace  SaboriUIManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->playerNumberImage = DG::Image::Create("./data/image/PlayerNumber.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->playerNumberImage.reset();
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
		this->render2D_Priority[1] = 0.6f;
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
		//☆制限時間の描画
		//サボりゲームの統括の情報を取得
		auto game = ge->GetTask<SaboriGame::Object>(SaboriGame::defGroupName, SaboriGame::defName);
		//描画
		testFont->Draw(ML::Box2D(ge->screen2DWidth / 2 - 150, 0, ge->screen2DWidth, ge->screen2DHeight),
			to_string(game->timeLimit), ML::Color(1, 0, 0, 0)
		);

		//☆サボり合計時間の描画
		//プレイヤー全てを抽出する
		auto players = ge->GetTasks<SaboriPlayer::Object>("プレイヤー");
		//プレイヤーの数だけループを回す
		int loopCount = 0; //ループした回数のカウント
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			//描画
			testFont->Draw(ML::Box2D(45 + ge->screen2DWidth * loopCount / 4, 65, ge->screen2DWidth, ge->screen2DHeight),
				to_string((int)(*p)->playerNum) + "P:" + to_string((*p)->totalSaboriTime), ML::Color(1, 0, 0, 0)
			);
			//ループ回数のカウント
			++loopCount;
		}

		//☆プレイヤー番号の描画
		this->DrawPlayerNumber();
	}
	//-------------------------------------------------------------------
	//プレイヤー番号の描画
	void Object::DrawPlayerNumber()
	{
		for (int i = 0; i < sizeof(this->playerNumbersDrawInfo) / sizeof(this->playerNumbersDrawInfo[0]); ++i)
		{
			ML::Box2D playerNumberDraw = this->playerNumbersDrawInfo[i].draw;
			playerNumberDraw.Offset(this->playerNumbersDrawInfo[i].pos);
			ML::Box2D playerNumberSrc = this->playerNumbersDrawInfo[i].src;

			//描画
			this->res->playerNumberImage->Draw(playerNumberDraw, playerNumberSrc);
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