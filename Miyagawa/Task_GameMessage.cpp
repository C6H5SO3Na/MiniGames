//-------------------------------------------------------------------
//�Q�[���̍ŏ��ɏo��w��
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_GameMessage.h"
#include  "../sound.h"
#include  "../easing.h"

namespace GameMessage
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
		render2D_Priority[1] = 0.01f;

		pos = ML::Vec2(3000, ge->screen2DHeight / 2.f);

		easing::Set("Start", easing::CIRCOUT, static_cast<float>(ge->screen2DWidth + src.w), ge->screen2DWidth / 2.f, 60, "End");
		easing::Set("End", easing::CIRCIN, ge->screen2DWidth / 2.f, static_cast<float>(-src.w), 60);

		//���C�[�W���O�J�n
		easing::Start("Start");

		se::LoadFile("default", "./data/sound/se/Common/�����J�n�̃S���O.wav");

		se::LoadFile("FinishSE", "./data/sound/se/Common/�����I���̃S���O.wav");
		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		if (!ge->QuitFlag() && nextTaskCreate) {
			//�������p���^�X�N�̐���
			img.reset();
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (!isPlayedSE) {
			se::Play(SEName);
			isPlayedSE = true;
		}

		//���C�[�W���O�ō��W�ړ�
		//Ready�𓮂���
		pos.x = easing::GetPos("Start");
		if (easing::GetState("Start") == easing::EQ_STATE::EQ_END) //�C�[�W���O�uGameRuleStart�v���I�������
		{
			pos.x = easing::GetPos("End");
		}
		//�C�[�W���O�����S�I��������^�X�N������
		if (easing::GetState("End") == easing::EQ_STATE::EQ_END) //�C�[�W���O�uGameRuleEnd�v���I�������
		{
			Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw;

		draw = ML::Box2D(-src.w, -src.h, src.w * 2, src.h * 2);
		draw.Offset(pos);

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
	Object::SP Object::Create(DG::Image::SP img, const ML::Box2D& src, const string& SEName)
	{
		auto logo = Create(true);
		logo->img = img;
		logo->src = src;
		logo->SEName = SEName;
		return logo;
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