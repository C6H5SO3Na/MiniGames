//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGamePlayer.h"
#include "../randomLib.h"

namespace TaxiPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
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
	//�󂯐g
	void  Object::MatchButton()
	{
		++matchCnt;
		nowBtn = GetRandom(0, 3);
	}
	//-------------------------------------------------------------------
	//�v�l
	void  Object::NormalState::think()
	{
		if (owner_->matchCnt >= 10) {
			owner_->ChangeState(new ClearState(owner_));
		}
	}
	//-------------------------------------------------------------------
	//�s��
	void  Object::NormalState::move()
	{
		if (owner_->input.B1.down) {
			if (owner_->nowBtn == 0) {
				owner_->MatchButton();
			}
		}
		if (owner_->input.B2.down) {
			if (owner_->nowBtn == 1) {
				owner_->MatchButton();
			}
		}
		if (owner_->input.B3.down) {
			if (owner_->nowBtn == 2) {
				owner_->MatchButton();
			}
		}
		if (owner_->input.B4.down) {
			if (owner_->nowBtn == 3) {
				owner_->MatchButton();
			}
		}
	}
	//-------------------------------------------------------------------
	//�`��
	void  Object::NormalState::render()
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
		//�`���`
		ML::Box2D draw(
			static_cast<int>(owner_->pos.x),
			static_cast<int>(owner_->pos.y),
			ge->screen2DWidth,
			ge->screen2DHeight
		);

		owner_->TestFont->Draw(draw, owner_->btn[owner_->nowBtn]);

		owner_->TestFont->Draw(ML::Box2D(draw.x - 100, draw.y - 100, draw.w, draw.h),
			"CLEAR!", ML::Color(1.f, 1.f, 0.f, 1.f)
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
	Object::Object() :state(new NormalState(this)) {	}
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