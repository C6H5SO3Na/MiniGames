//-------------------------------------------------------------------
//�o�����X�Q�[���̃v���C���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_BlanceGamePlayer.h"
#include  "../sound.h"

namespace  BGPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		playerImg = DG::Image::Create("./data/image/game_otsan_train.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		playerImg.reset();
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
		direction = 0;
		SetBGState(BGstate::BStart);
	
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
		auto in = controller->GetState();
		switch (GetBGState())
		{
		case BGstate::Playing:
			break;
		case BGstate::PlayR:
			direction += 1;
			if (in.LStick.BL.down) { SetBGState(BGstate::PlayL); }
			if (direction > 45) { SetBGState(BGstate::Fail); }
			if (direction == 0) { SetBGState(BGstate::Playing); }
			break;
		case BGstate::PlayL:
			direction -= 1;
			if (in.LStick.BR.down) { SetBGState(BGstate::PlayR); }
			if (direction < -45) { SetBGState(BGstate::Fail); }
			if (direction == 0) { SetBGState(BGstate::Playing); }
			break;
		case BGstate::Fail:
			pos.y = 600;
			direction = 90;
			break;
		}
		
		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D src(0, 0, 446, 542);
		ML::Box2D draw(-100, 0, 360, 432);
		switch (GetBGState())
		{
		case BGstate::Playing:
			break;
		case BGstate::PlayR:
			src = ML::Box2D(2236, 543, 446, 542);
			break;
		case BGstate::PlayL:
			src = ML::Box2D(2236, 1086, 446, 542);
			break;
		case BGstate::Fail:
			
			break;
		}
		res->playerImg->Rotation(ML::ToRadian(direction * 2), ML::Vec2(180, 432));
		res->playerImg->Draw(draw.OffsetCopy(pos), src);
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