//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"

ML::Vec2 BChara::MoveSet(int key)
{
	//0, 1, 2,  3,  4,  5,  6, 7, 8, 9,10,
	int table[] = { -1,90,270,-1,180,135,225,-1, 0,45,315 };
	float spd = 10.5f;
	ML::Vec2 est;
	//0は特殊条件
	if (key == 0)
	{
		est.x = 0.f;
		est.y = 0.f;
	}
	else
	{
		est.x = (float)cos(table[key] * D3DX_PI / 180.f) * spd;
		est.y = (float)-sin(table[key] * D3DX_PI / 180.f) * spd;
	}
	return est;
}
//-------------------------------------------------------------------
//当たり判定(単体)
bool  BChara::CheckHit(BChara::SP ptr)
{
	if (ptr == nullptr) { return false; }
	ML::Box2D me = this->GetHitBase().OffsetCopy(this->GetPos());
	ML::Box2D you = ptr->GetHitBase().OffsetCopy(ptr->GetPos());
	if (you.Hit(me)) {
		ptr->Received();
		return true;
	}
	return false;
}
//-------------------------------------------------------------------
//当たり判定(複数)
bool  BChara::CheckHit(shared_ptr<vector<BChara::SP>> iters)
{
	bool rtv = false;
	for_each(iters->begin(), iters->end(),
		[&](auto iter) {
			ML::Box2D me = this->GetHitBase().OffsetCopy(this->GetPos());
			ML::Box2D you = iter->GetHitBase().OffsetCopy(iter->GetPos());
			if (you.Hit(me)) {
				iter->Received();
				rtv = true;
				return;
			}
		});
	return rtv;
}