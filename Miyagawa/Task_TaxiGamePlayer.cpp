//-------------------------------------------------------------------
//�v���C���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGame.h"
#include  "Task_TaxiGamePlayer.h"
#include "../randomLib.h"
#include "../easing.h"
#include "../sound.h"

namespace TaxiGamePlayer
{
	Resource::WP  Resource::instance;
	int Object::playerScore;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�N���A�摜
		imgClear = DG::Image::Create("./data/image/clearImage.png");

		//�{�^���̉摜(�A�j���[�V�������݂̂��ߓ񎟌��z��)
		imgBtn[0][0] = DG::Image::Create("./data/image/button/default/xbox_button_color_a.png");
		imgBtn[1][0] = DG::Image::Create("./data/image/button/default/xbox_button_color_a_outline.png");
		imgBtn[0][1] = DG::Image::Create("./data/image/button/default/xbox_button_color_b.png");
		imgBtn[1][1] = DG::Image::Create("./data/image/button/default/xbox_button_color_b_outline.png");
		imgBtn[0][2] = DG::Image::Create("./data/image/button/default/xbox_button_color_x.png");
		imgBtn[1][2] = DG::Image::Create("./data/image/button/default/xbox_button_color_x_outline.png");
		imgBtn[0][3] = DG::Image::Create("./data/image/button/default/xbox_button_color_y.png");
		imgBtn[1][3] = DG::Image::Create("./data/image/button/default/xbox_button_color_y_outline.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		imgClear.reset();
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
		nowBtn = GetRandom(0, 3);
		
		//SE
		se::LoadFile("Miss", "./data/sound/se/ClassifyGame/maou_se_onepoint33.wav");
		se::LoadFile("Clear", "./data/sound/se/TaxiGame/clear.wav");
		se::LoadFile("Walk", "./data/sound/se/TaxiGame/footstep.wav");

		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		imgPlayer.reset();

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
		++animCnt;
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
	void  Object::Received()
	{
	}
	//-------------------------------------------------------------------
	//�{�^������v�����Ƃ��̏���
	void  Object::MatchButton()
	{
		++matchCnt;
		if (matchCnt >= 10) {
			isClear = true;
			return;
		}
		nowBtn = GetRandom(0, 3);
	}
	//-------------------------------------------------------------------
	//�{�^���̕`��
	void  Object::DrawButton() const
	{
		//�`���`
		ML::Box2D src(0, 0, 64, 64);
		ML::Box2D draw(-src.w / 2, -src.h / 2, src.w, src.h);
		draw.Offset(pos + ML::Vec2(-30.f, 60.f));
		res->imgBtn[animCnt / 10 % 2][nowBtn]->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//�N���A���b�Z�[�W�`��
	void  Object::DrawClearMessage() const
	{
		ML::Box2D src(0, 0, 97, 25);
		ML::Box2D draw(-src.w / 2, -src.h / 2, src.w, src.h);
		draw.Offset(pos);
		res->imgClear->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//�v�l
	void  Object::IdleState::think()
	{
		if (ge->gameState != MyPG::MyGameEngine::GameState::Game) {
			return;
		}
		if (owner_->BUTTON(0) == 0) { return; }
		if (owner_->BUTTON(0) == pow(2, 4 + owner_->nowBtn)) {//�r�b�g�P�ʂ̂��߂̌v�Z
			easing::Set("move" + to_string(owner_->controllerNum), easing::QUADINOUT, 0, -150, 60);
			easing::Start("move" + to_string(owner_->controllerNum));
			owner_->ChangeState(new MoveState(owner_));
			se::Play("Walk");
		}
		else {
			owner_->ChangeState(new MissState(owner_));
			se::Play("Miss");
		}
	}
	//-------------------------------------------------------------------
	//�s��
	void  Object::IdleState::move()
	{
	}
	//-------------------------------------------------------------------
	//�`��
	void  Object::IdleState::render()
	{
		//�v���C���`��
		ML::Box2D src(0, 0, 342, 486);
		ML::Box2D draw(-src.w / 2, -src.h / 2, -src.w, src.h);
		draw *= 0.35f;
		draw.Offset(owner_->pos);
		owner_->imgPlayer->Draw(draw, src);

		if (ge->gameState == MyPG::MyGameEngine::GameState::Game) {
			owner_->DrawButton();
		}

	}

	//-------------------------------------------------------------------
	//�v�l
	void  Object::MoveState::think()
	{
		if (easing::GetState("move" + to_string(owner_->controllerNum)) == easing::EQ_STATE::EQ_END) {
			owner_->MatchButton();
			if (owner_->isClear) {
				owner_->AddScore(playerScore, owner_->controller);
				owner_->ChangeState(new ClearState(owner_));
				se::Play("Clear");
				return;
			}
			owner_->ChangeState(new IdleState(owner_));
		}
	}
	//-------------------------------------------------------------------
	//�s��
	void  Object::MoveState::move()
	{
		owner_->pos.x = easing::GetPos("move" + to_string(owner_->controllerNum)) + owner_->prePos.x;
	}
	//-------------------------------------------------------------------
	//�`��
	void  Object::MoveState::render()
	{
		ML::Box2D animTable[] = {
			ML::Box2D(342, 0, 342, 486),
			ML::Box2D(684, 0, 342, 486),
			ML::Box2D(1026, 0, 342, 486),
		};
		ML::Box2D src = animTable[owner_->animCnt / 7 % size(animTable)];
		ML::Box2D draw(-src.w / 2, -src.h / 2, -src.w, src.h);
		draw *= 0.35f;
		draw.Offset(owner_->pos);
		owner_->imgPlayer->Draw(draw, src);
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
		//�v���C���[�`��
		ML::Box2D src(0, 0, 342, 486);
		ML::Box2D draw(-src.w / 2, -src.h / 2, -src.w, src.h);
		draw *= 0.35f;
		draw.Offset(owner_->pos);
		owner_->imgPlayer->Draw(draw, src);

		//�N���A���b�Z�[�W�`��
		owner_->DrawClearMessage();
	}

	//-------------------------------------------------------------------
	//�v�l
	void  Object::MissState::think()
	{
		if (owner_->moveCnt == 120) {
			owner_->ChangeState(new IdleState(owner_));
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
		//�`���`
		ML::Box2D src(0, 0, 342, 486);
		ML::Box2D draw(-src.w / 2, -src.h / 2, -src.w, src.h);
		draw *= 0.35f;
		draw.Offset(owner_->pos);
		//�U��
		draw.Offset(GetRandom(0, 10), GetRandom(0, 10));
		owner_->imgPlayer->Draw(draw, src, ML::Color(1.f, 1.f, 0.5f, 0.5f));

		owner_->DrawButton();
	}
	//-------------------------------------------------------------------
	//�i�K��ύX
	void  Object::ChangeState(StateBase* state_)
	{
		delete state;
		state = state_;
		moveCnt = 0;
		prePos = pos;
	}
	//-------------------------------------------------------------------
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
	//-------------------------------------------------------------------
	//�X�R�A���Z
	void Object::AddScore(int& score, XI::GamePad::SP con)
	{
		if (con == ge->in1) {
			ge->AddScore(0, score);
		}
		if (con == ge->in2) {
			ge->AddScore(1, score);
		}
		if (con == ge->in3) {
			ge->AddScore(2, score);
		}
		if (con == ge->in4) {
			ge->AddScore(3, score);
		}
		--score;
	}
	//-------------------------------------------------------------------
	//�R���g���[���[�ԍ����擾
	int Object::GetControllerNum(XI::GamePad::SP con) const
	{
		int rtv = 0;
		if (con == ge->in1) {
			rtv = 1;
		}
		else if (con == ge->in2) {
			rtv = 2;
		}
		else if (con == ge->in3) {
			rtv = 3;
		}
		else if (con == ge->in4) {
			rtv = 4;
		}
		return rtv;
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
	Object::Object() :state(new IdleState(this)), isClear(false) {	}
	//-------------------------------------------------------------------
	void Object::Spawn(const ML::Vec2& pos_, XI::GamePad::SP controller_, const int& controllerNum_)
	{
		auto player = Create(true);
		player->pos = pos_;
		player->controller = controller_;
		//�R���g���[���[�̔ԍ����擾
		player->controllerNum = player->GetControllerNum(player->controller);
		//�v���C���[�摜
		player->imgPlayer = DG::Image::Create(player->imgPlayerPath[controllerNum_]);

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