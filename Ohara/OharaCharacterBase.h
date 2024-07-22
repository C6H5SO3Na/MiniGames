#pragma once
//-----------------------------------------------------------------------------
//小原が制作したキャラクターの基底クラス
//-----------------------------------------------------------------------------
#include "../BChara.h"
#include "OharaPlayerNumber.h"

class OCharaBase : public BChara
{
public:
	//コンストラクタ
	OCharaBase()
		:
		state(State::Default),
		playerNum(PlayerNum::Player1),
		gameFps(60)
	{}

	//デストラクタ
	virtual ~OCharaBase() {}

protected:
	//☆クラス・構造体
	struct DrawInformation
	{
		//☆メンバ変数
		ML::Box2D draw, src;
	};

	//☆変数
	int gameFps;	//ゲームプレイする上で想定しているFPS値を設定
	
public:
	//☆クラス・構造体
	enum class State
	{
		//☆共通
		Default,						//初期状態、何もしない

		//☆サボりミニゲームのプレイヤー用
		PWork,						//仕事中状態
		PSabori,					//サボり状態
		PNoticed,					//サボりばれの状態

		//☆サボりミニゲームの上司用
		JWork,						//仕事中状態
		JBeforeCheckingForSabori,	//サボり確認の予備動作状態
		JCheckForSabori,			//サボり確認状態
		JFoundSabori,				//サボり発見状態

		//☆大食いミニゲームのプレイヤー用
		PWait,						//待機状態
		PEat,						//食事中状態

		//☆大食いミニゲームの料理用
		FExist,						//料理が残っている状態
		FNotExist,					//料理が残っていない状態
	};

	//☆変数
	State		 state;
	PlayerNum    playerNum;

protected:
	//☆メソッド
	virtual void Think() = 0;					//キャラクターの状態制御
	void UpdateState(State nowState);			//状態更新時の処理
	virtual void Move() = 0;					//状態毎の行動処理
	virtual DrawInformation GetDrawImage() = 0;	//描画画像の制御
};