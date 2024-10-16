#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲームの最初に出る指示
//-------------------------------------------------------------------
#include "../BChara.h"
#include  "Task_GameMessage.h"

namespace EasingLogo
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("UI");	//グループ名
	const  string  defName("イージングするロゴ");//タスク名
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

		//それぞれのゲームの指示の画像を格納
		vector<shared_ptr<DG::Image>> img;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
		//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		static  Object::SP  Spawn(const bool& isFinish);
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

		ML::Vec2 pos;
		GameMessage::Object::WP logo;//循環参照を防ぐためweak_ptrを使用
		bool isFinish;
		bool isCreated = false;

		ML::Box2D srcTable[8] = {
			ML::Box2D(0, 0, 647, 123),//Stage1
			ML::Box2D(0, 0, 341, 96),
			ML::Box2D(0, 0, 585, 94),
			ML::Box2D(0, 0, 583, 116),
			ML::Box2D(0, 0, 750, 161),
			ML::Box2D(0, 0, 700, 150),
			ML::Box2D(0, 0, 564, 116),
			ML::Box2D(0, 0, 438, 105)//Finish
		};
	};
}