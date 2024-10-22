//-------------------------------------------------------------------
//�R���g���[���[�o�^���
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SubscribeController.h"
#include  "Task_SubscribeControllerBG.h"
#include  "../Task_Game.h"
#include  "../sound.h"
#include  "../easing.h"

namespace SubscribeController
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�u~P�v
		playerNumLogo = DG::Image::Create("./data/image/PlayerNumber.png");

		//�L�����N�^�[
		playerImg[0] = DG::Image::Create("./data/image/game_otsan_pose_red.png");
		playerImg[1] = DG::Image::Create("./data/image/game_otsan_pose.png");
		playerImg[2] = DG::Image::Create("./data/image/game_otsan_pose_yellow.png");
		playerImg[3] = DG::Image::Create("./data/image/game_otsan_pose_green.png");

		//Start�{�^��
		startButton[0] = DG::Image::Create("./data/image/button/Double/xbox_button_start_icon.png");
		startButton[1] = DG::Image::Create("./data/image/button/Double/xbox_button_start_icon_outline.png");

		//A�{�^��
		AButton[0] = DG::Image::Create("./data/image/button/Double/xbox_button_color_a.png");
		AButton[1] = DG::Image::Create("./data/image/button/Double/xbox_button_color_a_outline.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		playerNumLogo.reset();
		playerImg->reset();
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
		render2D_Priority[1] = 0.f;
		TestFont = DG::Font::Create("�l�r �S�V�b�N", 40, 80);
		ge->players.clear();

		//�R���e�i�ɒǉ�
		inputs.push_back(ge->in1);
		inputs.push_back(ge->in2);
		inputs.push_back(ge->in3);
		inputs.push_back(ge->in4);

		//���^�X�N�̐���
		//�w�i
		//���X�N���[�������
		for (int i = 0; i < 3; ++i)
		{
			auto BG = SubscribeControllerBG::Object::Create(true);
			if (BG)
			{
				BG->pos.x = static_cast<float>(i * 1600);
			}
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{

		ge->KillAll_G("�R���g���[���[�o�^���");
		//���f�[�^���^�X�N���
		if (!ge->QuitFlag() && nextTaskCreate) {
			//�������p���^�X�N�̐���
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		easing::UpDate();//�K�{

		//�o�^����
		for (int i = 0; i < inputs.size(); ++i) {
			//���̃^�X�N��
			if (inputs[i]->GetState().ST.down && subscribeCnt > 0) {
				Kill();
			}

			//�o�^ 
			if (inputs[i]->GetState().B1.down) {
				if (!isPushButton[i]) {
					Subscribe(inputs[i], isPushButton[i], i);
				}
				//�C�[�W���O�J�n
				StartEasing(i);
			}
		}

		//�C�[�W���O����
		for (int i = 0; i < inputs.size(); ++i) {
			if (!isPushButton[i]) { continue; }
			if (easing::GetState("ReactionEnd" + to_string(i)) == easing::EQ_STATE::EQ_END) {
				easingPos[i] = 0.f;
			}
			else if (easing::GetState("Reaction" + to_string(i)) == easing::EQ_STATE::EQ_END) {
				easingPos[i] = easing::GetPos("ReactionEnd" + to_string(i));
			}
			else
			{
				easingPos[i] = easing::GetPos("Reaction" + to_string(i));
			}
		}
		++animCnt;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�v���C���[�ԍ�
		DrawPlayerNums();

		//�v���C���[
		DrawPlayers();

		//UI
		DrawUI();
	}
	//-------------------------------------------------------------------
	//�v���C���[�̓o�^����
	void  Object::Subscribe(XI::GamePad::SP controller, bool& isPush, const int& i)
	{
		isPush = true;
		ge->players.push_back(controller);
		controllerIndex[subscribeCnt] = i;
		++subscribeCnt;
	}
	//-------------------------------------------------------------------
	//�C�[�W���O�J�n
	void  Object::StartEasing(const int& index) const
	{
		easing::Set("Reaction" + to_string(index), easing::QUADOUT, 0, 50, 5, "ReactionEnd" + to_string(index));
		easing::Set("ReactionEnd" + to_string(index), easing::QUADOUT, 50, 0, 5);
		easing::Start("Reaction" + to_string(index));
	}

	//-------------------------------------------------------------------
	//�v���C���[�ԍ��܂Ƃߕ`��
	void  Object::DrawPlayerNums() const
	{
		//�e�[�u����
		const ML::Box2D srcTable[4] = {
			ML::Box2D(0, 0, 155, 105),ML::Box2D(155, 0, 192, 105), ML::Box2D(347, 0, 175, 105), ML::Box2D(522, 0, 193, 105)
		};

		//�e�[�u����
		const ML::Box2D drawTable[4] = {
			ML::Box2D(-78, -53, 155, 105),ML::Box2D(-96, -53, 192, 105),ML::Box2D(-88, -53, 175, 105), ML::Box2D(-97, -53, 193, 105)
		};

		//�v���C���[���`��
		for (int i = 0; i < ge->players.size(); ++i) {
			ML::Box2D draw = drawTable[i].OffsetCopy(320 + 400 * i, static_cast<int>(200 + easingPos[controllerIndex[i]]));
			res->playerNumLogo->Draw(draw, srcTable[i]);
		}
	}
	//-------------------------------------------------------------------
	//�v���C���[�܂Ƃߕ`��
	void  Object::DrawPlayers() const
	{
		//�v���C���[���`��
		for (int i = 0; i < ge->players.size(); ++i) {
			ML::Box2D draw(100 + 400 * i, 300, srcPlayer.w, srcPlayer.h);
			res->playerImg[i]->Draw(draw, srcPlayer);
		}
	}
	//-------------------------------------------------------------------
	//UI�`��
	void  Object::DrawUI() const
	{
		ML::Box2D textBox(550, 50, 1000, 100);
		string text = "�v���C���[�G���g���[";
		TestFont->Draw(textBox, text);

		DrawOperation();
	}
	//-------------------------------------------------------------------
	//UI�`��
	void  Object::DrawOperation() const
	{
		//�Q�[���J�n
		if (subscribeCnt > 0) {
			ML::Box2D textBox(800, 850, 1000, 100);
			string text = "�Q�[���J�n";
			TestFont->Draw(textBox, text);
		}

		//�Q�[���J�n�̃{�^��
		if (subscribeCnt > 0) {
			DrawButton(res->startButton[animCnt / 7 % 2], ML::Vec2(650.f, 820.f));
		}

		//�G���g���[
		{
			ML::Box2D textBox(800, 960, 1000, 100);
			string text = "�G���g���[";
			TestFont->Draw(textBox, text);
		}

		//�G���g���[�̃{�^��
		DrawButton(res->AButton[animCnt / 7 % 2], ML::Vec2(650.f, 930.f));
	}
	//-------------------------------------------------------------------
	//�{�^���`��
	void  Object::DrawButton(DG::Image::SP img, const ML::Vec2& pos) const
	{
		ML::Box2D src(0, 0, 128, 128);
		ML::Box2D draw = src.OffsetCopy(pos);
		img->Draw(draw, src);
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
	Object::Object() {}
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