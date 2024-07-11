//-------------------------------------------------------------------
//�T�{��~�j�Q�[���̃v���C���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriGame.h"

#include  "../fpscounter.h"

namespace  SaboriPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->image = DG::Image::Create("./data/image/Ohara/testImage/testCircle.png");
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
		this->state = State::PWork;
		
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
		auto game = ge->GetTask<SaboriGame::Object>("�{��");

		//���ڂ�~�j�Q�[���^�X�N���擾�ł��Ă��邩�m�F
		if (game == nullptr)
		{
			return;
		}

		//�~�j�Q�[�����̏���
		if (game->isGameOver == false)
		{
			this->moveCnt++;

			//��Ԕ��f
			this->Think();
			//��ԂɑΉ�����s������
			this->Move();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���`��
		DrawInformation drawImage = this->GetDrawImage();
		drawImage.draw.Offset(this->pos);
		
		this->res->image->Draw(drawImage.draw, drawImage.src);
	}
	//-------------------------------------------------------------------
	//���݂̃v���C���[�̏�Ԑ���
	void Object::Think()
	{
		auto input = this->controller->GetState();
		State nowState = this->state;	//�Ƃ肠�������݂̏�Ԃ���

		//���[�V�����̐؂�ւ�
		switch (nowState)
		{
		case State::PWork:		//�d�������
			if (input.B1.on) { nowState = State::PSabori; } //�T�{���Ԃ�
			break;

		case State::PSabori:		//�T�{����
			if (input.B1.up) { nowState = State::PWork; } //�d������Ԃ�
			if (noticedToSabori) { nowState = State::PNoticed; } //�T�{��΂��Ԃ�
			break;

		case State::PNoticed:	//�T�{��΂���
			if (this->moveCnt >= this->gameFps * 6) { nowState = State::PWork; } //�d������Ԃ� ���j�^�[FPS�ɃQ�[�����ˑ����Ȃ��悤�ɂ��邽�߂ɏ������� * GetFps() / (float)gameFps ����
			break;
		}

		//��ԍX�V
		this->UpdateState(nowState);
	}
	//-------------------------------------------------------------------
	//��Ԗ��̍s������
	void Object::Move()
	{
		//fpscounter���C���X�^���X������

		switch (this->state)
		{
		case State::PSabori:	//�T�{����
			this->totalSaboriTime += 1.f / gameFps; // / gameFps �� / GetFps() ���ɕύX���ă��j�^�[FPS�ɃQ�[�����ˑ����Ȃ��悤�ɂ���
			break;

		case State::PNoticed:
			this->noticedToSabori = false;
			break;
		}
	}
	//-------------------------------------------------------------------
	//�A�j���[�V��������
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(100, 0, 100, 100) },	//�d�����
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(200, 0, 100, 100) },	//�T�{����
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(0, 0, 100, 100) },	//�T�{��΂���
		};

		DrawInformation rtv;
		switch (this->state)
		{
		case State::PWork:		//�d�������
			rtv = imageTable[0];
			break;

		case State::PSabori:		//�T�{����
			rtv = imageTable[1];
			break;

		case State::PNoticed:	//�T�{��΂���
			rtv = imageTable[2];
			break;
		}

		return rtv;
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
		totalSaboriTime(0.f),
		noticedToSabori(false)
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