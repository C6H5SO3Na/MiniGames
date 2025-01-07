//-------------------------------------------------------------------
//プレイヤー静止状態
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include "../easing.h"
#include "../sound.h"
#include "Task_TaxiGamePlayerIdleState.h"
#include "Task_TaxiGamePlayerMoveState.h"
#include "Task_TaxiGamePlayerMissState.h"

namespace TaxiGamePlayer
{
	//-------------------------------------------------------------------
	//思考
	void  IdleState::think(Object& player)
	{
		if (ge->gameState != MyPG::MyGameEngine::GameState::Game) {
			return;
		}
		if (player.GetButtonNum(0) == 0) { return; }
		if (player.GetButtonNum(0) == pow(2, 4 + player.GetNowButton())) {//ビット単位のための計算
			easing::Set("move" + to_string(player.GetControllerNum()), easing::QUADINOUT, 0, -150, 60);
			easing::Start("move" + to_string(player.GetControllerNum()));
			player.ChangeState(new MoveState());
			se::Play("Walk");
		}
		else {
			player.ChangeState(new MissState());
			se::Play("Miss");
		}
	}
	//-------------------------------------------------------------------
	//行動
	void  IdleState::move(Object& player)
	{
	}
	//-------------------------------------------------------------------
	//描画
	void  IdleState::render(Object& player)
	{
		//プレイヤ描画
		ML::Box2D src(0, 0, 342, 486);
		ML::Box2D draw(-src.w / 2, -src.h / 2, -src.w, src.h);
		draw *= 0.35f;
		draw.Offset(player.pos);
		player.GetImagePlayer()->Draw(draw, src);

		if (ge->gameState == MyPG::MyGameEngine::GameState::Game) {
			player.DrawButton();
		}

	}
}