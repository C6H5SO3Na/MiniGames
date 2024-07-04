//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_hand.h"
#include  "Task_Clock.h"

namespace  hand
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/hand.png");
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
		this->render2D_Priority[1] = -0.6f;
		this->hitBase = ML::Box2D(-128, -128, 256, 256);
		this->pos.x = 200;
		this->pos.y = 200;
		this->speed = 5.0f;
		this->controller = ge->in1;
		this->state = State::Right;

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
		switch (this->state)
		{
		case State::Left:
			this->moveVec = ML::Vec2(-2 * this->speed, 0);
			if (this->pos.x < 100)
			{
				this->state = State::Right;
			}
			break;
		case State::Right:
			this->moveVec = ML::Vec2(2 * this->speed, 0);
			if (this->pos.x >1800)
			{
				this->state = State::Left;
			}
			break;
		case State::Down:
			this->moveVec = ML::Vec2(0, 2 * this->speed);
			if (this->pos.y > 550)
			{
				this->state = State::Up;
			}
			break;
		case State::Up:
			this->moveVec = ML::Vec2(0, -2 * this->speed);
			if (this->pos.y < 200)
			{
				this->state = State::Right;
			}
			break;
		}

		this->pos += this->moveVec;

		if (this->controller) {
			auto inp = this->controller->GetState();
			if (inp.LStick.BD.down) { state = State::Down; }
		}
		
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
		auto t = ge->GetTask<Clock::Object>("�ڊo�܂����v");
		auto you = t->hitBase.OffsetCopy(t->pos);
			if (you.Hit(me))
			{
				this->speed = 0;
			}		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src(0, 0, 256, 256);
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