//-------------------------------------------------------------------
//�o�����X�Q�[���̃v���C���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ClassifyGamePlayer.h"

namespace  CGPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		playerImg = DG::Image::Create("./data/image/chara02.png");
		for (int i = 0; i < 10; ++i) {
			bookImg[i] = DG::Image::Create("./data/image/debugrect.png");
		}
	
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
		workTime = 0;
		SetCGState(CGstate::BStart);
		for (int i = 0; i < 10; ++i) {
			books[i].bpos = pos + ML::Vec2(0, -40);
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
		auto in = controller->GetState();
		switch (GetCGState())
		{
		case CGstate::Playing:
			break;
		case CGstate::PlayR:
			if (FirstIntoState()) {
				books[workTime].color = 0;
				workTime++;
			}
			if (in.LStick.BL.down) {
				moveCnt = 0;
				books[workTime].bpos += ML::Vec2(-30, -100);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt = 119) {
				toAnotherState(workTime);
			}
			break;
		case CGstate::PlayG:
			if (FirstIntoState()) {
				books[workTime].color = 2;
				workTime++;
			}
			if (in.LStick.BU.down) {
				moveCnt = 0;
				books[workTime].bpos += ML::Vec2(0, -100);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt = 119) {
				toAnotherState(workTime);
			}
			break;
		case CGstate::PlayB:
			if (FirstIntoState()) {
				books[workTime].color = 1;
				workTime++;
			}
			if (in.LStick.BR.down) {
				moveCnt = 0;
				books[workTime].bpos += ML::Vec2(30, 100);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt = 119) {
				toAnotherState(workTime);
			}
			break;
		case CGstate::Fail:
			
			break;
		}
		
		
	}
	void  Object::toAnotherState(int workT) {
		books[workT].color = 3;
		moveCnt = 0;
		books[workT].bpos += ML::Vec2(0, -200);
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D src(0, 0, 32, 80);
		ML::Box2D draw(0, 0, 32, 80);
		
		res->playerImg->Draw(draw.OffsetCopy(pos), src);
		src = ML::Box2D(0, 0, 32, 32);
		draw = ML::Box2D(0, 0, 32, 32);
		for (int i = 0; i < 10; ++i) {
			if (workTime > i) {
				src.x = books[i].color * 32;
				ML::Box2D draw0 = draw.OffsetCopy(books[i].bpos);
				res->bookImg[i]->Draw(draw0, src);
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