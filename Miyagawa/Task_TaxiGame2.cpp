//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TaxiGame2.h"

#include "../randomLib.h"

#include  "../Task_Ending.h"
#include  "Task_TaxiGamePlayer2.h"
#include  "Task_TaxiGameTaxi.h"

namespace  TaxiGame2
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


		//���^�X�N�̐���
		TaxiGamePlayer2::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 1 / 5.f), ge->in1);
		TaxiGamePlayer2::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 2 / 5.f), ge->in2);
		TaxiGamePlayer2::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 3 / 5.f), ge->in3);
		TaxiGamePlayer2::Object::Spawn(ML::Vec2(ge->screenWidth - 100.f, ge->screenHeight * 4 / 5.f), ge->in4);

		for (int i = 0; i < 4; ++i) {
			TaxiGameTaxi::Object::Spawn(ML::Vec2(200.f, ge->screenHeight * (1 + i) / 5.f));
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�{��");
		ge->KillAll_G("�v���C���[");

		if (!ge->QuitFlag() && nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//auto inp = ge->in1->GetState( );
		//if (inp.ST.down) {
		//	ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
		//	ge->CreateEffect(99, ML::Vec2(0, 0));
		//}
		//if (ge->getCounterFlag("test") == ge->LIMIT) {
		//	Kill();
		//}
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