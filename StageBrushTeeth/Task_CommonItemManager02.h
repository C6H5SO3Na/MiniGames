#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include "Task_StainManager.h"
#include "Task_brush.h"

namespace  CommonItemManager02
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("共通アイテムマネージャー02");	//グループ名
	const  string  defName("マネージャー");	//タスク名
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
		//共有する変数はここに追加する
		//「変数宣言を書く」
		DG::Image::SP bgImg;
		DG::Image::SP teethImg;
		DG::Image::SP PlayerNum;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		//「変数宣言を書く」
		//「追加メソッドを書く」
		vector<StainManager::Object::SP> CreateStatinList;
		vector<brush::Object::SP> PLBrushList;
		vector<XI::GamePad::SP> CTList;

		int addscore[4] = { 4, 2, 1, 0 };
		int rank = 0;

		ML::Box2D PlayerAreaPos[4][4] = {
			{ ML::Box2D(480, 270, 960, 540)},
			{ ML::Box2D(480, 0, 960, 540), ML::Box2D(480, 540, 960, 540)},
			{ ML::Box2D(0, 0, 960, 540), ML::Box2D(960, 0, 960, 540), ML::Box2D(480, 540, 960, 540)},
			{ ML::Box2D(0, 0, 960, 540), ML::Box2D(960, 0, 960, 540), ML::Box2D(0, 540, 960, 540), ML::Box2D(960, 540, 960, 540) }
		};

		ML::Box2D PlayerNumIndexSrc[4] = {
			{ ML::Box2D(0, 0, 715 / 4 - 20, 105)},
			{ ML::Box2D(715 / 4 - 20, 0, 715 / 4, 105)},
			{ ML::Box2D(715 / 2 - 10, 0, 715 / 4, 105)},
			{ ML::Box2D(715 / 4 * 3 - 10, 0, 715 / 4, 105)}
		};
	};
}