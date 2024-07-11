#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//大食いミニゲーム本編
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include "OharaPlayerNumber.h"

namespace  OguiGame
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("本編");		//グループ名
	const  string  defName("大食いゲーム統括");	//タスク名
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
		DG::Image::SP	img;
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
		void  UpDate()			override;	//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();	//「終了」タスク消滅時に１回だけ行う処理
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		DG::Font::SP TestFont;

		//☆変数
		float timeLimit;	//制限時間
		bool  isGameOver;	//ミニゲーム終了(制限時間が0になった)時、trueにする

	private:
		//☆変数
		ML::Vec2 playerFirstPos[4] = {	//プレイヤーの初期位置
			{ ge->screen2DWidth / 5.f, ge->screen2DHeight / 2.f }, { ge->screen2DWidth * 2.f / 5.f, ge->screen2DHeight / 2.f },
			{ ge->screen2DWidth * 3.f / 5.f, ge->screen2DHeight / 2.f }, { ge->screen2DWidth * 4.f / 5.f, ge->screen2DHeight / 2.f }
		};
		XI::GamePad::SP controllers[4] = { ge->in1, ge->in2, ge->in3, ge->in4 };	//取得するコントローラー
		PlayerNum playersNum[4] = { PlayerNum::Player1, PlayerNum::Player2, PlayerNum::Player3, PlayerNum::Player4 }; //プレイヤーの識別番号設定用

		bool  nextTaskToGoIs;	//次のタスクにするかを判断。isGameOverがtrueになった後一定時間後、trueにする
		int   countToNextTask;	//次のタスクにするまでのカウント
	};
}