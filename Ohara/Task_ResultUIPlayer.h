#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//リザルト表示用プレイヤー
//-------------------------------------------------------------------
#include "OharaCharacterBase.h"
#include "Task_Result.h"

namespace  ResultUIPlayer
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("画面表示用");	//グループ名
	const  string  defName("表示用プレイヤー");	//タスク名
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
		DG::Image::SP playerImage[4];
		DG::Image::SP playerNumberImage;
		DG::Image::SP rankImage;
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
	private:
		//☆構造体
		//描画するプレイヤーの情報
		struct DrawPlayerInformation
		{
			int PlayerNum;
			int rank;
			int score;
		};
		DrawPlayerInformation drawPlayerInfo = {};

		//描画する位置
		struct DrawPos
		{
			ML::Vec2 displayPlayerPos;
			ML::Vec2 playerNumPos;
			ML::Vec2 rankPos;
			ML::Vec2 scorePos;
		};
		DrawPos drawPos = {};
		
		//☆メソッド
		DrawInformation GetDrawImages(int imageNum); ////描画画像の制御　imageNumが0 = プレイヤー、1 = プレイヤーの番号、2 = 順位、3 = スコア　の画像制御

		virtual void Think() override {}
		virtual void Move() override {}
		virtual DrawInformation GetDrawImage() override { return {}; }
		virtual void Received() override {}

	public:
		//☆メソッド
		void setPlayerInformation(const Result::Object::PlayerInformation& playerInfo);	//描画するプレイヤーの情報を取得する
		void setDrawPos(const ML::Vec2& playerPos, const ML::Vec2& playerNumPos, const ML::Vec2& rankPos, const ML::Vec2& scorePos); //描画位置を設定する
	};
}