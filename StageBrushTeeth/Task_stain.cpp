//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../randomLib.h"
#include  "../MyPG.h"
#include  "Task_stain.h"
#include  "Task_StainManager.h"
#define _USE_MATH_DEFINES
#include  "math.h"
#include "algorithm"

namespace  stain
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/game_dirt.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = -0.5f;
		this->pos.x = 0;
		this->pos.y = 0;
		this->hitBase = ML::Box2D(-20, -20, 40, 40);
		this->timeCnt = 0;
		this->animCnt = 0;
		this->animIndex = 0;
		this->speed = GetRandom(1.0f, 5.0f);
		dx = 0;
		dy = 0;
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->timeCnt++;
		this->animCnt++;
		//�A�j���X�V
		if (this->animCnt >= 5)
		{
			this->animCnt = 0;
			this->animIndex++;
			if (this->animIndex >= 3)
			{
				this->animIndex = 0;
			}
		}

		//�����_���Ő^��������܂�œ���
		if (rand() % 100 < 20) { // State�ς�道
			State = (State == MovementState::Forward) ? MovementState::Circular : MovementState::Forward;
		}

		switch (ge->gameState)
		{
		case MyPG::MyGameEngine::GameState::Start:
			if (State == MovementState::Forward) {
				//�����_���œ���
				//float direction = GetRandom(0.f, 2 * (float)M_PI); // ���W�A���ŕ������v�Z����
				//dx = speed * cos(direction);
				//dy = speed * sin(direction);
				float dx = (GetRandom(-1, 1) * speed); //�E��
				float dy = (GetRandom(-1, 1) * speed); //�㉺
			}
			else if (State == MovementState::Circular) {
				//�܂�œ���t
				float radius = GetRandom(1.f, 5.f);
				angle += 0.1f;
				dx = radius * cos(angle);
				dy = radius * sin(angle);
			}

			pos.x += dx;
			pos.y += dy;

			this->pos.x = max(minX, min(this->pos.x, maxX));
			this->pos.y = max(minY, min(this->pos.y, maxY));
			break;
		}

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src(animIndex * 256, 0, 256, 256);

		this->res->img->Draw(draw, src);
	}


	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}