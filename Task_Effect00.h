#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//エフェクト00
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Effect00
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("effect");	//グループ名
	const  string  defName("effect");	//タスク名
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
		DG::Image::SP  img;
		//追加したい変数・メソッドはここに追加する
		ML::Vec2	pos;		//キャラクタ位置
		ML::Vec2	moveVec;	//移動方向ベクトル
		ML::Vec2	graVec;		//重力各方向に準ずる
		int			resSizeX = 0, resSizeY = 0;//エフェクト画像一コマサイズ
		int			drawSizeX = 0, drawSizeY = 0;//エフェクト描画一コマサイズ

		float		scale = 0.f;		//拡大率
		float		addScale = 0.f;	//拡大率加算

		float		angle = 0.f;		//向き
		float		addAngle = 0.f;	//角度加算量

		bool		AddRender = false;	//加算合成フラグ
		float		alpha = 0.f;		//半透明
		float		addAlpha = 0.f;	//加算量

		int			count = 0;		//フレームカウント
		int			countMax = 0;	//消滅フレーム

		float		animno = 0.f;		//アニメーションコマ数
		float		addAnim = 0.f;	//アニメーション加算
		int			animMax = 0;	//終了到達コマ
		int			animResNum = 0;	//リソースの折り返し数

		bool fade = false;

		std::string filename;
	};
}