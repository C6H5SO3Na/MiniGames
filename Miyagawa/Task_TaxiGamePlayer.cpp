//-------------------------------------------------------------------
//�v���C���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGame.h"
#include  "Task_TaxiGamePlayer.h"
#include "../randomLib.h"

namespace TaxiGamePlayer
{
	Resource::WP  Resource::instance;
	int Object::clearNum;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/chara02.png");
		imgBtn[0][0] = DG::Image::Create("./data/image/button/default/xbox_button_a.png");
		imgBtn[1][0] = DG::Image::Create("./data/image/button/default/xbox_button_a_outline.png");
		imgBtn[0][1] = DG::Image::Create("./data/image/button/default/xbox_button_b.png");
		imgBtn[1][1] = DG::Image::Create("./data/image/button/default/xbox_button_b_outline.png");
		imgBtn[0][2] = DG::Image::Create("./data/image/button/default/xbox_button_x.png");
		imgBtn[1][2] = DG::Image::Create("./data/image/button/default/xbox_button_x_outline.png");
		imgBtn[0][3] = DG::Image::Create("./data/image/button/default/xbox_button_y.png");
		imgBtn[1][3] = DG::Image::Create("./data/image/button/default/xbox_button_y_outline.png");
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
		nowBtn = GetRandom(0, static_cast<int>(size(btn)) - 1);
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
		pos.x -= 150.f;
		if (matchCnt >= 10) {
			isClear = true;
			return;
		}
		nowBtn = GetRandom(0, static_cast<int>(size(btn)) - 1);
	}
	//-------------------------------------------------------------------
	//�v�l
	void  Object::NormalState::think()
	{
		if (owner_->isClear) {
			owner_->ChangeState(new ClearState(owner_));
			owner_->PullClear(clearNum, owner_->controller);
			return;
		}
		if (owner_->BUTTON(0) == 0) { return; }
		if (owner_->BUTTON(0) != pow(2, 4 + owner_->nowBtn)) {
			owner_->ChangeState(new MissState(owner_));
		}
	}
	//-------------------------------------------------------------------
	//�s��
	void  Object::NormalState::move()
	{
		if (owner_->BUTTON(0) == pow(2, 4 + owner_->nowBtn)) {
			owner_->MatchButton();
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

	//-------------------------------------------------------------------
	//�v�l
	void  Object::MissState::think()
	{
		if (owner_->moveCnt == 120) {
			owner_->ChangeState(new NormalState(owner_));
		}
	}
	//-------------------------------------------------------------------
	//�s��
	void  Object::MissState::move()
	{
		++owner_->moveCnt;
	}
	//-------------------------------------------------------------------
	//�`��
	void  Object::MissState::render()
	{
		{
			//�`���`
			ML::Box2D src(176, 0, 48, 80);

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
	//�i�K��ύX
	void  Object::ChangeState(StateBase* state_)
	{
		delete state;
		state = state_;
		moveCnt = 0;
	}

	//�{�^��(���͂��r�b�g�œ���)
	int  Object::BUTTON(int state)
	{
		//���ꂼ��̃{�^������
		struct Input {
			bool A = false;
			bool B = false;
			bool X = false;
			bool Y = false;
		};
		Input inp[4] = {
			{input.B1.down, input.B2.down, input.B3.down, input.B4.down},
			{input.B1.on, input.B2.on, input.B3.on, input.B4.on},
			{input.B1.up, input.B2.up, input.B3.up, input.B4.up},
			{input.B1.off, input.B2.off, input.B3.off, input.B4.off}
		};

		int rtv = 0;
		if (inp[state].A) {
			rtv |= A;
		}
		if (inp[state].B) {
			rtv |= B;
		}
		if (inp[state].X) {
			rtv |= X;
		}
		if (inp[state].Y) {
			rtv |= Y;
		}
		return rtv;
	}

	//�N���A��ʒm
	void Object::PullClear(int& n, XI::GamePad::SP con)
	{
		auto game = ge->GetTask<TaxiGame::Object>(TaxiGame::defGroupName);

		if (con == ge->in1) {
			game->playerRank[0] = n;
		}
		if (con == ge->in2) {
			game->playerRank[1] = n;
		}
		if (con == ge->in3) {
			game->playerRank[2] = n;
		}
		if (con == ge->in4) {
			game->playerRank[3] = n;
		}
		++n;
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