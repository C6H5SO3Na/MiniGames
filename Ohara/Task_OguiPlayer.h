#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//大食いミニゲームのプレイヤー
//-------------------------------------------------------------------
#include "OharaCharacterBase.h"

namespace  OguiPlayer
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("プレイヤー");	//グループ名
	const  string  defName("大食いプレイヤー");	//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()		override;
		bool  Finalize()		override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//共有する変数はここに追加する
		DG::Image::SP image[4];
		DG::Image::SP buttonImage_A;
		DG::Image::SP buttonImage_A_Outline;
	};
	//-------------------------------------------------------------------
	class  Object : public  OCharaBase
	{
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
		void  UpDate()		override;	//「実行」１フレーム毎に行う処理
		void  Render2D_AF()	override;	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//追加したい変数・メソッドはここに追加する
		//BCharaに含まれないモノのみここに追加する
		//☆変数
		XI::GamePad::SP controller;		//入力情報を受け取りたいコントローラーのデータを格納
		int				eatFoodCount;	//食べた料理の数

		//☆メソッド
		void SetExistFood(bool FoodExistenceInformation);	//料理の存在しているかの情報を取得

		virtual void Received() override {}

	private:
		//☆変数
		int	 attack;		//食事量、1回ボタンを押して減る料理の量を表す
		bool existFood;		//料理があるか判断する、あったらtrue

		bool isPlayStart;		//ゲーム本編が開始されたらtrue

		bool isStartButtonDraw;	//ボタンの描画をするときtrue
		ML::Vec2 buttonDrawPos;	//ボタンの描画位置

		//☆メソッド
		virtual void Think() override;
		virtual void Move() override;
		virtual DrawInformation GetDrawImage();

		void DrawButton();	//ボタンの描画処理
	};
}