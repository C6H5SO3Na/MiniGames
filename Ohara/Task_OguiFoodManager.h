#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//大食いミニゲームの料理管理
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include "OharaPlayerNumber.h"

namespace  OguiFoodManager
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("管理");		//グループ名
	const  string  defName("大食い料理管理");	//タスク名
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
		//☆メソッド
		void NotExistFood(PlayerNum food_PlayerNum);	//料理がなくなった情報を取得

	private:
		//☆変数
		int  createCount[4];			//料理が完成するまでのカウント、要素数0を1Pの料理、要素数1を2Pの料理・・・として使用する
		bool hasExistFoods[4];			//料理が存在しているか判別、要素数0を1Pの料理、要素数1を2Pの料理・・・として使用する
		ML::Vec2 foodPositions[4] = {	//料理の出現位置
			{ ge->screen2DWidth / 8.f, ge->screen2DHeight / 2.f + 100 }, 
			{ ge->screen2DWidth * 3.f / 8.f, ge->screen2DHeight / 2.f + 100 },
			{ ge->screen2DWidth * 5.f / 8.f, ge->screen2DHeight / 2.f + 100 },
			{ ge->screen2DWidth * 7.f / 8.f, ge->screen2DHeight / 2.f + 100 }
		};

		//☆メソッド
		void CreateFood(PlayerNum food_PlayerNum);	//料理の生成
		void SendCreateFoodInformation(PlayerNum food_PlayerNum);			//料理を生成した情報を他タスクに送る
	};
}