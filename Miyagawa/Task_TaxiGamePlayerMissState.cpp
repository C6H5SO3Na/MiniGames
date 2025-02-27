//-------------------------------------------------------------------
//プレイヤー入力失敗状態
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include "../easing.h"
#include "../sound.h"
#include "../randomlib.h"
#include "Task_TaxiGamePlayerIdleState.h"
#include "Task_TaxiGamePlayerMissState.h"

namespace TaxiGamePlayer
{
	//-------------------------------------------------------------------
	//思考
	void  MissState::think(Object& player)
	{
		if (player.moveCnt == 120) {
			player.ChangeState(new IdleState());
		}
	}
	//-------------------------------------------------------------------
	//行動
	void  MissState::move(Object& player)
	{
		++player.moveCnt;
	}
	//-------------------------------------------------------------------
	//描画
	void  MissState::render(Object& player)
	{
		//描画矩形
		ML::Box2D src(0, 0, 342, 486);
		ML::Box2D draw(-src.w / 2, -src.h / 2, -src.w, src.h);
		draw *= 0.35f;
		draw.Offset(player.pos);
		//振動
		draw.Offset(GetRandom(0, 10), GetRandom(0, 10));
		player.GetImagePlayer()->Draw(draw, src, ML::Color(1.f, 1.f, 0.5f, 0.5f));

		player.DrawButton();
	}
}