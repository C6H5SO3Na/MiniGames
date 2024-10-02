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
	const  string  defGroupName("大食いゲーム本編");		//グループ名
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
		DG::Image::SP gameRuleImage;
		DG::Image::SP fightImage;
		DG::Image::SP finishImage;
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
		//☆変数
		// 大食いゲーム関係------------------------------------------------------------------------------------------------------------------------
		float timeLimit;	//制限時間
		bool  isInGame;		//ミニゲーム中、trueにする

	private:
		//☆構造体・列挙型
		//ゲームの状態
		enum class GameState
		{
			BeforeGameStart,	//ゲーム開始前
			Game,				//ゲーム中
			End,				//ゲーム終了
		};

		//順位決めに必要なプレイヤーの情報
		struct PlayerInformation
		{
			PlayerNum playerNum;	//プレイヤー識別用番号
			int		  eatFoodCount;	//食べた料理の数
			int		  rank;			//順位
		};

		//☆変数
		// プレイヤー関係--------------------------------------------------------------------------------------------------------------------------
		ML::Vec2				playerFirstPos[4];	// プレイヤーの初期位置
		XI::GamePad::SP			controllers[4];		// 取得するコントローラー
		vector<XI::GamePad::SP> useControllers;		// 実際に使用するコントローラーを格納する
		PlayerNum				playersNum[4];		// プレイヤーの識別番号設定用
		int						playerCount;		// ゲームを遊ぶプレイヤーの人数(1～4の範囲で値を入れる)
		PlayerInformation		playersInfo[4];		// 順位決めに必要なプレイヤーの情報

		// 大食いゲーム関係--------------------------------------------------------------------------------------------------------------------------
		GameState	gameState;				// ゲームの状態
		int			countToChangeGameState;	// GameStateを変更するまでのカウント
		int			countToNextTask;		// 次のタスクにするまでのカウント
		int			gameFps;				// 想定FPS
		bool		gameStart;				// ゲーム開始時true

		// 文字描画関係-----------------------------------------------------------------------------------------------------------------------------
		ML::Vec2	gameRuleImagePos;	// ゲームの説明の文章の初期位置
		ML::Vec2	fightImagePos;		// 「Fight」の文字の初期位置
		ML::Vec2	finishImagePos;		// 「Finish」の文字の初期位置
		int			countToFightDraw;	// 「Fight」の文字を描画するまでのカウント

		//☆関数
		void GameStateTransition();					//ゲームの状態遷移
		void UpdateGameState(GameState nowState);	//ゲームの状態変更時処理
		void Work();								//状態毎の処理
		void Render();								//状態毎の描画

		void Ranking();		//順位決めの処理
		bool compare(const PlayerInformation& playerInfoA, const PlayerInformation& playerInfoB);	//playerInfoAとplayerInfoBのeatFoodCountで比較し、playerInfoAの方が大きい時trueを返す
		void SendScore();	//ge->scoreに得点を送る

	public:
		//getter関数
		int GetPlayerCount();	// ゲームを遊ぶプレイヤーの人数の情報を渡す
	};
}