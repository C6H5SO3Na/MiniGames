#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//サボりミニゲームの上司
//-------------------------------------------------------------------
#include "OharaCharacterBase.h"

namespace  SaboriJoushi
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ギミック");	//グループ名
	const  string  defName("上司");				//タスク名
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
		DG::Image::SP image;
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
		void  Render2D_AF()	override;	//「2D描画」１フレーム毎に知らせ行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//追加したい変数・メソッドはここに追加する
		//BCharaに含まれないモノのみここに追加する

		//☆変数
		float changeStateTime;			//状態を変更するタイミングを格納する
		int	  changeStateKinds;			//変更する状態の種類を決める数字を格納する
		bool  hasFoundPlayerSabori;		//プレイヤーがさぼっていたらtrue

		//☆メソッド
		virtual void Think() override;
		virtual void Move() override;
		virtual DrawInformation GetDrawImage() override;
		virtual void Received() override {}

		void CheckPlayerSabori(); //プレイヤーがさぼり状態かどうか判定する
	};
}