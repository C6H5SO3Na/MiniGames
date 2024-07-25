#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤー
//-------------------------------------------------------------------
#include "../BChara.h"

namespace TaxiGamePlayer
{
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

		DG::Image::SP imgPlayer;
		DG::Image::SP imgBtn[2][4];//[0,1][A,B,X,Y]
		DG::Image::SP imgClear;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
		//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		static void Spawn(const ML::Vec2& pos_, XI::GamePad::SP controller_);
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
		ML::Vec2 prePos;

		//クリアフラグ
		bool isClear;

		int animCnt = 0;

		int nowBtn = 0;
		int matchCnt = 0;

		//メソッド
		void Think();
		void Move();
		void Recieved() override;
		void DrawButton();
		void MatchButton();

		//ボタンのビット
		enum Button {
			A = 0b00010000,
			B = 0b00100000,
			X = 0b01000000,
			Y = 0b10000000
		};

		//ポリモーフィズム
		//状態の抽象インターフェース
		class StateBase {
		public:
			virtual ~StateBase() {}
			virtual void think() = 0;
			virtual void move() = 0;
			virtual void render() = 0;
		private:
			ML::Box2D animTable_;
		};

		//静止
		class IdleState :public StateBase {
		public:
			IdleState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		//動いている
		class MoveState :public StateBase {
		public:
			MoveState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		//クリア
		class ClearState :public StateBase {
		public:
			ClearState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		//ミス
		class MissState :public StateBase {
		public:
			MissState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		StateBase* state;

		void ChangeState(StateBase* state_);//状態変更
		int BUTTON(int state);
		void AddScore(int& n, XI::GamePad::SP con);

	public:
		bool IsClear() const { return isClear; }
		static int playerScore;//順位
	};
}