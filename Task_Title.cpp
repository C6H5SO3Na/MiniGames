//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include "randomLib.h"
#include  "Task_Effect00.h"
#include "sound.h"
#include "easing.h"

#include  "Task_Game.h"

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
		controller = ge->in1;
		render2D_Priority[1] = 0.5f;
		this->Cnt = 0;
		int RenderTime = 0;

		ge->stage = 1;

		easing::Set("titleY", easing::BACKOUT, -250, ge->screen2DHeight / 5, 200);
		easing::Start("titleY");


		//�f�o�b�O�p�t�H���g�̏���
		TestFont = DG::Font::Create("�l�r �S�V�b�N", 30, 30);

		//���y�t�@�C���̓ǂݍ���
		//BGM
		// BGM�̗��p����t�@�C���͋Ȃ̒����ɂ���ăT�C�Y���ς��܂��B
		// ��ʓI��Wav�͒��g���g�`�f�[�^�ƌĂ΂��T�C�Y���傫�ȃf�[�^�ɂ�܂��B
		// ���mp3�̓l�b�g�Ȃǂł̈�����z�肵�����k�`���ł��B
		// BGM��p����ꍇ��mp3�`���̃t�@�C���𗘗p���܂��傤�B
		// ���Ȃ݂ɂ��̃T���v���̃t�@�C����
		// �Đ�����1�F30�قڂ̋Ȃ�
		// mp3 = 4.3MB wav = 19MB�ł��B
		//bgm::LoadFile("bgm1", "./data/sound/bgm/�e�ރL���`_2.mp3");
		//bgm::Play("bgm1");

		//se
		// se�͌��ʉ��ł��B
		// bgm�Ƃ̍ő�̈Ⴂ�͂ЂƂ̉��y�t�@�C���̓����Đ��ɑΉ����Ă��邱�Ƃł��B
		// se��wav�t�@�C�������������Ƃ��o���܂���Bmp3�̓G���[�ɂȂ�܂��B
		// �����Đ��̕K�v���Ȃ����̂�BGM
		// �����Đ����K�v�Ȃ��̂�se�Ǝg�������Ă��������B
	  // �܂����̃T���v���̂悤�ȓ��{��t�@�C�����͂�߂������ǂ��ł��B
		// �ǂݍ��݃G���[�̌��ɂȂ�܂��B
		//se::LoadFile("se1","./data/sound/se/�u�������������v.wav");

//		se::LoadFile("se2", "./data/sound/se/�u�撣�����ˁv.mp3");����̓G���[ wav����Ȃ��ƃ_��
		//se::LoadFile("se2", "./data/sound/se/�u�撣�����ˁv.wav");

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
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = controller->GetState();

		this->Cnt++;

		int input = 0;
		//							16�i		2�i	
		if (inp.B1.down) input |= 0x0001;	// 0000000001
		if (inp.B2.down) input |= 0x0002;	// 0000000010
		if (inp.B3.down) input |= 0x0004;	// 0000000100
		if (inp.B4.down) input |= 0x0008;	// 0000001000
		if (inp.ST.down) input |= 0x0100;	// 0100000000

		if (input & 0x0100) {
			ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
			ge->CreateEffect(98, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			Kill();
		}

		easing::UpDate();
		logoY = easing::GetPos("titleY");

		return;

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		TestFont->Draw(ML::Box2D(0, 0, ge->screen2DWidth, ge->screen2DHeight),
			"TITLE"
		);

		ML::Box2D draw (0, 0, 1980, 1080);
		ML::Box2D src(0, 0, 1980, 1080);

		this->res->img->Draw(draw, src);

		ML::Box2D draw2(ge->screen2DWidth / 14.5, logoY, 1666, 530);
		ML::Box2D src2(0, 0, 1666, 530);
		this->res->TitleLogo01->Draw(draw2, src2);

		if (this->Cnt > 0)
		{
			if ((this->Cnt / 10) % 5 == 0)
			{
				return; //8�t���[����4�t���[���摜��\�����Ȃ�
			}
			ML::Box2D draw3(ge->screen2DWidth / 4.5, 750, 980, 234);
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