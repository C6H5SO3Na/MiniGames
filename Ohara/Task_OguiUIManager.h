#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//大食いミニゲームのUI管理
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"

namespace  OguiUIManager
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("管理");	//グループ名
	const  string  defName("UI管理");	//タスク名
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
		DG::Image::SP playerNumberImage;
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
		
	private:
		//☆変数
		DG::Font::SP testFont;

		//プレイヤー識別番号関係-------------------------------------------------------------------------------------
		//☆構造体
		struct PlayerNumberDrawInformation
		{
			ML::Box2D	draw, src;	// 描画位置, 元画像切り取り位置
			ML::Vec2	pos;		// Offsetの位置
		};
		
		//☆変数
		PlayerNumberDrawInformation playerNumbersDrawInfo[4];	// プレイヤー番号の描画情報
		int							playerCount;				// ゲームを遊ぶプレイヤーの人数(1〜4の範囲で値を入れる)

		//☆関数
		void DrawPlayerNumber();	//プレイヤー番号の描画
	};
}