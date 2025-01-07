#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤーの状態の抽象インターフェース
//-------------------------------------------------------------------
#include "Task_TaxiGamePlayer.h"

namespace TaxiGamePlayer
{
	//前方宣言
	class Object;

	//ポリモーフィズム
	class StateBase {
	public:
		virtual ~StateBase() {}
		virtual void think(Object& player) = 0;
		virtual void move(Object& player) = 0;
		virtual void render(Object& player) = 0;
	};
}