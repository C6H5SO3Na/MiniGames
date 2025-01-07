//-------------------------------------------------------------------
//プレイヤー移動状態
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include "../easing.h"
#include "../sound.h"
#include "Task_TaxiGamePlayerMoveState.h"
#include "Task_TaxiGamePlayerIdleState.h"
#include "Task_TaxiGamePlayerClearState.h"

namespace TaxiGamePlayer
{
	//-------------------------------------------------------------------
	//思考
	void  MoveState::think(Object& player)
	{
		if (easing::GetState("move" + to_string(player.GetControllerNum())) == easing::EQ_STATE::EQ_END) {
			player.MatchedButton();
			if (player.IsClear()) {
				player.AddScore(Object::playerScore, player.GetController());
				player.ChangeState(new ClearState());
				se::Play("Clear");
				return;
			}
			player.ChangeState(new IdleState());
		}
	}
	//-------------------------------------------------------------------
	//行動
	void  MoveState::move(Object& player)
	{
		player.pos.x = easing::GetPos("move" + to_string(player.GetControllerNum())) + player.GetPrePosition().x;
	}
	//-------------------------------------------------------------------
	//描画
	void  MoveState::render(Object& player)
	{
		animationTable_.push_back(ML::Box2D(342, 0, 342, 486));
		animationTable_.push_back(ML::Box2D(684, 0, 342, 486));
		animationTable_.push_back(ML::Box2D(1026, 0, 342, 486));

		ML::Box2D src = animationTable_[player.GetAnimationCount() / 7 % animationTable_.size()];
		ML::Box2D draw(-src.w / 2, -src.h / 2, -src.w, src.h);
		draw *= 0.35f;
		draw.Offset(player.pos);
		player.GetImagePlayer()->Draw(draw, src);
	}
}