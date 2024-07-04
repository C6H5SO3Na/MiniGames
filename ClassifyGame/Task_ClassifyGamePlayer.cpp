//-------------------------------------------------------------------
//バランスゲームのプレイヤー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ClassifyGamePlayer.h"

namespace  CGPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		playerImg = DG::Image::Create("./data/image/chara02.png");
		for (int i = 0; i < 10; ++i) {
			bookImg[i] = DG::Image::Create("./data/image/debugrect.png");
		}
	
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		playerImg.reset();
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
		workTime = 0;
		SetCGState(CGstate::BStart);
		for (int i = 0; i < 10; ++i) {
			books[i].bpos = pos + ML::Vec2(0, -40);
		}
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
		auto in = controller->GetState();
		switch (GetCGState())
		{
		case CGstate::Playing:
			break;
		case CGstate::PlayR:
			if (FirstIntoState()) {
				books[workTime].color = 0;
				workTime++;
			}
			if (in.LStick.BL.down) {
				moveCnt = 0;
				books[workTime].bpos += ML::Vec2(-30, -100);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt = 119) {
				toAnotherState(workTime);
			}
			break;
		case CGstate::PlayG:
			if (FirstIntoState()) {
				books[workTime].color = 2;
				workTime++;
			}
			if (in.LStick.BU.down) {
				moveCnt = 0;
				books[workTime].bpos += ML::Vec2(0, -100);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt = 119) {
				toAnotherState(workTime);
			}
			break;
		case CGstate::PlayB:
			if (FirstIntoState()) {
				books[workTime].color = 1;
				workTime++;
			}
			if (in.LStick.BR.down) {
				moveCnt = 0;
				books[workTime].bpos += ML::Vec2(30, 100);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt = 119) {
				toAnotherState(workTime);
			}
			break;
		case CGstate::Fail:
			
			break;
		}
		
		
	}
	void  Object::toAnotherState(int workT) {
		books[workT].color = 3;
		moveCnt = 0;
		books[workT].bpos += ML::Vec2(0, -200);
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D src(0, 0, 32, 80);
		ML::Box2D draw(0, 0, 32, 80);
		
		res->playerImg->Draw(draw.OffsetCopy(pos), src);
		src = ML::Box2D(0, 0, 32, 32);
		draw = ML::Box2D(0, 0, 32, 32);
		for (int i = 0; i < 10; ++i) {
			if (workTime > i) {
				src.x = books[i].color * 32;
				ML::Box2D draw0 = draw.OffsetCopy(books[i].bpos);
				res->bookImg[i]->Draw(draw0, src);
			}
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