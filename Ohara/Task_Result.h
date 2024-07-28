#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//最終リザルト画面タスク
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"

namespace  Result
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("リザルト画面");	//グループ名
	const  string  defName("リザルト統括");	//タスク名
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

		//☆クラス・構造体・列挙型
		enum class ResultState
		{
			Preface,			//前置き
			ResultAnnouncement,	//結果発表
			End,				//結果発表終了
		};
		ResultState resultState;

		struct PlayerInformation
		{
			int PlayerNum;
			int rank;
			int score;
		};
		PlayerInformation playersInfo[4] = { {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 0} };
		
		//☆メソッド
		void ChangeableToNextState();	//次の状態に変更できるようにする

	private:		
		//☆変数
		XI::GamePad::SP controllers[4] = { ge->in1, ge->in2, ge->in3, ge->in4 }; //入力するコントローラーの情報を格納
		
		bool nextStateGoIs;			//trueになったら次の状態に変更出来るようにする
		int  countUpToStateChange;	//状態変更までのカウント
		int  gameFps;				//ゲームプレイする上で想定しているFPS値を設定
		bool shouldKillTask;		//UpDate処理内でTaskを消滅させる時に使用 trueの時に消滅させる

		//☆メソッド
		void ResultStateTransition();					//リザルトの状態遷移
		void UpdateResultState(ResultState nowState);	//リザルトの状態変更時処理
		void ProcessEachState();						//状態毎の処理

		void Ranking();	//順位決めの処理
		bool compare(const PlayerInformation& scoreA, const PlayerInformation& scoreB);	//scoreAとscoreBに入れた変数のscoreで比較し、scoreAが大きい時trueを返す
	};
}