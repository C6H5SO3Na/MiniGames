#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤー移動状態
//-------------------------------------------------------------------
#include "Task_TaxiGamePlayerStateBase.h"

namespace TaxiGamePlayer
{
	class MoveState :public StateBase {
	public:
		MoveState() = default;

		void think(Object& player) override;
		void move(Object& player) override;
		void render(Object& player) override;
	private:
		vector<ML::Box2D> animationTable_;
	};
}