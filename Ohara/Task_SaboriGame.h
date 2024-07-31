#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//サボりミニゲーム本編
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"
#include "OharaPlayerNumber.h"

namespace  SaboriGame
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("サボりゲーム本編");		//グループ名
	const  string  defName("サボりゲーム統括");	//タスク名
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
		DG::Image::SP readyImage;
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
		ML::Vec2 playerFirstPos[4] = {	//プレイヤーの初期位置
			{ ge->screen2DWidth / 8.f, ge->screen2DHeight - 230.f }, 
			{ ge->screen2DWidth * 3.f / 8.f, ge->screen2DHeight - 230.f },
			{ ge->screen2DWidth * 5.f / 8.f, ge->screen2DHeight - 230.f },
			{ ge->screen2DWidth * 7.f / 8.f, ge->screen2DHeight - 230.f }
		};
		ML::Vec2 joushiFirstPos = { ge->screen2DWidth / 2.f, ge->screen2DHeight / 3.f };	//上司の初期位置
		XI::GamePad::SP controllers[4] = { ge->in1, ge->in2, ge->in3, ge->in4 };			//取得するコントローラー
		PlayerNum playersNum[4] = { PlayerNum::Player1, PlayerNum::Player2, PlayerNum::Player3, PlayerNum::Player4 }; //プレイヤーの識別番号設定用

		bool  gameStart;			//ゲーム開始時true	
		float timeLimit;			//制限時間
		bool  isInGame;				//ミニゲーム中か判断する。ミニゲーム中trueにする
		int   countToNextTask;		//次のタスクにするまでのカウント

		//☆列挙型
		enum class GameState
		{
			BeforeGameStart,	//ゲーム開始前
			Game,				//ゲーム中
			End,				//ゲーム終了
		};
		GameState gameState;

		//☆メソッド
		void GameStateTransition();					//ゲームの状態遷移
		void UpdateGameState(GameState nowState);	//ゲームの状態変更時処理
		void Work();								//状態毎の処理

	private:
		//☆構造体
		//順位決めに必要なプレイヤーの情報
		struct PlayerInformation
		{
			PlayerNum playerNum;		//プレイヤー識別用番号
			float	  totalSaboriTime;	//合計さぼり時間
			int		  rank;				//順位
		};

		//☆変数
		PlayerInformation playersInfo[4];

		ML::Vec2 readyImagePos = ML::Vec2(0.f, ge->screen2DHeight / 2.f);
		ML::Vec2 fightImagePos = ML::Vec2(ge->screen2DWidth / 2.f, ge->screen2DHeight / 2.f);
		ML::Vec2 finishImagePos = ML::Vec2(0.f, ge->screen2DHeight / 2.f);

		int gameFps = 60;
		int countToFightDraw;			//「Fight」の文字を描画するまでのカウント
		int countToChangeGameState;		//GameStateを変更するまでのカウント

		//☆メソッド
		void Ranking();		//順位決めの処理
		bool compare(const PlayerInformation& playerInfoA, const PlayerInformation& playerInfoB);	//playerInfoAとplayerInfoBのtotalSaboriTimeで比較し、playerInfoAの方が大きい時trueを返す
		void SendScore();	//ge->scoreに得点を送る

		void Render();		//状態毎の描画
	};
}