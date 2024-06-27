//-------------------------------------------------------------------
//�T�{��~�j�Q�[���p�v���C���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriPlayer.h"

namespace  SaboriPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->image = DG::Image::Create("./data/image/testImage/testCircle.png");
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
		this->pos = ML::Vec2(ge->screen2DWidth / 2.f, ge->screen2DHeight / 2.f);
		
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
		this->moveCount++;

		//��Ԕ��f
		this->Think();
		//��ԂɑΉ�����s������
		this->Move();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���`�揈��
		DrawInformation drawImage = this->DrawImage();
		drawImage.draw.Offset(this->pos);
		
		this->res->image->Draw(drawImage.draw, drawImage.src);
	}
	//-------------------------------------------------------------------
	//���݂̃v���C���[�̏�Ԑ���
	void Object::Think()
	{
		State nowState = this->state;	//�Ƃ肠�������݂̏�Ԃ���

		//���[�V�����̐؂�ւ�
		switch (nowState)
		{
		case State::Work:		//�d�����
			//�{�^�����͂ŃT�{���Ԃ�
			break;

		case State::Break:		//�T�{����
			//�{�^�����͂���߂���d����Ԃ�
			//��i�ɔ������ꂽ��T�{��΂��Ԃ�
			break;

		case State::Noticed:	//�T�{��΂���
			//��莞�Ԃ�������d����Ԃ�
			break;
		}

		//��ԍX�V
		this->UpdateState(nowState);
	}
	//-------------------------------------------------------------------
	//��ԍX�V���̏���
	void Object::UpdateState(State nowState)
	{
		if (nowState != this->state)
		{
			this->state = nowState;
			this->moveCount = 0;
		}
	}
	//-------------------------------------------------------------------
	//��Ԗ��̍s������
	void Object::Move()
	{
		switch (this->state)
		{
		case State::Break:
			//�T�{�������Ԃ����Z
			break;
		}
	}
	//-------------------------------------------------------------------
	//�A�j���[�V��������
	Object::DrawInformation Object::DrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(100, 0, 100, 100) },	//�d�����
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(200, 0, 100, 100) },	//�T�{����
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(0, 0, 100, 100) },	//�T�{��΂���
		};

		DrawInformation rtv;
		switch (this->state)
		{
		case State::Work:		//�d�����
			rtv = imageTable[0];
			break;

		case State::Break:		//�T�{����
			rtv = imageTable[1];
			break;

		case State::Noticed:	//�T�{��΂���
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
		state(State::Work),
		moveCount(0)
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