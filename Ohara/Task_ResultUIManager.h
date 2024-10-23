#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//リザルト画面のUI管理
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include  "Task_ResultUIPlayer.h"
#include  "Task_Result.h"

namespace  ResultUIManager
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("管理");	//グループ名
	const  string  defName("UI管理");		//タスク名
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
		DG::Image::SP prefaceImage;
		DG::Image::SP buttonImage_A;
		DG::Image::SP buttonImage_A_Outline;
		DG::Image::SP thankyouImage;
		DG::Image::SP directToTitleImage;
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
		void DataChangesWhenUpdatingState(); //状態更新時にするデータ(変数)の変更
		 
	private:
		//☆変数
		ML::Vec2 playerRankEachPlayerPosition[4] = {	//表示用プレイヤーの位置
			ML::Vec2(ge->screen2DWidth / 5.f, ge->screen2DHeight / 2.f),		//一番左の位置
			ML::Vec2(ge->screen2DWidth * 2.f / 5.f, ge->screen2DHeight / 2.f),	//左から二番目の位置
			ML::Vec2(ge->screen2DWidth * 3.f / 5.f, ge->screen2DHeight / 2.f),	//左から三番の位置
			ML::Vec2(ge->screen2DWidth * 4.f / 5.f, ge->screen2DHeight / 2.f)	//一番右の位置
		};

		ML::Vec2 playerNumberPosition[4] = {			//プレイヤー番号の表示位置
			ML::Vec2(ge->screen2DWidth / 5.f, ge->screen2DHeight / 2.f - 200),			//一番左の位置
			ML::Vec2(ge->screen2DWidth * 2.f / 5.f, ge->screen2DHeight / 2.f - 200),	//左から二番目の位置
			ML::Vec2(ge->screen2DWidth * 3.f / 5.f, ge->screen2DHeight / 2.f - 200),	//左から三番の位置
			ML::Vec2(ge->screen2DWidth * 4.f / 5.f, ge->screen2DHeight / 2.f - 200)		//一番右の位置
		};

		ML::Vec2 rankSentencePosition[4] = {			//会社の階級の表示位置
			ML::Vec2(ge->screen2DWidth / 5.f, ge->screen2DHeight / 2.f + 200),			//一番左の位置
			ML::Vec2(ge->screen2DWidth * 2.f / 5.f, ge->screen2DHeight / 2.f + 200),	//左から二番目の位置
			ML::Vec2(ge->screen2DWidth * 3.f / 5.f, ge->screen2DHeight / 2.f + 200),	//左から三番の位置
			ML::Vec2(ge->screen2DWidth * 4.f / 5.f, ge->screen2DHeight / 2.f + 200)		//一番右の位置
		};

		ML::Vec2 scoreDrawPosition[4] = {				//スコアの描画位置
			ML::Vec2(ge->screen2DWidth / 5.f, ge->screen2DHeight / 2.f - 400),			//一番左の位置
			ML::Vec2(ge->screen2DWidth * 2.f / 5.f, ge->screen2DHeight / 2.f - 400),	//左から二番目の位置
			ML::Vec2(ge->screen2DWidth * 3.f / 5.f, ge->screen2DHeight / 2.f - 400),	//左から三番の位置
			ML::Vec2(ge->screen2DWidth * 4.f / 5.f, ge->screen2DHeight / 2.f - 400)		//一番右の位置
		};

		int  srcValues[4][4] = {	//画像の切り取り部分
			{ 0, 0, 567, 157 },		//「結果発表」の画像
			{ 0, 0, 128, 128},		// Aボタンの画像
			{ 0, 0, 1120, 101 },	//「遊んでくれてありがとう」の画像
			{ 0, 0, 635, 82 },		//「ボタンでタイトルへ」と描画
		};	
		int  drawValues[5][4] = {	//画像の描画位置
			{ -srcValues[0][2], -srcValues[0][3], srcValues[0][2] * 2, srcValues[0][3] * 2},	//「結果発表」の画像
			{ static_cast<int>(ge->screen2DWidth) - srcValues[1][2] - 50, static_cast<int>(ge->screen2DHeight) - srcValues[1][3] - 50, srcValues[1][2], srcValues[1][3] },									// ResultState::ResultAnnouncementで使うAボタンの画像
			{ static_cast<int>(ge->screen2DWidth) / 2 - srcValues[2][2] / 2, static_cast<int>(ge->screen2DHeight) / 2 - srcValues[2][3] / 2 - 150, srcValues[2][2], srcValues[2][3] },						//「遊んでくれてありがとう」の画像
			{ static_cast<int>(ge->screen2DWidth) / 2 - srcValues[1][2] / 2 - srcValues[3][2] / 2, static_cast<int>(ge->screen2DHeight) / 2 - srcValues[1][3] / 2 + 100, srcValues[1][2], srcValues[1][3]}, // ResultState::Endで使うAボタンの画像
			{ static_cast<int>(ge->screen2DWidth) / 2 - srcValues[3][2] / 2 + srcValues[1][2] / 2, static_cast<int>(ge->screen2DHeight) / 2 - srcValues[3][3] / 2 + 100, srcValues[3][2], srcValues[3][3]}, //「ボタンでタイトルへ」と描画
		};	

		ML::Vec2 prefaceImagePos = ML::Vec2(-srcValues[0][2], ge->screen2DHeight / 2.f);

		int  drawUpToCount;						// 描画するまでのカウント
		int	 animationCount;					// アニメーション用のカウント
		int  gameFps;							// ゲームプレイする上で想定しているFPS値を設定
		bool hasEndedDrawing;					// 描画が終了したらtrue
		bool isChangedFalse_hasEndedDrawing;	// hasEndedDrawingをfalseに変えたらtrue
		int	 displayPattern;					// 結果発表時の表示パターン
		bool resultStart;						// リザルト開始時true	

		//☆メソッド
		void ProcessEachResultState();				//Task_Resultの状態毎の処理
		void DrawingProcessingEachResultState();	//Task_Resultの状態毎の描画処理

		int  DecideDisplayPattern();	//結果発表時の表示パターンを決める
		void GenerateDisplayPlayer();   //表示用プレイヤーの生成
	};
}