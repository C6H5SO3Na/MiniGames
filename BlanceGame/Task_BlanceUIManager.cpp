//-------------------------------------------------------------------
//�o�����X�Q�[���t�h�}�l�[�W���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_BlanceUIManager.h"
#include  "Task_BlanceGame.h"
#include  "Task_BlanceGamePM.h"

namespace  BlanceGUIM
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		backImg = DG::Image::Create("./data/image/train.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		backImg.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		render2D_Priority[1] = 0.5;
		pos = ML::Vec2{ 0,0 };
		SetBGState(BGstate::Start);
		for (int i = 0; i < 4; ++i) {
			failFlag[i] = false;
		}
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto bg = ge->GetTask<BlanceGame::Object>("�o�����X�Q�[��");
		if (!bg) {
			return;
		}
		if (bg->shake) {
			pos.x += 239;
		}
		else
		{
			pos.x -= 1;
		}
		auto pm = ge->GetTask<BlanceGamePM::Object>("�o�����X�Q�[��PM");
		for (int i = 0; i < 4; ++i) {
			if (pm->pList[i]->GetBGState() == BGstate::Fail) {
				failFlag[i] = true;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D src = ML::Box2D(0, 0, 1255, 1058);
		ML::Box2D draw = ML::Box2D(0, 0, 1255, 1058);
		draw = draw.OffsetCopy(pos);
		res->backImg->Draw(src, draw);
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
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
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
	Resource::~Resource() { this->Finalize(); }
}