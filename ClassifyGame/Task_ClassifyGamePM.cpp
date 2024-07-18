//-------------------------------------------------------------------
//�o�����X�Q�[��
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ClassifyGamePM.h"
#include  "Task_ClassifyGamePlayer.h"
#include  "CGBChara.h"

namespace  ClassifyGamePM
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
		gameCnt = 0;
		cList.push_back(ge->in1);
		cList.push_back(ge->in2);
		cList.push_back(ge->in3);
		cList.push_back(ge->in4);
		//���^�X�N�̐���

		for (int i = 0; i < 4; ++i) {
			auto p = CGPlayer::Object::Create(true);
			pList.push_back(p);
			pList[i]->posInitialize(ML::Vec2(450 + 960 * (i % 2), 400 + 540 * (i / 2)));
			pList[i]->controller = cList[i];
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("CGPlayer");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		gameCnt++;
		if (gameCnt == 1320) {
			int winner = 0;
			for (int i = 0; i < 3; ++i) {
					if(pList[i+1]->Fb> pList[i]->Fb){
						winner = i + 1;
				}
			}
			this->Kill();
		}
		else if (gameCnt % 120 == 0) {
			int r = rand() % 3;
			switch (r)
			{
			case 0:
				for (int i = 0; i < 4; ++i) {
					if (pList[i]->GetCGState() != CGBChara::CGstate::Fail) {
						pList[i]->SetCGState(CGBChara::CGstate::PlayR);
					}
				}
				break;
			case 1:
				for (int i = 0; i < 4; ++i) {
					if (pList[i]->GetCGState() != CGBChara::CGstate::Fail) {
						pList[i]->SetCGState(CGBChara::CGstate::PlayG);
					}
				}
				break;
			case 2:
				for (int i = 0; i < 4; ++i) {
					if (pList[i]->GetCGState() != CGBChara::CGstate::Fail) {
						pList[i]->SetCGState(CGBChara::CGstate::PlayB);
					}
				}
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