#pragma once
#include "../BChara.h"
class BGBChara :public BChara
{
	
public:
	BGBChara():
		bgState(BGstate::BStart)
	{}
	enum BGstate {
		BStart, //BeforeStart
		Start,	//�n�܂�
		Playing,//�v���C��
		PlayR,	//�E�ɓ|��
		PlayL,	//���ɓ|��
		Fail,	//���s
		GameOver//�Q�[���I��
	};
	BGstate GetBGState() {
		return this->bgState;
	}
	void SetBGState(BGstate b) {
		this->bgState = b;
	}
private:
	BGstate bgState;
};
