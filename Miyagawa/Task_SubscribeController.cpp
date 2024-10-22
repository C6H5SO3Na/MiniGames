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
		//「~P」
		playerNumLogo = DG::Image::Create("./data/image/PlayerNumber.png");

		//キャラクター
		playerImg[0] = DG::Image::Create("./data/image/game_otsan_pose_red.png");
		playerImg[1] = DG::Image::Create("./data/image/game_otsan_pose.png");
		playerImg[2] = DG::Image::Create("./data/image/game_otsan_pose_yellow.png");
		playerImg[3] = DG::Image::Create("./data/image/game_otsan_pose_green.png");

		//Startボタン
		startButton[0] = DG::Image::Create("./data/image/button/Double/xbox_button_start_icon.png");
		startButton[1] = DG::Image::Create("./data/image/button/Double/xbox_button_start_icon_outline.png");

		//Aボタン
		AButton[0] = DG::Image::Create("./data/image/button/Double/xbox_button_color_a.png");
		AButton[1] = DG::Image::Create("./data/image/button/Double/xbox_button_color_a_outline.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		playerNumLogo.reset();
		playerImg->reset();
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
		TestFont = DG::Font::Create("ＭＳ ゴシック", 40, 80);
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

		//登録処理
		for (int i = 0; i < inputs.size(); ++i) {
			//次のタスクへ
			if (inputs[i]->GetState().ST.down && subscribeCnt > 0) {
				Kill();
			}

			//登録 
			if (inputs[i]->GetState().B1.down) {
				if (!isPushButton[i]) {
					Subscribe(inputs[i], isPushButton[i], i);
				}
				//イージング開始
				StartEasing(i);
			}
		}

		//イージング処理
		for (int i = 0; i < inputs.size(); ++i) {
			if (!isPushButton[i]) { continue; }
			if (easing::GetState("ReactionEnd" + to_string(i)) == easing::EQ_STATE::EQ_END) {
				easingPos[i] = 0.f;
			}
			else if (easing::GetState("Reaction" + to_string(i)) == easing::EQ_STATE::EQ_END) {
				easingPos[i] = easing::GetPos("ReactionEnd" + to_string(i));
			}
			else
			{
				easingPos[i] = easing::GetPos("Reaction" + to_string(i));
			}
		}
		++animCnt;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//プレイヤー番号
		DrawPlayerNums();

		//プレイヤー
		DrawPlayers();

		//UI
		DrawUI();
	}
	//-------------------------------------------------------------------
	//プレイヤーの登録処理
	void  Object::Subscribe(XI::GamePad::SP controller, bool& isPush, const int& i)
	{
		isPush = true;
		ge->players.push_back(controller);
		controllerIndex[subscribeCnt] = i;
		++subscribeCnt;
	}
	//-------------------------------------------------------------------
	//イージング開始
	void  Object::StartEasing(const int& index) const
	{
		easing::Set("Reaction" + to_string(index), easing::QUADOUT, 0, 50, 5, "ReactionEnd" + to_string(index));
		easing::Set("ReactionEnd" + to_string(index), easing::QUADOUT, 50, 0, 5);
		easing::Start("Reaction" + to_string(index));
	}

	//-------------------------------------------------------------------
	//プレイヤー番号まとめ描画
	void  Object::DrawPlayerNums() const
	{
		//テーブル化
		const ML::Box2D srcTable[4] = {
			ML::Box2D(0, 0, 155, 105),ML::Box2D(155, 0, 192, 105), ML::Box2D(347, 0, 175, 105), ML::Box2D(522, 0, 193, 105)
		};

		//テーブル化
		const ML::Box2D drawTable[4] = {
			ML::Box2D(-78, -53, 155, 105),ML::Box2D(-96, -53, 192, 105),ML::Box2D(-88, -53, 175, 105), ML::Box2D(-97, -53, 193, 105)
		};

		//プレイヤー分描画
		for (int i = 0; i < ge->players.size(); ++i) {
			ML::Box2D draw = drawTable[i].OffsetCopy(320 + 400 * i, static_cast<int>(200 + easingPos[controllerIndex[i]]));
			res->playerNumLogo->Draw(draw, srcTable[i]);
		}
	}
	//-------------------------------------------------------------------
	//プレイヤーまとめ描画
	void  Object::DrawPlayers() const
	{
		//プレイヤー分描画
		for (int i = 0; i < ge->players.size(); ++i) {
			ML::Box2D draw(100 + 400 * i, 300, srcPlayer.w, srcPlayer.h);
			res->playerImg[i]->Draw(draw, srcPlayer);
		}
	}
	//-------------------------------------------------------------------
	//UI描画
	void  Object::DrawUI() const
	{
		ML::Box2D textBox(550, 50, 1000, 100);
		string text = "プレイヤーエントリー";
		TestFont->Draw(textBox, text);

		DrawOperation();
	}
	//-------------------------------------------------------------------
	//UI描画
	void  Object::DrawOperation() const
	{
		//ゲーム開始
		if (subscribeCnt > 0) {
			ML::Box2D textBox(800, 850, 1000, 100);
			string text = "ゲーム開始";
			TestFont->Draw(textBox, text);
		}

		//ゲーム開始のボタン
		if (subscribeCnt > 0) {
			DrawButton(res->startButton[animCnt / 7 % 2], ML::Vec2(650.f, 820.f));
		}

		//エントリー
		{
			ML::Box2D textBox(800, 960, 1000, 100);
			string text = "エントリー";
			TestFont->Draw(textBox, text);
		}

		//エントリーのボタン
		DrawButton(res->AButton[animCnt / 7 % 2], ML::Vec2(650.f, 930.f));
	}
	//-------------------------------------------------------------------
	//ボタン描画
	void  Object::DrawButton(DG::Image::SP img, const ML::Vec2& pos) const
	{
		ML::Box2D src(0, 0, 128, 128);
		ML::Box2D draw = src.OffsetCopy(pos);
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