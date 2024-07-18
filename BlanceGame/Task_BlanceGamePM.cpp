//-------------------------------------------------------------------
//�o�����X�Q�[���v���C���}�l�[�W���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_BlanceGame.h"
#include  "Task_BlanceGamePM.h"
#include  "Task_BlanceGamePlayer.h"
#include  "BGBChara.h"

namespace  BlanceGamePM
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
		this->res = Resource::Create();

		//���f�[�^������
		cList.push_back(ge->in1);
		cList.push_back(ge->in2);
		cList.push_back(ge->in3);
		cList.push_back(ge->in4);
		//���^�X�N�̐���

		for (int i = 0; i < 4; ++i) {
			auto p = BGPlayer::Object::Create(true);
			pList.push_back(p);
			pList[i]->pos=ML::Vec2(200 + 400*i, 500);
			pList[i]->controller = cList[i];
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("BGPlayer");

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
			auto p = ge->GetTasks<BGPlayer::Object>("BGPlayer");
			int r = rand() % 2;
			switch (r)
			{
			case 0:
				for (int i = 0; i < 4; ++i) {
					if (pList[i]->GetBGState() != BGBChara::BGstate::Fail) {
						pList[i]->SetBGState(BGBChara::BGstate::PlayL);
					}
				}
				break;
			case 1:
				for (int i = 0; i < 4; ++i) {
					if (pList[i]->GetBGState() != BGBChara::BGstate::Fail) {
						pList[i]->SetBGState(BGBChara::BGstate::PlayR);
					}
				}
				break;
			default:
				break;
			}
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