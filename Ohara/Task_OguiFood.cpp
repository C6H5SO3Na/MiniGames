//-------------------------------------------------------------------
//��H���~�j�Q�[���̗���
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiFood.h"
#include  "Task_OguiFoodManager.h"
#include  "Task_OguiPlayer.h"
#include  "Task_OguiGame.h"

#include  "../randomLib.h"

namespace  OguiFood
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->image = DG::Image::Create("./data/image/otsan_ramen.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->image.reset();
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
		this->render2D_Priority[1] = 0.6f;
		this->state = State::FExist;
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
		auto game = ge->GetTask<OguiGame::Object>(OguiGame::defGroupName, OguiGame::defName);

		//��H���~�j�Q�[���^�X�N���擾�ł��Ă��邩�m�F
		if (game == nullptr) // null�`�F�b�N
		{
			return;
		}
		
		//�~�j�Q�[�����̏���
		if (game->isInGame == true)
		{
			this->moveCnt++;

			//��Ԕ��f
			this->Think();
			//��ԂɑΉ�����s������
			this->Move();
		}
	}

	//-------------------------------------------------------------------
	//�����ō�����֐��̏���
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�������`��
		DrawInformation drawImage = GetDrawImage();
		drawImage.draw.Offset(this->pos);

		this->res->image->Draw(drawImage.draw, drawImage.src);

		//���c�藿���c�ʕ`��
		//�`��
		if (this->hp > 0)
		{
			testFont->Draw(ML::Box2D(static_cast<int>(this->pos.x) - 25, static_cast<int>(this->pos.y) - 25, ge->screen2DWidth, ge->screen2DHeight),
				to_string(this->hp), ML::Color(1, 0, 0, 0)
			);
		}
		else
		{
			testFont->Draw(ML::Box2D(static_cast<int>(this->pos.x) - 25, static_cast<int>(this->pos.y) - 25, ge->screen2DWidth, ge->screen2DHeight),
				"0", ML::Color(1, 0, 0, 0)
			);
		}
	}
	//-------------------------------------------------------------------
	//���݂̗����̏�Ԑ���
	void Object::Think()
	{
		State nowState = this->state; //�Ƃ肠�������݂̏�Ԃ���

		//���[�V�����̐؂�ւ�
		switch (nowState)
		{
		case State::FExist:
			if (this->hp <= 0) { nowState = State::FNotExist; }
			break;
		}

		//��ԍX�V
		this->UpdateState(nowState);
	}
	//-------------------------------------------------------------------
	//��Ԗ��̍s������
	void Object::Move()
	{
		switch (this->state)
		{
		case State::FNotExist:
			//�����������Ȃ������𑼃^�X�N�ɑ���
			SendNotExistInformation();
			this->Kill();

			//�����o�O���o���炱�̏������̒��ɏ�̏���������
			//�������������Ȃ������A30�t���[���ڂɃI�u�W�F�N�g�������\�����o��
			//if (this->moveCnt >= 30) //���j�^�[FPS�ɃQ�[�����ˑ����Ȃ��悤�ɂ��邽�߂ɏ������� (int)(30 * GetFps() / (float)gameFps) �ɕύX����
			//{
			//	
			//}
		}
	}
	//-------------------------------------------------------------------
	//�A�j���[�V��������
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(-367 / 3, -233 / 3, 733 / 3, 466 / 3), ML::Box2D(0, 0, 734, 466) }
		};

		DrawInformation rtv = imageTable[0];

		return rtv;
	}
	//-------------------------------------------------------------------
	//�����̌���(�󂯐g�̏���)
	void Object::ReduceHP(int damage)
	{
		hp -= damage;
	}
	//-------------------------------------------------------------------
	//�����������Ȃ������𗿗��Ǘ��^�X�N�ɑ���
	void Object::SendNotExistInformation()
	{
		//�������Ǘ��^�X�N�ɏ��𑗂�
		auto foodManager = ge->GetTask<OguiFoodManager::Object>("�Ǘ�");
		foodManager->NotExistFood(this->playerNum);

		//���v���C���[�ɏ��𑗂�
		//�S�Ẵv���C���[���擾
		auto players = ge->GetTasks<OguiPlayer::Object>("�v���C���[");
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			//���̗�����H�ׂ��v���C���[������
			if ((*p)->playerNum == this->playerNum)
			{
				//�v���C���[�̐H�ׂ��������̃J�E���g�𑝂₷
				(*p)->eatFoodCount++;
				//�����������Ȃ����̂�false�𑗂�
				(*p)->SetExistFood(false);
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
		hp(0)
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