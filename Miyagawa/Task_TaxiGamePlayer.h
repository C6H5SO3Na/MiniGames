#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤー
//-------------------------------------------------------------------
#include "../BChara.h"
#include "Task_TaxiGamePlayerStateBase.h"

namespace TaxiGamePlayer
{
	//前方宣言
	class StateBase;

	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("タクシー");	//グループ名
	const  string  defName("プレイヤ");	//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();

		DG::Image::SP imageButton[2][4];//[0,1][A,B,X,Y]
		DG::Image::SP imageClear;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
		//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		static void Spawn(const ML::Vec2& pos_, XI::GamePad::SP controller_, const int& controllerNum_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);

		//コントローラーの変数
		XI::GamePad::SP controller;//in1 in2など
		XI::VGamePad input;

		//前回の位置(イージング用)
		ML::Vec2 prePosition;

		//プレイヤー画像
		DG::Image::SP imagePlayer;

		//プレイヤー画像のパス
		const string imgPlayerPath[4] = {
			"./data/image/game_otsan_run_red.png",
			"./data/image/game_otsan_run.png",
			"./data/image/game_otsan_run_yellow.png",
			"./data/image/game_otsan_run_green.png"
		};

		//クリアフラグ
		bool isClear;

		int animationCount = 0;

		int nowButton = 0;
		int matchCount = 0;

		int controllerNum = 1;

		//メソッド
		void Think();
		void Move();
		void Received() override;

		//ボタンのビット
		enum Button {
			A = 0b00010000,
			B = 0b00100000,
			X = 0b01000000,
			Y = 0b10000000
		};

		StateBase* state;

	public:
		int GetButtonNum(int state);
		void ChangeState(StateBase* state_);//状態変更
		bool IsClear() const { return isClear; }
		static int playerScore;//順位
		void AddScore(int& n, XI::GamePad::SP con);
		void MatchedButton();

		int GetNowButton() const { return nowButton; }
		XI::GamePad::SP GetController() const { return controller; }
		int GetControllerNum() const { return controllerNum; }

		int GetAnimationCount() const { return animationCount; }

		ML::Vec2 GetPrePosition() const { return prePosition; }

		void DrawClearMessage() const; //クリアメッセージ表示
		void DrawButton() const; //ボタン表示
		DG::Image::SP GetImagePlayer() const { return imagePlayer; }//プレイヤー画像リソースを取得
	};
}