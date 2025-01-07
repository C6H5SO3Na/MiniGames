//-------------------------------------------------------------------
//�v���C���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGame.h"
#include  "Task_TaxiGamePlayer.h"
#include  "Task_TaxiGamePlayerIdleState.h"
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
		imageClear = DG::Image::Create("./data/image/clearImage.png");

		//�{�^���̉摜(�A�j���[�V�������݂̂��ߓ񎟌��z��)
		imageButton[0][0] = DG::Image::Create("./data/image/button/default/xbox_button_color_a.png");
		imageButton[1][0] = DG::Image::Create("./data/image/button/default/xbox_button_color_a_outline.png");
		imageButton[0][1] = DG::Image::Create("./data/image/button/default/xbox_button_color_b.png");
		imageButton[1][1] = DG::Image::Create("./data/image/button/default/xbox_button_color_b_outline.png");
		imageButton[0][2] = DG::Image::Create("./data/image/button/default/xbox_button_color_x.png");
		imageButton[1][2] = DG::Image::Create("./data/image/button/default/xbox_button_color_x_outline.png");
		imageButton[0][3] = DG::Image::Create("./data/image/button/default/xbox_button_color_y.png");
		imageButton[1][3] = DG::Image::Create("./data/image/button/default/xbox_button_color_y_outline.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		imageClear.reset();
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
		nowButton = GetRandom(0, 3);

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
		imagePlayer.reset();

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
		++animationCount;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		state->render(*this);
	}
	//-------------------------------------------------------------------
	//�v�l
	void  Object::Think()
	{
		state->think(*this);
	}
	//-------------------------------------------------------------------
	//�s��
	void  Object::Move()
	{
		state->move(*this);
	}
	//-------------------------------------------------------------------
	//�󂯐g
	void  Object::Received()
	{
	}
	//-------------------------------------------------------------------
	//�{�^������v�����Ƃ��̏���
	void  Object::MatchedButton()
	{
		//�����蔻��ł͂Ȃ��񐔂Ŕ���
		++matchCount;
		if (matchCount >= 10) {
			isClear = true;
			return;
		}
		nowButton = GetRandom(0, 3);
	}
	//-------------------------------------------------------------------
	//�{�^���̕`��
	void  Object::DrawButton() const
	{
		//�`���`
		ML::Box2D src(0, 0, 64, 64);
		ML::Box2D draw(-src.w / 2, -src.h / 2, src.w, src.h);
		draw.Offset(pos + ML::Vec2(-30.f, 60.f));
		res->imageButton[animationCount / 10 % 2][nowButton]->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//�N���A���b�Z�[�W�`��
	void  Object::DrawClearMessage() const
	{
		ML::Box2D src(0, 0, 97, 25);
		ML::Box2D draw(-src.w / 2, -src.h / 2, src.w, src.h);
		draw.Offset(pos);
		res->imageClear->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//�i�K��ύX
	void  Object::ChangeState(StateBase* state_)
	{
		delete state;
		state = state_;
		moveCnt = 0;
		prePosition = pos;
	}
	//-------------------------------------------------------------------
	//�{�^��(���͂��r�b�g�œ���)
	int  Object::GetButtonNum(int state)
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

		//�r�b�g��or�œ���
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
		ge->AddScore(controllerNum - 1, score);
		--score;
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
	Object::Object() :state(new IdleState()), isClear(false) {	}
	//-------------------------------------------------------------------
	void Object::Spawn(const ML::Vec2& pos_, XI::GamePad::SP controller_, const int& controllerNum_)
	{
		auto player = Create(true);
		player->pos = pos_;
		player->controller = controller_;
		//�R���g���[���[�̔ԍ����擾
		player->controllerNum = controllerNum_ + 1;
		//�v���C���[�摜
		player->imagePlayer = DG::Image::Create(player->imgPlayerPath[controllerNum_]);

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