#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲームの最初に出る指示
//-------------------------------------------------------------------
#include "../BChara.h"

namespace GameMessage
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("UI");	//グループ名
	const  string  defName("イージングロゴ");//タスク名
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
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
		//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		static  Object::SP  Create(DG::Image::SP img, const ML::Box2D& src, const string& SEName = "default");
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

		ML::Box2D src;
		DG::Image::SP img;
		ML::Vec2 pos;
		bool isPlayedSE = false;
		string SEName = "default"; //書き忘れで落ちないようにする
	};
}