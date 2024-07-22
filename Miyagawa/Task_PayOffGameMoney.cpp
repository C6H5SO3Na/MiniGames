//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_PayOffGameMoney.h"
#include "../randomLib.h"

namespace PayOffGameMoney
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/money.png");
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
		render2D_Priority[1] = 0.9f;

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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//draw,src,color
		DrawInfo drawInfoTable[] =
		{
			{ {  0,   0, 128, 128}, {  0,   0, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {128,   0, 128, 128}, {128,   0, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {256,   0, 128, 128}, {256,   0, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {  0, 128, 128, 128}, {  0, 128, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {128, 128, 128, 128}, {128, 128, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {256, 128, 128, 128}, {256, 128, 128, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {  0, 256, 256, 128}, {  0, 256, 256, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {256, 256, 256, 128}, {256, 256, 256, 128}, {1.f, 1.f, 1.f, 1.f}},
			{ {  0, 384, 256, 128}, {  0, 384, 256, 128}, {1.f, 1.f, 1.f, 1.f}},
		};


		for (DrawInfo i : drawInfoTable) {
			i.draw.Offset(800, 300);
			res->img->Draw(i.draw, i.src);
		}
	}

	//-------------------------------------------------------------------
	//�󂯐g
	void  Object::Recieved()
	{
		int point[] = {
			1,5,10,50,100,500,1000,5000,10000
		};
		ge->isGameClear;
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