//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_CommonItemManager01.h"
#include "Task_Clock.h"
#include "Task_hand.h"

namespace  CommonItemManager01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->bgImg = DG::Image::Create("./data/image/heya_blue.jpg");
		this->PlayerNum = DG::Image::Create("./data/image/PlayerNumber.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->bgImg.reset();
		this->PlayerNum.reset();
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
		this->rank = 0;

		/*CTList.push_back(ge->in1);
		CTList.push_back(ge->in2);
		CTList.push_back(ge->in3);
		CTList.push_back(ge->in4);*/

		for (auto i = 0; i < ge->players.size(); ++i)
		{
			auto c = Clock::Object::Create(true, i);
			ClockList.push_back(c);
			//ClockList[i]->id = i;

			auto h = hand::Object::Create(true);
			PLhandList.push_back(h);
			PLhandList[i]->id = i;

			c->Positionalise(PlayerAreaPos[ge->players.size() - 1][i]);
			h->Positionalise(PlayerAreaPos[ge->players.size() - 1][i]);

			h->controller = ge->players[i]/*CTList[i]*/;
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		for (int i = 0; i < ge->players.size(); ++i) {
			//�w�i
			ML::Box2D draw(0, 0, 1920 / 2, 1080 / 2);
			ML::Box2D src(0, 0, 1920, 1080);
			draw.Offset(PlayerAreaPos[ge->players.size() - 1][i].x, PlayerAreaPos[ge->players.size() - 1][i].y);
			this->res->bgImg->Draw(draw, src);

			//�v���C���[�i���o�[
			int ToRight = (i % 2) * 770;
			ML::Box2D draw01(0 + ToRight, 1080 / 2 - 105, 715 / 4, 105);
			ML::Box2D src01 = PlayerNumIndexSrc[i];
			draw01.Offset(PlayerAreaPos[ge->players.size() - 1][i].x, PlayerAreaPos[ge->players.size() - 1][i].y);
			this->res->PlayerNum->Draw(draw01, src01);
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