//-------------------------------------------------------------------
//コントローラー登録画面
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SubscribeController.h"
#include  "Task_SubscribeControllerBG.h"
#include  "../Task_Game.h"
#include  "../sound.h"
#include  "../easing.h"

namespace SubscribeController
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
		render2D_Priority[1] = 0.f;
		TestFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);
		ge->players.clear();

		//コンテナに追加
		inputs.push_back(ge->in1);
		inputs.push_back(ge->in2);
		inputs.push_back(ge->in3);
		inputs.push_back(ge->in4);

		//★タスクの生成
		//背景
		//横スクロールを作る
		for (int i = 0; i < 3; ++i)
		{
			auto BG = SubscribeControllerBG::Object::Create(true);
			if (BG)
			{
				BG->pos.x = static_cast<float>(i * 1600);
			}
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{

		ge->KillAll_G("コントローラー登録画面");
		//★データ＆タスク解放
		if (!ge->QuitFlag() && nextTaskCreate) {
			//★引き継ぎタスクの生成
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		easing::UpDate();//必須
		for (int i = 0; i < inputs.size(); ++i) {
			//次のタスクへ
			if (inputs[i]->GetState().ST.down && subscribeCnt > 0) {
				Kill();
			}

			//登録 
			if (inputs[i]->GetState().B1.down) {
				easing::Set("Reaction" + to_string(controllerIndex[i]), easing::QUADOUT, 0, 50, 5, "ReactionEnd" + to_string(controllerIndex[i]));
				easing::Set("ReactionEnd" + to_string(controllerIndex[i]), easing::QUADOUT, 50, 0, 5);
				easing::Start("Reaction" + to_string(controllerIndex[i]));
				if (!isPushButton[i]) {
					Subscribe(inputs[i], isPushButton[i], i);
				}
			}
		}

		for (int i = 0; i < ge->players.size(); ++i) {
			if (easing::GetState("ReactionEnd" + to_string(controllerIndex[i])) == easing::EQ_STATE::EQ_END) {
				easingPos[controllerIndex[i]] = 0.f;
			}
			else if (easing::GetState("Reaction" + to_string(controllerIndex[i])) == easing::EQ_STATE::EQ_END) {
				easingPos[controllerIndex[i]] = easing::GetPos("ReactionEnd" + to_string(controllerIndex[i]));
			}
			else
			{
				easingPos[controllerIndex[i]] = easing::GetPos("Reaction" + to_string(controllerIndex[i]));
			}
		}

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		for (int i = 0; i < ge->players.size(); ++i) {
			if (!isPushButton[controllerIndex[i]]) { continue; }
			ML::Box2D textBox(100 * i, 100 + easingPos[i], 100, 100);
			string text = to_string(i + 1) + "P";
			TestFont->Draw(textBox, text);
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Subscribe(XI::GamePad::SP controller, bool& isPush, const int& i)
	{
		isPush = true;
		ge->players.push_back(controller);
		controllerIndex[subscribeCnt] = i;
		++subscribeCnt;
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