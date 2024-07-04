#pragma once
#include "../BChara.h"
class CGBChara :public BChara
{
	
public:
	CGBChara() :
		cgState(CGstate::BStart)
	{}
	enum CGstate {
		BStart, //BeforeStart
		Start,	//�n�܂�
		Playing,//�v���C��
		PlayR,	//�Ԃ̏���
		PlayG,	//�΂̏���
		PlayB,	//�̏���
		Fail,	//���s
		GameOver//�Q�[���I��
	};
	CGstate GetCGState() {
		return this->cgState;
	}
	void SetCGState(CGstate c) {
		this->cgState = c;
	}
private:
	CGstate cgState;
};
