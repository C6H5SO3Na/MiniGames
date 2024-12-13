#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//c่ิฬo[
//-------------------------------------------------------------------
#include "../BChara.h"
#include  "Task_EasingLogo.h"

namespace TimeLimitBar
{
	//^XNษ่ฤ้O[vผฦลLผ
	const  string  defGroupName("UI");	//O[vผ
	const  string  defName("c่ิo[");//^XNผ
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
		//ฯXsย
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
		bool  Initialize();	//u๚ปv^XNถฌษP๑พฏsค
		void  UpDate()			override;//uภsvPt[ษsค
		void  Render2D_AF()		override;//u2D`ๆvPt[ษsค
		bool  Finalize();		//uIนv^XNมลษP๑พฏsค

		//ถฌ๛ ๘อtrueล^XNVXeึฉฎo^
		static  Object::SP  Create(bool flagGameEnginePushBack_);

		ML::Box2D srcBase;
		ML::Box2D drawBase;
		int remainingCnt;
		float maxCnt;
		int minPower;
		float gaugeAmount;
		EasingLogo::Object::WP finish;//FinishS

		ML::Point barSize = { 96,32 };

		void Received() override{}

		void DrawFlame() const;//g`ๆ
		void DrawGauge() const;//Q[W`ๆ
	};
}