//-------------------------------------------------------------------
//リザルト表示用プレイヤー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ResultUIPlayer.h"

namespace  ResultUIPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->playerImage = DG::Image::Create("./data/image/Ohara/testImage/testSquare.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->playerImage.reset();
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
		//☆表示用プレイヤーの描画
		DrawInformation drawPlayerImage = this->GetDrawImages(0);
		drawPlayerImage.draw.Offset(this->drawPos.displayPlayerPos);

		this->res->playerImage->Draw(drawPlayerImage.draw, drawPlayerImage.src);

		//☆プレイヤー番号の描画

		//☆順位画像の描画

		//☆スコアの描画

	}
	//-------------------------------------------------------------------
	//アニメーション制御　imageNumが0 = プレイヤー、1 = プレイヤーの番号、2 = 順位　の画像制御
	Object::DrawInformation Object::GetDrawImages(int imageNum)
	{
		DrawInformation imageTable[] = {
			//プレイヤー
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(200, 0, 100, 100) },	//要素数0 1位プレイヤー
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(100, 0, 100, 100) },	//要素数1 2位プレイヤー
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(300, 0, 100, 100) },	//要素数2 3位プレイヤー
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(0, 0, 100, 100)},	//要素数3 4位プレイヤー

			//プレイヤーの番号

			//順位

			//スコア
		};
		DrawInformation rtv = {};

		//どの描画情報を取得するか決める
		switch (imageNum)
		{
		case 0: //プレイヤー
			//順位でどの画像を表示するか決める
			switch (this->drawPlayerInfo.rank)
			{
			case 1:	//1位
				rtv = imageTable[0];
				break;

			case 2:	//2位
				rtv = imageTable[1];
				break;

			case 3:	//3位
				rtv = imageTable[2];
				break;

			case 4:	//4位
				rtv = imageTable[3];
				break;
			}

		case 1: //プレイヤー番号
			switch (this->drawPlayerInfo.PlayerNum)
			{
			case 1: //1P
				break;

			case 2: //2P
				break;

			case 3:	//3P
				break;

			case 4: //4P
				break;
			}
			break;

		case 2: //順位
			break;

		case 3: //スコア
			break;
		}

		return rtv;
	}
	//-------------------------------------------------------------------
	//描画するプレイヤーの情報を取得する
	void Object::setPlayerInformation(const Result::Object::PlayerInformation& playerInfo)
	{
		this->drawPlayerInfo.PlayerNum = playerInfo.PlayerNum;
		this->drawPlayerInfo.rank = playerInfo.rank;
		this->drawPlayerInfo.score = playerInfo.score;
	}
	//-------------------------------------------------------------------
	//描画位置を設定する
	void Object::setDrawPos(const ML::Vec2& playerPos, const ML::Vec2& playerNumPos, const ML::Vec2& rankPos, const ML::Vec2& scorePos)
	{
		this->drawPos.displayPlayerPos = playerPos;
		this->drawPos.playerNumPos = playerNumPos;
		this->drawPos.rankPos = rankPos;
		this->drawPos.scorePos = scorePos;
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
	Object::Object()
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
	Resource::~Resource() { this->Finalize(); }
}