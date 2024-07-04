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
		Start,	//始まる
		Playing,//プレイ中
		PlayR,	//赤の書類
		PlayG,	//緑の書類
		PlayB,	//青の書類
		Fail,	//失敗
		GameOver//ゲーム終了
	};
	CGstate GetCGState() {
		return this->cgState;
	}
	void SetCGState(CGstate c) {
		this->cgState = c;
		this->firstFrameIntoState = true;
	}
	bool FirstIntoState() {//始めてこのSTATEに入ることをチェック
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
