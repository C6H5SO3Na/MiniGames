#pragma once
#include "../BChara.h"
class CGBChara :public BChara
{
	
public:
	CGBChara() :
		cgState(CGstate::BStart),
		firstFrameIntoState(false)
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
		this->firstFrameIntoState = true;
	}
	bool FirstIntoState() {//�n�߂Ă���STATE�ɓ��邱�Ƃ��`�F�b�N
		if (this->firstFrameIntoState == true)
		{
			this->firstFrameIntoState = false;
			return true;
		}
		return false;
	}
	
private:
	CGstate cgState;
	bool firstFrameIntoState;
};
