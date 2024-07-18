//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiFoodManager.h"
#include  "Task_OguiFood.h"
#include  "Task_OguiPlayer.h"
#include  "Task_OguiGame.h"

namespace  OguiFoodManager
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
		auto game = ge->GetTask<OguiGame::Object>("�{��");

		//��H���~�j�Q�[���^�X�N���擾�ł��Ă��邩�m�F
		if (game == nullptr)
		{
			return;
		}

		//�~�j�Q�[�����̏���
		if (game->isInGame == true)
		{
			//�������̐���
			//���������邩�m�F
			for (int i = 0; i < size(hasExistFoods); ++i)
			{
				//����������������30�t���[���҂�����A�����������v���C���[�̏ꏊ�ɗ�����z�u
				if (this->hasExistFoods[i] == false)
				{
					this->createCount[i]++;
					if (this->createCount[i] >= 10) //���j�^�[FPS�ɃQ�[�����ˑ����Ȃ��悤�ɂ��邽�߂ɏ������� * GetFps() / (float)gameFps ��ǉ�����
					{
						this->createCount[i] = 0; //�����J�E���g���[���N���A����
						CreateFood((PlayerNum)(i + 1)); //i+1�łǂ̃v���C���[�̗����𐶐����邩����
						this->hasExistFoods[i] = true;
					}
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//�����̐���
	void Object::CreateFood(PlayerNum food_PlayerNum)
	{
		//�������̐���
		auto f = OguiFood::Object::Create(true);
		f->hp = f->maxHP;
		f->playerNum = food_PlayerNum;
		f->pos = this->foodPositions[(int)food_PlayerNum - 1]; //�󂯎�����v���C���[���ʔԍ�-1�łǂ̃v���C���[�̈ʒu�ɗ������o�������邩����

		//���������������ꂽ���𑗂�
		SendCreateFoodInformation(food_PlayerNum);
	}
	//-------------------------------------------------------------------
	//�������Ȃ��Ȃ��������擾
	void Object::NotExistFood(PlayerNum food_PlayerNum)
	{
		this->hasExistFoods[(int)food_PlayerNum - 1] = false; //�󂯎�����v���C���[���ʔԍ�-1�łǂ̃v���C���[�ɗ�����������������
	}
	//-------------------------------------------------------------------
	//�����𐶐��������𑼃^�X�N�ɑ���
	void Object::SendCreateFoodInformation(PlayerNum food_PlayerNum)
	{
		//���v���C���[�ɏ��𑗂�
		//�v���C���[��S�Ď擾
		auto players = ge->GetTasks<OguiPlayer::Object>("�v���C���[");
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			//�������������ꂽ�v���C���[������
			if ((*p)->playerNum == food_PlayerNum)
			{
				//�������������ꂽ�̂�true�𑗂�
				(*p)->SetExistFood(true);
			}
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
	Object::Object()
		:
		createCount{30, 30, 30, 30},
		hasExistFoods{false, false, false, false}
	{	}
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