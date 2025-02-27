#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤー入力失敗状態
//-------------------------------------------------------------------
#include "Task_TaxiGamePlayerStateBase.h"

namespace TaxiGamePlayer
{
	class MissState :public StateBase {
	public:
		MissState() = default;

		void think(Object& player) override;
		void move(Object& player) override;
		void render(Object& player) override;
	};
}