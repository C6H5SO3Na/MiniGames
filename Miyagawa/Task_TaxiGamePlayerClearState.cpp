//-------------------------------------------------------------------
//�v���C���[�N���A���
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include "../easing.h"
#include "../sound.h"
#include "Task_TaxiGamePlayerClearState.h"

namespace TaxiGamePlayer
{
	//-------------------------------------------------------------------
	//�v�l
	void  ClearState::think(Object& player)
	{
	}
	//-------------------------------------------------------------------
	//�s��
	void  ClearState::move(Object& player)
	{
	}
	//-------------------------------------------------------------------
	//�`��
	void  ClearState::render(Object& player)
	{
		//�v���C���[�`��
		ML::Box2D src(0, 0, 342, 486);
		ML::Box2D draw(-src.w / 2, -src.h / 2, -src.w, src.h);
		draw *= 0.35f;
		draw.Offset(player.pos);
		player.GetImagePlayer()->Draw(draw, src);

		//�N���A���b�Z�[�W�`��
		player.DrawClearMessage();
	}
}