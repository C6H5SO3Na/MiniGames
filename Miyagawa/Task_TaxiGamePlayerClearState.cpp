//-------------------------------------------------------------------
//プレイヤークリア状態
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include "../easing.h"
#include "../sound.h"
#include "Task_TaxiGamePlayerClearState.h"

namespace TaxiGamePlayer
{
	//-------------------------------------------------------------------
	//思考
	void  ClearState::think(Object& player)
	{
	}
	//-------------------------------------------------------------------
	//行動
	void  ClearState::move(Object& player)
	{
	}
	//-------------------------------------------------------------------
	//描画
	void  ClearState::render(Object& player)
	{
		//プレイヤー描画
		ML::Box2D src(0, 0, 342, 486);
		ML::Box2D draw(-src.w / 2, -src.h / 2, -src.w, src.h);
		draw *= 0.35f;
		draw.Offset(player.pos);
		player.GetImagePlayer()->Draw(draw, src);

		//クリアメッセージ描画
		player.DrawClearMessage();
	}
}