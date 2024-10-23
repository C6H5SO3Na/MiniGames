#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//残り時間のバー
//-------------------------------------------------------------------
#include "../BChara.h"
#include  "Task_EasingLogo.h"

namespace TimeLimitBar
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("UI");	//グループ名
	const  string  defName("残り時間バー");//タスク名
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

		DG::Image::SP img;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
		//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		static  Object::SP Create(const ML::Vec2& pos);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理

		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);

		ML::Box2D srcBase;
		ML::Box2D drawBase;
		int remainingCnt;
		float maxCnt;
		int minPower;
		float gaugeAmount;
		EasingLogo::Object::WP finish;//Finishロゴ

		ML::Point barSize = { 96,32 };

		void Received() override{}

		void DrawFlame() const;//枠描画
		void DrawGauge() const;//ゲージ描画
	};
}