#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���[�Î~���
//-------------------------------------------------------------------
#include "Task_TaxiGamePlayerStateBase.h"

namespace TaxiGamePlayer
{
	class IdleState :public StateBase {
	public:
		IdleState() = default;

		void think(Object& player) override;
		void move(Object& player) override;
		void render(Object& player) override;
	};
}