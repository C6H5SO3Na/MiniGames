#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//バランスゲームのプレイヤー
//-------------------------------------------------------------------
#include "BGBChara.h"

namespace  BGPlayer
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("BGPlayer");	//グループ名
	const  string  defName("NoName");		//タスク名
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
		DG::Image::SP playerImg;
		DG::Image::SP playerNumImg;
		DG::Image::SP controllerMarkR;
		DG::Image::SP controllerMarkL;
	};
	//-------------------------------------------------------------------
	class  Object : public  BGBChara
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
		void Received() {};
		int direction;
		XI::GamePad::SP controller;
		int playerNum = 0;
		ML::Box2D playerNumUIsrc[4] = {
			ML::Box2D(0, 0, 155, 105),ML::Box2D(155, 0, 192, 105),ML::Box2D(347, 0, 175, 105), ML::Box2D(522, 0, 193, 105)
		};
		ML::Box2D playerNumUIdraw[4] = {
			ML::Box2D(0, 0, 155, 105),ML::Box2D(0, 0, 192, 105),ML::Box2D(0, 0, 175, 105), ML::Box2D(0, 0, 193, 105)
		};
		int animCnt;
		int animIndex;

	};
}