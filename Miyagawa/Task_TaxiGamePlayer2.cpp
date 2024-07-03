//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGamePlayer2.h"
#include "../randomLib.h"

namespace TaxiGamePlayer2
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/chara02.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		res = Resource::Create();

		//���f�[�^������
		render2D_Priority[1] = 0.5f;
		TestFont = DG::Font::Create("�l�r �S�V�b�N", 30, 30);

		//���^�X�N�̐���
		nowBtn = GetRandom(0, 3);
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		input = controller->GetState();
		Think();
		Move();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		state->render();
	}
	//-------------------------------------------------------------------
	//�v�l
	void  Object::Think()
	{
		state->think();
	}
	//-------------------------------------------------------------------
	//�s��
	void  Object::Move()
	{
		state->move();
	}
	//-------------------------------------------------------------------
	//�󂯐g
	void  Object::Recieved()
	{
	}
	//-------------------------------------------------------------------
	//�{�^������v�����Ƃ��̏���
	void  Object::MatchButton()
	{
		++matchCnt;
		pos.y -= 150.f;
		if (matchCnt >= 10) {
			isClear = true;
			return;
		}
		nowBtn = GetRandom(0, 3);
	}
	//-------------------------------------------------------------------
	//�v�l
	void  Object::NormalState::think()
	{
		if (owner_->isClear) {
			owner_->ChangeState(new ClearState(owner_));
		}
	}
	//-------------------------------------------------------------------
	//�s��
	void  Object::NormalState::move()
	{
		if (owner_->input.B1.down) {
			if (owner_->nowBtn == A) {
				owner_->MatchButton();
			}
		}
		if (owner_->input.B2.down) {
			if (owner_->nowBtn == B) {
				owner_->MatchButton();
			}
		}
		if (owner_->input.B3.down) {
			if (owner_->nowBtn == X) {
				owner_->MatchButton();
			}
		}
		if (owner_->input.B4.down) {
			if (owner_->nowBtn == Y) {
				owner_->MatchButton();
			}
		}
	}
	//-------------------------------------------------------------------
	//�`��
	void  Object::NormalState::render()
	{
		{
			//�`���`
			ML::Box2D src(0, 0, 32, 80);

			ML::Box2D draw(
				-src.w,
				-src.h,
				-src.w * 2,
				src.h * 2
			);

			draw.Offset(owner_->pos);

			owner_->res->img->Draw(draw, src);
		}

		{
			//�`���`
			ML::Box2D draw(
				static_cast<int>(owner_->pos.x),
				static_cast<int>(owner_->pos.y),
				ge->screen2DWidth,
				ge->screen2DHeight
			);

			owner_->TestFont->Draw(draw, owner_->btn[owner_->nowBtn]);
		}
	}
	//-------------------------------------------------------------------
	//�v�l
	void  Object::ClearState::think()
	{
	}
	//-------------------------------------------------------------------
	//�s��
	void  Object::ClearState::move()
	{
	}
	//-------------------------------------------------------------------
	//�`��
	void  Object::ClearState::render()
	{
		{
			//�`���`
			ML::Box2D src(0, 0, 32, 80);

			ML::Box2D draw(
				-src.w,
				-src.h,
				-src.w * 2,
				src.h * 2
			);

			draw.Offset(owner_->pos);

			owner_->res->img->Draw(draw, src);
		}

		//�`���`
		ML::Box2D draw(
			static_cast<int>(owner_->pos.x) - 100,
			static_cast<int>(owner_->pos.y) - 100,
			ge->screen2DWidth,
			ge->screen2DHeight
		);

		owner_->TestFont->Draw(draw, "CLEAR!", ML::Color(1.f, 1.f, 0.f, 1.f)
		);
	}
	//�i�K��ύX
	void  Object::ChangeState(StateBase* state_)
	{
		delete state;
		state = state_;
		moveCnt = 0;
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
		return  Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() :state(new NormalState(this)), isClear(false) {	}
	//-------------------------------------------------------------------
	void Object::Spawn(const ML::Vec2& pos_, XI::GamePad::SP controller_)
	{
		auto player = Create(true);
		player->pos = pos_;
		player->controller = controller_;
	}
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
	Resource::~Resource() { Finalize(); }
}