//-------------------------------------------------------------------
//�Q�[���̍ŏ��ɏo��w��
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_EasingLogo.h"
#include  "Task_GameMessage.h"

namespace EasingLogo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img.push_back(DG::Image::Create("./data/image/SaboriGameRuleSentence.png"));
		img.push_back(DG::Image::Create("./data/image/bar.png"));
		img.push_back(DG::Image::Create("./data/image/bar.png"));
		img.push_back(DG::Image::Create("./data/image/bar.png"));
		img.push_back(DG::Image::Create("./data/image/SaboriGameRuleSentence.png"));
		img.push_back(DG::Image::Create("./data/image/OugiGameRuleSentence.png"));
		img.push_back(DG::Image::Create("./data/image/bar.png"));
		img.push_back(DG::Image::Create("./data/image/Finish.png"));
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		for (auto& i : img) {
			i.reset();
		}
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
		render2D_Priority[1] = -0.99f;

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
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (!isCreated) {
			if (isFinish) {
				logo = GameMessage::Object::Create(res->img[7], srcTable[7], "FinishSE");
			}
			else {
				logo = GameMessage::Object::Create(res->img[ge->nowStage], srcTable[ge->nowStage]);
			}
			isCreated = true;
		}
		if (logo.lock() == nullptr) {
			Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
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
	Object::Object():isFinish(false) {}
	//-------------------------------------------------------------------
	Object::SP Object::Spawn(const bool& isFinish)
	{
		auto logo = Create(true);
		logo->isFinish = isFinish;
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