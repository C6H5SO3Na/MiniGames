//-------------------------------------------------------------------
//バランスゲームのプレイヤー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_BlanceGamePlayer.h"
#include  "../sound.h"

namespace  BGPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		playerImg = DG::Image::Create("./data/image/game_otsan_train.png");
		playerNumImg = DG::Image::Create("./data/image/PlayerNumber.png");
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
		direction = 0;
		SetBGState(BGstate::BStart);
	
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
		switch (GetBGState())
		{
		case BGstate::BStart:
			SetBGState(BGstate::Playing);
			break;
		case BGstate::Playing:
			break;
		case BGstate::PlayR:
			direction += 1;
			if (in.LStick.BL.down) { SetBGState(BGstate::returnL); }
			if (direction > 45) { SetBGState(BGstate::Fail); }		
			break;
		case BGstate::PlayL:
			direction -= 1;
			if (in.LStick.BR.down) { SetBGState(BGstate::returnR); }
			if (direction < -45) { SetBGState(BGstate::Fail); }			
			break;
		case BGstate::returnL:
			direction -= 1;
			if (direction == 0) { SetBGState(BGstate::Playing); }
			break;
		case BGstate::returnR:
			direction += 1;
			if (direction == 0) { SetBGState(BGstate::Playing); }
			break;
		case BGstate::Fail:
			break;
		case BGstate::GameOver:
			pos.y = 600;
			direction = 90;
			break;
		}
		
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D src(0, 0, 446, 542);
		ML::Box2D draw(-100, 0, 360, 432);
		switch (GetBGState())
		{
		case BGstate::Playing:
			break;
		case BGstate::PlayR:
			src = ML::Box2D(2236, 543, 446, 542);
			break;
		case BGstate::PlayL:
			src = ML::Box2D(2236, 1086, 446, 542);
			break;
		case BGstate::returnR:
			src = ML::Box2D(2236, 543, 446, 542);
			break;
		case BGstate::returnL:
			src = ML::Box2D(2236, 1086, 446, 542);
			break;
		case BGstate::Fail:
			
			break;
		}
		res->playerImg->Rotation(ML::ToRadian(direction * 2), ML::Vec2(180, 432));
		res->playerImg->Draw(draw.OffsetCopy(pos), src);
		//playerNum
		src = playerNumUIsrc[playerNum];
		draw = playerNumUIdraw[playerNum];
		res->playerNumImg->Draw(draw.OffsetCopy(pos + ML::Vec2(0, -250)), src);

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