//-------------------------------------------------------------------
//プレイヤー
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGame.h"
#include  "Task_TaxiGamePlayer.h"
#include  "Task_TaxiGamePlayerIdleState.h"
#include "../randomLib.h"
#include "../easing.h"
#include "../sound.h"

namespace TaxiGamePlayer
{
	Resource::WP  Resource::instance;
	int Object::playerScore;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//クリア画像
		imageClear = DG::Image::Create("./data/image/clearImage.png");

		//ボタンの画像(アニメーション込みのため二次元配列)
		imageButton[0][0] = DG::Image::Create("./data/image/button/default/xbox_button_color_a.png");
		imageButton[1][0] = DG::Image::Create("./data/image/button/default/xbox_button_color_a_outline.png");
		imageButton[0][1] = DG::Image::Create("./data/image/button/default/xbox_button_color_b.png");
		imageButton[1][1] = DG::Image::Create("./data/image/button/default/xbox_button_color_b_outline.png");
		imageButton[0][2] = DG::Image::Create("./data/image/button/default/xbox_button_color_x.png");
		imageButton[1][2] = DG::Image::Create("./data/image/button/default/xbox_button_color_x_outline.png");
		imageButton[0][3] = DG::Image::Create("./data/image/button/default/xbox_button_color_y.png");
		imageButton[1][3] = DG::Image::Create("./data/image/button/default/xbox_button_color_y_outline.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		imageClear.reset();
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
		render2D_Priority[1] = 0.5f;
		nowButton = GetRandom(0, 3);

		//SE
		se::LoadFile("Miss", "./data/sound/se/ClassifyGame/maou_se_onepoint33.wav");
		se::LoadFile("Clear", "./data/sound/se/TaxiGame/clear.wav");
		se::LoadFile("Walk", "./data/sound/se/TaxiGame/footstep.wav");

		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		imagePlayer.reset();

		if (!ge->QuitFlag() && nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		input = controller->GetState();
		Think();
		Move();
		++animationCount;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		state->render(*this);
	}
	//-------------------------------------------------------------------
	//思考
	void  Object::Think()
	{
		state->think(*this);
	}
	//-------------------------------------------------------------------
	//行動
	void  Object::Move()
	{
		state->move(*this);
	}
	//-------------------------------------------------------------------
	//受け身
	void  Object::Received()
	{
	}
	//-------------------------------------------------------------------
	//ボタンが一致したときの処理
	void  Object::MatchedButton()
	{
		//当たり判定ではなく回数で判定
		++matchCount;
		if (matchCount >= 10) {
			isClear = true;
			return;
		}
		nowButton = GetRandom(0, 3);
	}
	//-------------------------------------------------------------------
	//ボタンの描画
	void  Object::DrawButton() const
	{
		//描画矩形
		ML::Box2D src(0, 0, 64, 64);
		ML::Box2D draw(-src.w / 2, -src.h / 2, src.w, src.h);
		draw.Offset(pos + ML::Vec2(-30.f, 60.f));
		res->imageButton[animationCount / 10 % 2][nowButton]->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//クリアメッセージ描画
	void  Object::DrawClearMessage() const
	{
		ML::Box2D src(0, 0, 97, 25);
		ML::Box2D draw(-src.w / 2, -src.h / 2, src.w, src.h);
		draw.Offset(pos);
		res->imageClear->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//段階を変更
	void  Object::ChangeState(StateBase* state_)
	{
		delete state;
		state = state_;
		moveCnt = 0;
		prePosition = pos;
	}
	//-------------------------------------------------------------------
	//ボタン(入力をビットで入手)
	int  Object::GetButtonNum(int state)
	{
		//それぞれのボタン判定
		struct Input {
			bool A = false;
			bool B = false;
			bool X = false;
			bool Y = false;
		};
		Input inp[4] = {
			{input.B1.down, input.B2.down, input.B3.down, input.B4.down},
			{input.B1.on, input.B2.on, input.B3.on, input.B4.on},
			{input.B1.up, input.B2.up, input.B3.up, input.B4.up},
			{input.B1.off, input.B2.off, input.B3.off, input.B4.off}
		};

		int rtv = 0;

		//ビットのorで入手
		if (inp[state].A) {
			rtv |= A;
		}
		if (inp[state].B) {
			rtv |= B;
		}
		if (inp[state].X) {
			rtv |= X;
		}
		if (inp[state].Y) {
			rtv |= Y;
		}
		return rtv;
	}
	//-------------------------------------------------------------------
	//スコア加算
	void Object::AddScore(int& score, XI::GamePad::SP con)
	{
		ge->AddScore(controllerNum - 1, score);
		--score;
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
	Object::Object() :state(new IdleState()), isClear(false) {	}
	//-------------------------------------------------------------------
	void Object::Spawn(const ML::Vec2& pos_, XI::GamePad::SP controller_, const int& controllerNum_)
	{
		auto player = Create(true);
		player->pos = pos_;
		player->controller = controller_;
		//コントローラーの番号を取得
		player->controllerNum = controllerNum_ + 1;
		//プレイヤー画像
		player->imagePlayer = DG::Image::Create(player->imgPlayerPath[controllerNum_]);

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