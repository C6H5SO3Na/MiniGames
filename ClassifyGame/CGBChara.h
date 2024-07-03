#pragma once
#include "../BChara.h"
class CGBChara :public BChara
{
	
public:
	enum CGstate {
		CStart,
		Start,
		Playing,
		PlayR,
		PlayG,
		PlayB,
		Fail,
		GameOver
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
