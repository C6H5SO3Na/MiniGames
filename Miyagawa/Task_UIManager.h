#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//UIマネージャー
//-------------------------------------------------------------------
#include "../BChara.h"
#include  "Task_EasingLogo.h"
#include  "Task_FightLogo.h"
#include  "Task_TimeLimitBar.h"

namespace UIManager
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("UI");	//グループ名
	const  string  defName("マネージャー");//タスク名
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
	class  Object : public  BChara
	{
		//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
		static EasingLogo::Object::SP ShowRule();
		static EasingLogo::Object::SP ShowFinish();
		static TimeLimitBar::Object::SP CreateTimeLimitBar(const ML::Vec2& pos, const int& time);
		static FightLogo::Object::SP CreateFightLogo();

	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理

		void Received() override {}
		enum class Phase {
			None, RuleShow, RuleEasing, StartShow, StartEasing, FinishShow, FinishEasing,
		};
		Phase phase = Phase::RuleShow;
	};
}