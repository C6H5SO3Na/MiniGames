#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���[�N���A���
//-------------------------------------------------------------------
#include "Task_TaxiGamePlayerStateBase.h"

namespace TaxiGamePlayer
{
	class ClearState :public StateBase {
	public:
		ClearState() = default;

		void think(Object& player) override;
		void move(Object& player) override;
		void render(Object& player) override;
	};
}