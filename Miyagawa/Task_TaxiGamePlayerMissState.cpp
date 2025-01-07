//-------------------------------------------------------------------
//ƒvƒŒƒCƒ„[“ü—ÍŽ¸”só‘Ô
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
	//Žvl
	void  MissState::think(Object& player)
	{
		if (player.moveCnt == 120) {
			player.ChangeState(new IdleState());
		}
	}
	//-------------------------------------------------------------------
	//s“®
	void  MissState::move(Object& player)
	{
		++player.moveCnt;
	}
	//-------------------------------------------------------------------
	//•`‰æ
	void  MissState::render(Object& player)
	{
		//•`‰æ‹éŒ`
		ML::Box2D src(0, 0, 342, 486);
		ML::Box2D draw(-src.w / 2, -src.h / 2, -src.w, src.h);
		draw *= 0.35f;
		draw.Offset(player.pos);
		//U“®
		draw.Offset(GetRandom(0, 10), GetRandom(0, 10));
		player.GetImagePlayer()->Draw(draw, src, ML::Color(1.f, 1.f, 0.5f, 0.5f));

		player.DrawButton();
	}
}