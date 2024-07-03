#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤー
//-------------------------------------------------------------------
#include "../BChara.h"

namespace TaxiGamePlayer2
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("プレイヤ");	//グループ名
	const  string  defName("タクシー");	//タスク名
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

		DG::Image::SP img;
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

		XI::GamePad::SP controller;//in1 in2など
		XI::VGamePad input;
		void Think();
		void Move();
		void Recieved() override;
		DG::Font::SP TestFont;
		string str;
		bool isClear;

		enum Button {
			A, B, X, Y
		};
		string btn[4] = {
			"A","B","X","Y"
		};
		int nowBtn = 0;
		void MatchButton();
		int matchCnt = 0;

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

		//通常
		class NormalState :public StateBase {
		public:
			NormalState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		//通常
		class ClearState :public StateBase {
		public:
			ClearState(Object* ptr) :owner_(ptr) {}
			void think() override;
			void move() override;
			void render() override;
		private:
			Object* owner_;
		};

		StateBase* state;

		void ChangeState(StateBase* state_);//状態変更
	};
}