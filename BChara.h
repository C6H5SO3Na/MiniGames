#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

class BChara : public BTask
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������������������������������
	//�L�����N�^���ʃ����o�ϐ�
	ML::Vec2    pos;		//�L�����N�^�ʒu
	ML::Box2D   hitBase;	//�����蔻��͈�
	ML::Vec2	moveVec;	//�ړ��x�N�g��
	int			moveCnt;	//�s���J�E���^
	//�����i2D���_�j
	float angle;

	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����鎖����
	BChara()
		: pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, moveVec(0, 0)
		, moveCnt(0)
		, angle(0.f)
	{
	}
	virtual  ~BChara() {}

protected:
	ML::Vec2 MoveSet(int key);

	//�󂯐g�̏���
	virtual void Received() = 0;
	//�����蔻��擾
	ML::Box2D GetHitBase() const { return hitBase; }
	//���W�擾
	ML::Vec2 GetPos() const { return pos; }

	//�����蔻��
	bool  CheckHit(BChara::SP ptr);//ge->GetTask<>()
	bool  CheckHit(shared_ptr<vector<BChara::SP>> ptr);//ge->GetTasks<>()
};