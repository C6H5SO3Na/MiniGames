//-----------------------------------------------------------------------------
//���������삵���L�����N�^�[�̊��N���X
//-----------------------------------------------------------------------------
#include "OharaCharacterBase.h"

//��ԍX�V���̏���
void OCharaBase::UpdateState(State nowState)
{
	if (nowState != this->state)
	{
		this->state = nowState;
		this->moveCnt = 0;
	}
}
