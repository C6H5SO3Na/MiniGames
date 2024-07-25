//-------------------------------------------------------------------
//バランスゲームのプレイヤー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ClassifyGamePlayer.h"
#include  "../Task_Game.h"

namespace  CGPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		playerImg = DG::Image::Create("./data/image/chara02.png");
		for (int i = 0; i < 10; ++i) {
			bookImg = DG::Image::Create("./data/image/debugrect.png");
		}
		CountImg = DG::Image::Create("./data/image/font_number.png");
	
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
		workTime = 4;
		SetCGState(CGstate::BStart);
		Rb = 0;
		Gb = 0;
		Bb = 0;
		Fb = 0;
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「初期化」書類の座標
	void Object::posInitialize(ML::Vec2 ppos)
	{
		pos = ppos;
		for (int i = 0; i < 14; ++i) {
			books[i].bpos = pos + ML::Vec2(0, -40);
		}
		for (int i = 0; i < 3; ++i) {
			books[i].bpos += ML::Vec2(-80 + i * 80, -100);
			books[i].color = i;
		}
		books[3].bpos += ML::Vec2(0, -200);
		books[3].color = 3;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			Game::Object::Create(true);
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
				moveCnt = 0;
				workTime++;
			}
			if (in.LStick.BL.down) {
				moveCnt = 0;
				Rb++;
				books[workTime - 1].bpos += ML::Vec2(-80, -100 - Rb * 5);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt == 118 || in.LStick.BU.down || in.LStick.BR.down) {//時間切れ、或は間違いボタンを押した
				toFailState(workTime);
				SetCGState(CGstate::Fail);
			}
			break;
		case CGstate::PlayB:
			if (FirstIntoState()) {
				books[workTime].color = 1;
				moveCnt = 0;
				workTime++;
			}
			if (in.LStick.BU.down) {
				moveCnt = 0;
				Bb++;
				books[workTime - 1].bpos += ML::Vec2(0, -100 - Bb * 5);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt == 118 || in.LStick.BL.down || in.LStick.BR.down) {//時間切れ、或は間違いボタンを押した
				toFailState(workTime);
				SetCGState(CGstate::Fail);
			}
			break;
		case CGstate::PlayG:
			if (FirstIntoState()) {
				books[workTime].color = 2;
				moveCnt = 0;
				workTime++;
			}
			if (in.LStick.BR.down) {
				moveCnt = 0;
				Gb++;			
				books[workTime - 1].bpos += ML::Vec2(80, -100 - Gb * 5);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt == 118 ||in.LStick.BL.down || in.LStick.BU.down) {//時間切れ、或は間違いボタンを押した
				toFailState(workTime);
				SetCGState(CGstate::Fail);
			}
			break;
		case CGstate::Fail:	
			Fb++;
			SetCGState(CGstate::Playing);
			break;
		}
		
		
	}
	void  Object::toFailState(int workT) {
		books[workT - 1].color = 3;
		moveCnt = 0;
		books[workT - 1].bpos += ML::Vec2(0, -200 - Fb * 5);
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
		for (int i = 0; i < 14; ++i) {
			if (workTime > i) {
				src.x = books[i].color * 32;
				ML::Box2D draw0 = draw.OffsetCopy(books[i].bpos);
				res->bookImg->Draw(draw0, src);
			}
		}
		src = ML::Box2D(0, 35, 20, 25);
		draw = ML::Box2D(0, 0, 20, 25);
		res->CountImg->Draw(draw.OffsetCopy(pos+ML::Vec2(-80, -100)), src.OffsetCopy(20 * Rb, 0));
		res->CountImg->Draw(draw.OffsetCopy(pos + ML::Vec2(0, -100)), src.OffsetCopy(20 * Bb, 0));
		res->CountImg->Draw(draw.OffsetCopy(pos + ML::Vec2(80, -100)), src.OffsetCopy(20 * Gb, 0));
		res->CountImg->Draw(draw.OffsetCopy(pos + ML::Vec2(0, -200)), src.OffsetCopy(20 * Fb, 0));
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