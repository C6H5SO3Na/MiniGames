#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���[�̏�Ԃ̒��ۃC���^�[�t�F�[�X
//-------------------------------------------------------------------
#include "Task_TaxiGamePlayer.h"

namespace TaxiGamePlayer
{
	//�O���錾
	class Object;

	//�|�����[�t�B�Y��
	class StateBase {
	public:
		virtual ~StateBase() {}
		virtual void think(Object& player) = 0;
		virtual void move(Object& player) = 0;
		virtual void render(Object& player) = 0;
	};
}