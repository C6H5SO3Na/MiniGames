//-------------------------------------------------------------------
//�o�����X�Q�[���̃v���C���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ClassifyGamePlayer.h"
#include  "../Task_Game.h"
#include  "../sound.h"

namespace  CGPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		for (int i = 0; i < 10; ++i) {
			bookImg = DG::Image::Create("./data/image/game_file2.png");
		}
		CountImg = DG::Image::Create("./data/image/font_number.png");
		deskImg= DG::Image::Create("./data/image/game_desk.png");
		handLImg = DG::Image::Create("./data/image/tsume_hand.png");
		handRImg = DG::Image::Create("./data/image/tsume_hand_1.png");
		playerNumImg = DG::Image::Create("./data/image/PlayerNumber.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		bookImg.reset();
		CountImg.reset();
		deskImg.reset();
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

		//se
		se::LoadFile("seCGf", "./data/sound/se/ClassifyGame/maou_se_onepoint33.wav");
		se::LoadFile("seCGc", "./data/sound/se/ClassifyGame/current.wav");

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
			books[i].bpos += ML::Vec2(-180 + i * 180, -100);
			books[i].color = i;
		}
		books[3].bpos += ML::Vec2(0, -250);
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
				se::Play("seBGd");
			}
			if (in.LStick.BL.down) {
				moveCnt = 0;
				Rb++;
				books[workTime - 1].bpos += ML::Vec2(-180, -100 - Rb * 5);
				se::Play("seCGc");
				SetCGState(CGstate::Playing);
			}
			if (moveCnt == 118 || in.LStick.BU.down || in.LStick.BR.down) {//���Ԑ؂�A���͊ԈႢ�{�^����������
				toFailState(workTime);
				SetCGState(CGstate::Fail);
			}
			moveCnt++;
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
				se::Play("seCGc");
				SetCGState(CGstate::Playing);
			}
			if (moveCnt == 118 || in.LStick.BL.down || in.LStick.BR.down) {//���Ԑ؂�A���͊ԈႢ�{�^����������
				toFailState(workTime);
				SetCGState(CGstate::Fail);
			}
			moveCnt++;
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
				books[workTime - 1].bpos += ML::Vec2(180, -100 - Gb * 5);
				se::Play("seCGc");
				SetCGState(CGstate::Playing);
			}
			if (moveCnt == 118 ||in.LStick.BL.down || in.LStick.BU.down) {//���Ԑ؂�A���͊ԈႢ�{�^����������
				toFailState(workTime);
				SetCGState(CGstate::Fail);
			}
			moveCnt++;
			break;
		case CGstate::Fail:	
			Fb++;
			se::Play("seCGf");
			SetCGState(CGstate::Playing);
			break;
		}
	}
	void  Object::toFailState(int workT) {
		books[workT - 1].color = 3;
		moveCnt = 0;
		books[workT - 1].bpos += ML::Vec2(0, -250 - Fb * 5);
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//��
		ML::Box2D src(0, 0, 1920, 1080);
		ML::Box2D draw(-450, -300, 960, 540);
		res->deskImg->Draw(draw.OffsetCopy(pos), src);
		//��
		src = ML::Box2D(0, 0, 623, 800);
		draw = ML::Box2D(-230, 0, 124, 160);
		res->handLImg->Draw(draw.OffsetCopy(pos), src);
		src = ML::Box2D(0, 0, 623, 800);
		draw = ML::Box2D(126, 0, 124, 160);
		res->handRImg->Draw(draw.OffsetCopy(pos), src);
		//����
		src = ML::Box2D(0, 0, 742, 700);
		draw = ML::Box2D(0, 0, 96, 96);
		for (int i = 0; i < 14; ++i) {
			if (workTime > i) {
				src.x = books[i].color * 742;
				ML::Box2D draw0 = draw.OffsetCopy(books[i].bpos);
				res->bookImg->Draw(draw0, src);
			}
		}
		//�J�E���^�[
		src = ML::Box2D(0, 35, 20, 25);
		draw = ML::Box2D(0, 0, 20, 25);
		res->CountImg->Draw(draw.OffsetCopy(pos + ML::Vec2(-180, -100)), src.OffsetCopy(20 * Rb, 0));
		res->CountImg->Draw(draw.OffsetCopy(pos + ML::Vec2(0, -100)), src.OffsetCopy(20 * Bb, 0));
		res->CountImg->Draw(draw.OffsetCopy(pos + ML::Vec2(180, -100)), src.OffsetCopy(20 * Gb, 0));
		res->CountImg->Draw(draw.OffsetCopy(pos + ML::Vec2(0, -250)), src.OffsetCopy(20 * Fb, 0));

		//playerNum
		src = playerNumUIsrc[playerNum];
		draw = playerNumUIdraw[playerNum];
		res->playerNumImg->Draw(draw.OffsetCopy(pos+ ML::Vec2(-400, -250)), src);
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