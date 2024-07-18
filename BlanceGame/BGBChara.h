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
		Start,	//Žn‚Ü‚é
		Playing,//ƒvƒŒƒC’†
		PlayR,	//‰E‚É“|‚·
		PlayL,	//¶‚É“|‚·
		Fail,	//Ž¸”s
		GameOver//ƒQ[ƒ€I—¹
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
