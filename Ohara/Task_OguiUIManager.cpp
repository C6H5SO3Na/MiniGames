//-------------------------------------------------------------------
//��H���~�j�Q�[����UI�Ǘ�
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiUIManager.h"
#include  "Task_OguiGame.h"
#include  "Task_OguiPlayer.h"

namespace  OguiUIManager
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
		testFont = DG::Font::Create("�l�r �S�V�b�N", 30, 30);
		
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���H�ׂ��������̕`��
		//�v���C���[�S�Ă𒊏o����
		auto players = ge->GetTasks<OguiPlayer::Object>("�v���C���[");
		//�v���C���[�̐��������[�v����
		int loopCount = 0; //���[�v�����񐔂̃J�E���g
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			//�`��
			testFont->Draw(ML::Box2D(45 + ge->screen2DWidth * loopCount / 4, 65, ge->screen2DWidth, ge->screen2DHeight),
				to_string((int)(*p)->playerNum) + "P:" + to_string((*p)->eatFoodCount)
			);
			//���[�v�񐔂̃J�E���g
			++loopCount;
		}

		//���������Ԃ̕`��
		//�T�{��Q�[���̓����̏����擾
		auto game = ge->GetTask<OguiGame::Object>("�{��");
		//�`��
		testFont->Draw(ML::Box2D(ge->screen2DWidth / 2, 0, ge->screen2DWidth, ge->screen2DHeight),
			to_string(game->timeLimit)
		);
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