#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//コントローラー登録画面
//-------------------------------------------------------------------
#include "../GameEngine_Ver3_83.h"

namespace SubscribeController
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("コントローラー登録画面");	//グループ名
	const  string  defName("Noname");//タスク名
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

		void Subscribe(XI::GamePad::SP controller, bool& isPush, const int& i);

		DG::Font::SP TestFont;
		bool isPushButton[4] = {};
		int controllerIndex[4] = {};//~Pは~コントローラーか
		int subscribeCnt = 0;
		float easingPos[4] = {};

		vector<XI::GamePad::SP> inputs;
	};
}