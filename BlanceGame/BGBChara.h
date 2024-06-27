#pragma once
#include "../BChara.h"
class BGBChara :public BChara
{
	
public:
	enum BGstate {
		BStart,
		Start,
		Playing,
		PlayR,
		PlayL,
		Fail,
		GameOver
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
