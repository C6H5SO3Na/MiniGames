//-----------------------------------------------------------------------------
//小原が制作したキャラクターの基底クラス
//-----------------------------------------------------------------------------
#include "OharaCharacterBase.h"

//状態更新時の処理
void OCharaBase::UpdateState(State nowState)
{
	if (nowState != this->state)
	{
		this->state = nowState;		//状態変更
		this->moveCnt = 0;			//行動カウントのクリア
		this->animationCount = 0;	//アニメーションカウントのクリア
	}
}
