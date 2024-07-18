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
			bookImg = DG::Image::Create("./data/image/debugrect.png");
		}
		CountImg = DG::Image::Create("./data/image/font_number.png");
	
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
		workTime = 4;
		SetCGState(CGstate::BStart);
		Rb = 0;
		Gb = 0;
		Bb = 0;
		Fb = 0;
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�������v���ނ̍��W
	void Object::posInitialize(ML::Vec2 ppos)
	{
		pos = ppos;
		for (int i = 0; i < 14; ++i) {
			books[i].bpos = pos + ML::Vec2(0, -40);
		}
		for (int i = 0; i < 3; ++i) {
			books[i].bpos += ML::Vec2(-80 + i * 80, -100);
			books[i].color = i;
		}
		books[3].bpos += ML::Vec2(0, -200);
		books[3].color = 3;
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
				moveCnt = 0;
				workTime++;
			}
			if (in.LStick.BL.down) {
				moveCnt = 0;
				Rb++;
				books[workTime - 1].bpos += ML::Vec2(-80, -100 - Rb * 5);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt == 118 || in.LStick.BU.down || in.LStick.BR.down) {//���Ԑ؂�A���͊ԈႢ�{�^����������
				toFailState(workTime);
				SetCGState(CGstate::Fail);
			}
			break;
		case CGstate::PlayB:
			if (FirstIntoState()) {
				books[workTime].color = 1;
				moveCnt = 0;
				workTime++;
			}
			if (in.LStick.BU.down) {
				moveCnt = 0;
				Bb++;
				books[workTime - 1].bpos += ML::Vec2(0, -100 - Bb * 5);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt == 118 || in.LStick.BL.down || in.LStick.BR.down) {//���Ԑ؂�A���͊ԈႢ�{�^����������
				toFailState(workTime);
				SetCGState(CGstate::Fail);
			}
			break;
		case CGstate::PlayG:
			if (FirstIntoState()) {
				books[workTime].color = 2;
				moveCnt = 0;
				workTime++;
			}
			if (in.LStick.BR.down) {
				moveCnt = 0;
				Gb++;			
				books[workTime - 1].bpos += ML::Vec2(80, -100 - Gb * 5);
				SetCGState(CGstate::Playing);
			}
			moveCnt++;
			if (moveCnt == 118 ||in.LStick.BL.down || in.LStick.BU.down) {//���Ԑ؂�A���͊ԈႢ�{�^����������
				toFailState(workTime);
				SetCGState(CGstate::Fail);
			}
			break;
		case CGstate::Fail:	
			Fb++;
			SetCGState(CGstate::Playing);
			break;
		}
		
		
	}
	void  Object::toFailState(int workT) {
		books[workT - 1].color = 3;
		moveCnt = 0;
		books[workT - 1].bpos += ML::Vec2(0, -200 - Fb * 5);
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
		for (int i = 0; i < 14; ++i) {
			if (workTime > i) {
				src.x = books[i].color * 32;
				ML::Box2D draw0 = draw.OffsetCopy(books[i].bpos);
				res->bookImg->Draw(draw0, src);
			}
		}
		src = ML::Box2D(0, 35, 20, 25);
		draw = ML::Box2D(0, 0, 20, 25);
		res->CountImg->Draw(draw.OffsetCopy(pos+ML::Vec2(-80, -100)), src.OffsetCopy(20 * Rb, 0));
		res->CountImg->Draw(draw.OffsetCopy(pos + ML::Vec2(0, -100)), src.OffsetCopy(20 * Bb, 0));
		res->CountImg->Draw(draw.OffsetCopy(pos + ML::Vec2(80, -100)), src.OffsetCopy(20 * Gb, 0));
		res->CountImg->Draw(draw.OffsetCopy(pos + ML::Vec2(0, -200)), src.OffsetCopy(20 * Fb, 0));
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