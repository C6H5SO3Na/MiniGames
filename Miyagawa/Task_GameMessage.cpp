//-------------------------------------------------------------------
//ゲームの最初に出る指示
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_GameMessage.h"
#include  "../sound.h"
#include  "../easing.h"

namespace GameMessage
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
		res = Resource::Create();

		//★データ初期化
		render2D_Priority[1] = 0.01f;

		pos = ML::Vec2(3000, ge->screen2DHeight / 2.f);

		easing::Set("Start", easing::CIRCOUT, static_cast<float>(ge->screen2DWidth + src.w), ge->screen2DWidth / 2.f, 60, "End");
		easing::Set("End", easing::CIRCIN, ge->screen2DWidth / 2.f, static_cast<float>(-src.w), 60);

		//☆イージング開始
		easing::Start("Start");

		se::LoadFile("default", "./data/sound/se/Common/試合開始のゴング.wav");

		se::LoadFile("FinishSE", "./data/sound/se/Common/試合終了のゴング.wav");
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
			img.reset();
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (!isPlayedSE) {
			se::Play(SEName);
			isPlayedSE = true;
		}

		//☆イージングで座標移動
		//Readyを動かす
		pos.x = easing::GetPos("Start");
		if (easing::GetState("Start") == easing::EQ_STATE::EQ_END) //イージング「GameRuleStart」が終わったら
		{
			pos.x = easing::GetPos("End");
		}
		//イージングが完全終了したらタスクを消去
		if (easing::GetState("End") == easing::EQ_STATE::EQ_END) //イージング「GameRuleEnd」が終わったら
		{
			Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw;

		draw = ML::Box2D(-src.w, -src.h, src.w * 2, src.h * 2);
		draw.Offset(pos);

		img->Draw(draw, src);
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
	Object::Object() {}
	//-------------------------------------------------------------------
	Object::SP Object::Create(DG::Image::SP img, const ML::Box2D& src, const string& SEName)
	{
		auto logo = Create(true);
		logo->img = img;
		logo->src = src;
		logo->SEName = SEName;
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