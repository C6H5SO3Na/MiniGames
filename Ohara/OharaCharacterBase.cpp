//-----------------------------------------------------------------------------
//���������삵���L�����N�^�[�̊��N���X
//-----------------------------------------------------------------------------
#include "OharaCharacterBase.h"

//��ԍX�V���̏���
void OCharaBase::UpdateState(State nowState)
{
	if (nowState != this->state)
	{
		this->state = nowState;		//��ԕύX
		this->moveCnt = 0;			//�s���J�E���g�̃N���A
		this->animationCount = 0;	//�A�j���[�V�����J�E���g�̃N���A
	}
}
