//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include "randomLib.h"
#include  "Task_Effect00.h"
#include "sound.h"
#include "easing.h"

#include  "Task_Game.h"

#include  <chrono>
#include  <thread>

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/gametitle_bg.png");
		this->TitleLogo01 = DG::Image::Create("./data/image/gametitle_title.png");
		this->StartLogo = DG::Image::Create("./data/image/gametitle_start.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->TitleLogo01.reset();
		this->StartLogo.reset();

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

		//BGM
		bgm::LoadFile("Title_bgm", "./data/sound/bgm/title_yutarisannbo.mp3");
		bgm::Play("Title_bgm");

		//SE
		se::LoadFile("se_confirm", "./data/sound/se/Common/confirmSE.wav");


		//★データ初期化
		controller = ge->in1;
		render2D_Priority[1] = 0.5f;
		this->Cnt = 0;
		int RenderTime = 0;

		ge->stage = 1;

		easing::Set("titleY", easing::BACKOUT, -250, ge->screen2DHeight / 5, 200);
		easing::Start("titleY");


		//デバッグ用フォントの準備
		TestFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);

		//音楽ファイルの読み込み
		//BGM
		// BGMの利用するファイルは曲の長さによってサイズが変わります。
		// 一般的にWavは中身が波形データと呼ばれるサイズが大きなデータにります。
		// 一方mp3はネットなどでの扱いを想定した圧縮形式です。
		// BGMを用いる場合はmp3形式のファイルを利用しましょう。
		// ちなみにこのサンプルのファイルは
		// 再生時間1：30ほぼの曲で
		// mp3 = 4.3MB wav = 19MBです。
		//bgm::LoadFile("bgm1", "./data/sound/bgm/弾むキモチ_2.mp3");
		//bgm::Play("bgm1");

		//se
		// seは効果音です。
		// bgmとの最大の違いはひとつの音楽ファイルの同時再生に対応していることです。
		// seはwavファイルしか扱うことが出来ません。mp3はエラーになります。
		// 同時再生の必要がないものはBGM
		// 同時再生が必要なものはseと使い分けてください。
	  // またこのサンプルのような日本語ファイル名はやめた方が良いです。
		// 読み込みエラーの元になります。
		//se::LoadFile("se1","./data/sound/se/「すごいすごい」.wav");

//		se::LoadFile("se2", "./data/sound/se/「頑張ったね」.mp3");これはエラー wavじゃないとダメ
		//se::LoadFile("se2", "./data/sound/se/「頑張ったね」.wav");

		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		bgm::Stop("Title_bgm");
		if (!ge->QuitFlag() && nextTaskCreate) {
			se::Play("se_confirm");
			std::this_thread::sleep_for(std::chrono::seconds(1));	
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = controller->GetState();

		this->Cnt++;

		int input = 0;
		//							16進		2進	
		if (inp.B1.down) input |= 0x0001;	// 0000000001
		if (inp.B2.down) input |= 0x0002;	// 0000000010
		if (inp.B3.down) input |= 0x0004;	// 0000000100
		if (inp.B4.down) input |= 0x0008;	// 0000001000
		if (inp.ST.down) input |= 0x0100;	// 0100000000

		if (input & 0x0100) {
			ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
			ge->CreateEffect(98, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			Kill();
		}

		easing::UpDate();
		logoY = easing::GetPos("titleY");

		return;

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		TestFont->Draw(ML::Box2D(0, 0, ge->screen2DWidth, ge->screen2DHeight),
			"TITLE"
		);

		ML::Box2D draw (0, 0, 1980, 1080);
		ML::Box2D src(0, 0, 1980, 1080);

		this->res->img->Draw(draw, src);

		ML::Box2D draw2(ge->screen2DWidth / 14.5, logoY, 1666, 530);
		ML::Box2D src2(0, 0, 1666, 530);
		this->res->TitleLogo01->Draw(draw2, src2);

		if (this->Cnt > 0)
		{
			if ((this->Cnt / 10) % 5 == 0)
			{
				return; //8フレーム中4フレーム画像を表示しない
			}
			ML::Box2D draw3(ge->screen2DWidth / 4.5, 750, 980, 234);
			ML::Box2D src3(0, 0, 980, 234);
			this->res->StartLogo->Draw(draw3, src3);
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
	Resource::~Resource() { Finalize(); }
}