#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//バランスゲーム
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include "Task_ClassifyGamePlayer.h"

namespace  ClassifyGamePM
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ClassifyGamePM");	//グループ名
	const  string  defName("NoName");	//タスク名
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
		int gameCnt;

		vector<CGPlayer::Object::SP> pList;	//プレイヤリスト
		vector<XI::GamePad::SP> cList;		//コントローラリスト
		struct NumIndex { //順位決める用
			int num;
			int index;
		};
		static bool compare(const NumIndex& a, const NumIndex& b);
		void assignRanks(vector<int>& nums, vector<int>& ranks);
		NumIndex ni;
		ML::Vec2 playerWindowVec[4][4] = { 
			{ML::Vec2(945,570),ML::Vec2(0,0),ML::Vec2(0,0),ML::Vec2(0,0)},
			{ML::Vec2(945,300),ML::Vec2(945,840),ML::Vec2(0,0),ML::Vec2(0,0)},
			{ML::Vec2(450,300),ML::Vec2(1410,300),ML::Vec2(945,840),ML::Vec2(0,0)},
			{ML::Vec2(450,300),ML::Vec2(1410,300),ML::Vec2(450,840),ML::Vec2(1410,840)}
		};
		bool isCount;
	};
}