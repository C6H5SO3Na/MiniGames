//-------------------------------------------------------------------
//残り時間のバー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TimeLimitBar.h"
#include  "Task_UIManager.h"
#include  "../Task_Game.h"

namespace TimeLimitBar
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/bar.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		img.reset();
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
		render2D_Priority[1] = 0.01f;
		srcBase = ML::Box2D(0, 0, 96, 32);

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
		switch (ge->gameState) {
		case MyPG::MyGameEngine::GameState::Start:
			gaugeAmount = static_cast<float>(remainingCnt) / maxCnt;
			break;
		case MyPG::MyGameEngine::GameState::Game:
			--remainingCnt;
			if (remainingCnt <= 0) {
				ge->hasAllClearedGame = true;
			}
			gaugeAmount = static_cast<float>(remainingCnt) / maxCnt;
			break;

		case MyPG::MyGameEngine::GameState::Finish:
			break;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		DrawFlame();
		DrawGauge();
	}
	//-------------------------------------------------------------------
	//バーの枠描画
	void Object::DrawFlame() const
	{
		ML::Box2D src(0, 0, srcBase.w, srcBase.h);
		ML::Box2D draw(-srcBase.w * 6 / 2, -srcBase.h * 2 / 2, srcBase.w * 6, srcBase.h * 2);
		draw.Offset(pos);
		res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//バーのゲージ描画
	void Object::DrawGauge() const
	{
		int gSize = static_cast<int>(srcBase.w * gaugeAmount);
		ML::Box2D src(0, srcBase.h, gSize, srcBase.h);
		ML::Box2D draw(-srcBase.w * 6 / 2, -src.h * 2 / 2, gSize * 6, srcBase.h * 2);
		draw.Offset(pos);
		res->img->Draw(draw, src);
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
	Object::Object() :gaugeAmount(0.f), maxCnt(0), minPower(0), remainingCnt(0) {}
	//-------------------------------------------------------------------
	Object::SP Object::Create(const ML::Vec2& pos, const int& time)
	{
		auto gauge = Create(true);
		gauge->pos = pos;
		gauge->remainingCnt = time;
		gauge->maxCnt = gauge->remainingCnt;
		return gauge;
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