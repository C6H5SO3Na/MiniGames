//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include "randomLib.h"
#include  "Task_Effect00.h"
#include "sound.h"
#include "easing.h"

#include  "./Miyagawa/Task_SubscribeController.h"

#include  <chrono>
#include  <thread>

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/gametitle_bg.png");
		this->TitleLogo01 = DG::Image::Create("./data/image/gametitle_title.png");
		this->StartLogo = DG::Image::Create("./data/image/gametitle_start.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->TitleLogo01.reset();
		this->StartLogo.reset();

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

		//BGM
		bgm::LoadFile("Title_bgm", "./data/sound/bgm/title_yutarisannbo.mp3");
		bgm::Play("Title_bgm");

		//SE
		se::LoadFile("se_confirm", "./data/sound/se/Common/confirmSE.wav");


		//���f�[�^������
		inputs.push_back(ge->in1);
		inputs.push_back(ge->in2);
		inputs.push_back(ge->in3);
		inputs.push_back(ge->in4);

		render2D_Priority[1] = 0.5f;
		this->Cnt = 0;
		int RenderTime = 0;

		ge->stage = 1;

		easing::Set("titleY", easing::BACKOUT, -250, ge->screen2DHeight / 5.f, 200);
		easing::Start("titleY");


		//�f�o�b�O�p�t�H���g�̏���
		TestFont = DG::Font::Create("�l�r �S�V�b�N", 30, 30);

		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		bgm::Stop("Title_bgm");
		if (!ge->QuitFlag() && nextTaskCreate) {
			se::Play("se_confirm");
			std::this_thread::sleep_for(std::chrono::seconds(1));
			SubscribeController::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->Cnt++;

		//�S�ẴR���g���[���[�������
		for_each(inputs.begin(), inputs.end(), [](auto input) {
			if (input->GetState().B2.down) {
				//�C�[�W���O���̓C�[�W���O�X�L�b�v
				if (easing::GetState("titleY") == easing::EQ_STATE::EQ_START) {

					easing::Set("titleY", easing::LINEAR, -200, ge->screen2DHeight / 5.f, 1);
					easing::Start("titleY");
				}
				//�C�[�W���O���I�����Ă�����t�F�[�h
				else if (ge->getCounterFlag("test") != ge->ACTIVE) {
					ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
					ge->CreateEffect(98, ML::Vec2(0, 0));
				}
			}});
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			Kill();
		}

		easing::UpDate();
		logoY = easing::GetPos("titleY");

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		TestFont->Draw(ML::Box2D(0, 0, ge->screen2DWidth, ge->screen2DHeight),
			"TITLE"
		);

		ML::Box2D draw(0, 0, 1980, 1080);
		ML::Box2D src(0, 0, 1980, 1080);

		this->res->img->Draw(draw, src);

		ML::Box2D draw2(static_cast<int>(ge->screen2DWidth / 14.5f), static_cast<int>(logoY), 1666, 530);
		ML::Box2D src2(0, 0, 1666, 530);
		this->res->TitleLogo01->Draw(draw2, src2);

		if (easing::GetState("titleY") == easing::EQ_STATE::EQ_END)
		{
			if ((this->Cnt / 10) % 5 == 0)
			{
				return; //8�t���[����4�t���[���摜��\�����Ȃ�
			}
			ML::Box2D draw3(static_cast<int>(ge->screen2DWidth / 4.5f), 750, 980, 234);
			ML::Box2D src3(0, 0, 980, 234);
			this->res->StartLogo->Draw(draw3, src3);
		}

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
	Resource::~Resource() { Finalize(); }
}