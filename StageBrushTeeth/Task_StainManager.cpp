//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_StainManager.h"
#include  "Task_stain.h"
#include  "../randomLib.h"

namespace  StainManager
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

		
		this->minPosX = 0;
		this->maxPosX = 0;
		this->minPosY = 0;
		this->maxPosY = 0;

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	void Object::Positionalise(int PlayerNum)
	{
		ML::Box2D StainArea(PlayerNum % 2 * (1980 / 2), PlayerNum / 2 * (1080 / 2), (1980 / 2), (1080 / 2));
		minPosX = StainArea.x;
		minPosY = StainArea.y;
		maxPosX = StainArea.x + StainArea.w -32;
		maxPosY = StainArea.y + StainArea.h -32;
	}
	//-------------------------------------------------------------------
	void Object::CreateStain()
	{
		for (int i = 0; i < 5; ++i)
		{
			auto position = GetStainPos(positions);
			positions.push_back(position);
		}

		//���f�[�^������
		for (int i = 0; i < 5; ++i)
		{
			auto s = stain::Object::Create(true);
			s->pos = positions[i];
			stains.push_back(s);
		}
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
	}
	//-------------------------------------------------------------------
	ML::Vec2 Object::GetStainPos(vector<ML::Vec2>& positions)
	{
		float x = GetRandom(this->minPosX, this->maxPosX);
		float y = GetRandom(this->minPosY, this->maxPosY);

		int w = 64;
		int h = 64;
		auto hit = ML::Box2D(-w / 2, -h / 2, w, h);

		// �d�Ȃ��Ă��Ȃ����`�F�b�N
		auto me = hit.OffsetCopy(x, y);
		for (auto p = positions.begin(); p != positions.end(); p++)
		{
			auto you = hit.OffsetCopy((*p));
			if (you.Hit(me)) {
				return GetStainPos(positions);
			}
		}

		return ML::Vec2(x, y);
	}
	//-------------------------------------------------------------------
	
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