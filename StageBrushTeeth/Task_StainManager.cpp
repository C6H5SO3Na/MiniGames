//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_StainManager.h"
#include  "Task_stain.h"
#include  "../randomLib.h"
#include  "Task_CommonItemManager02.h"
#include  "Task_StageBrushTeeth.h"

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
		const int num = 170;
		ML::Vec2 pos[][2] = {
			{ ML::Vec2(0.f, 0.f), ML::Vec2(1980 / 2, 1080 / 2) },
			{ ML::Vec2(1980 / 2, 0.f), ML::Vec2(1980, 1080 / 2) },
			{ ML::Vec2(0.f, 1080 / 2), ML::Vec2(1980 / 2, 1080) },
			{ ML::Vec2(1980 / 2, 1080 / 2), ML::Vec2(1980, 1080) },
		};
		const int min = 0;
		const int max = 1;

		/*ML::Box2D StainArea(PlayerNum % 2 * (1980 / 2), PlayerNum / 2 * (1080 / 2), (1980 / 2), (1080 / 2));*/
		minPosX = pos[PlayerNum][min].x + num + 40;//StainArea.x;
		minPosY = pos[PlayerNum][min].y + num - 50;//StainArea.y;
		maxPosX = pos[PlayerNum][max].x - 16 - num - 40;//StainArea.x + StainArea.w - 32;
		maxPosY = pos[PlayerNum][max].y - 16 - num + 100;//StainArea.y + StainArea.h - 32;
	}
	//-------------------------------------------------------------------
	void Object::CreateStain()
	{
		for (int i = 0; i < 10; ++i)
		{
			auto position = GetStainPos(positions);
			positions.push_back(position);
		}

		//���f�[�^������
		for (int i = 0; i < 10; ++i)
		{
			auto s = stain::Object::Create(true);
			s->pos = positions[i];
			s->minX = minPosX;
			s->minY = minPosY;
			s->maxX = maxPosX;
			s->maxY = maxPosY;
			s->id = this->id;//GIVE STAIN A ID
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
		auto stains = ge->GetTasks<stain::Object>("�悲��");
		auto com = ge->GetTask<CommonItemManager02::Object>("���ʃA�C�e���}�l�[�W���[02");
		int KillNum = 0;
		for (auto s = stains->begin(); s != stains->end(); s++)
		{
			if ((*s)->id == this->id) {//if stain and manager have same id
				KillNum++;
			}
		}
		if (KillNum == 0 && !isClear) //�`�F�b�N�N���A //if stain that have same id is null make isClear to true 
		{
			isClear = true;
		}
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

		int w = 32;
		int h = 32;
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