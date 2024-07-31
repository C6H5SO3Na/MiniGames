//-------------------------------------------------------------------
//���U���g�\���p�v���C���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ResultUIPlayer.h"

namespace  ResultUIPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->playerImage = DG::Image::Create("./data/image/game_otsan_pose.png");
		this->playerNumberImage = DG::Image::Create("./data/image/PlayerNumber.png");
		this->rankImage = DG::Image::Create("./data/image/CompanyRank.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->playerImage.reset();
		this->playerNumberImage.reset();
		this->rankImage.reset();
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���\���p�v���C���[�̕`��
		DrawInformation drawPlayerImage = this->GetDrawImages(0);
		drawPlayerImage.draw.Offset(this->drawPos.displayPlayerPos);

		this->res->playerImage->Draw(drawPlayerImage.draw, drawPlayerImage.src);

		//���v���C���[�ԍ��̕`��
		DrawInformation drawPlayerNumberImage = this->GetDrawImages(1);
		drawPlayerNumberImage.draw.Offset(this->drawPos.playerNumPos);

		this->res->playerNumberImage->Draw(drawPlayerNumberImage.draw, drawPlayerNumberImage.src);

		//�����ʉ摜�̕`��
		DrawInformation drawRankImage = this->GetDrawImages(2);
		drawRankImage.draw.Offset(this->drawPos.rankPos);

		this->res->rankImage->Draw(drawRankImage.draw, drawRankImage.src);

		//���X�R�A�̕`��

	}
	//-------------------------------------------------------------------
	//�A�j���[�V��������@imageNum��0 = �v���C���[�A1 = �v���C���[�̔ԍ��A2 = ���ʁ@�̉摜����
	Object::DrawInformation Object::GetDrawImages(int imageNum)
	{
		DrawInformation imageTable[] = {
			//�v���C���[
			{ ML::Box2D(-218 / 2, -271 / 2, 436 / 2, 542 / 2), ML::Box2D(0, 0, 436, 542) },		//�v�f��0 1�ʃv���C���[
			{ ML::Box2D(-218 / 2, -271 / 2, 436 / 2, 542 / 2), ML::Box2D(436, 0, 436, 542) },	//�v�f��1 2�ʃv���C���[
			{ ML::Box2D(-218 / 2, -271 / 2, 436 / 2, 542 / 2), ML::Box2D(872, 0, 436, 542) },	//�v�f��2 3�ʃv���C���[
			{ ML::Box2D(-218 / 2, -271 / 2, 436 / 2, 542 / 2), ML::Box2D(1308, 0, 436, 542)},	//�v�f��3 4�ʃv���C���[

			//�v���C���[�̔ԍ�
			{ ML::Box2D(-78, -53, 155, 105), ML::Box2D(0, 0, 155, 105) },	//�v�f��4 1P
			{ ML::Box2D(-96, -53, 192, 105), ML::Box2D(155, 0, 192, 105) },	//�v�f��5 2P
			{ ML::Box2D(-88, -53, 175, 105), ML::Box2D(347, 0, 175, 105) },	//�v�f��6 3P
			{ ML::Box2D(-97, -53, 193, 105), ML::Box2D(522, 0, 193, 105) },	//�v�f��7 4P

			//����
			{ ML::Box2D(-77, -43, 154, 85), ML::Box2D(0, 0, 154, 85) },		//�v�f��8  1��(�В�)
			{ ML::Box2D(-77, -43, 154, 85), ML::Box2D(154, 0, 154, 85) },	//�v�f��9  2��(����)
			{ ML::Box2D(-78, -43, 155, 85), ML::Box2D(308, 0, 155, 85) },	//�v�f��10 3��(�ے�)
			{ ML::Box2D(-110, -43, 219, 85), ML::Box2D(463, 0, 219, 85) },	//�v�f��11 4��(���Ј�)

			//�X�R�A
		};
		DrawInformation rtv = {};

		//�ǂ̕`������擾���邩���߂�
		switch (imageNum)
		{
		case 0: //�v���C���[
			//���ʂłǂ̉摜��\�����邩���߂�
			switch (this->drawPlayerInfo.rank)
			{
			case 1:	//1��
				rtv = imageTable[0];
				break;

			case 2:	//2��
				rtv = imageTable[1];
				break;

			case 3:	//3��
				rtv = imageTable[2];
				break;

			case 4:	//4��
				rtv = imageTable[3];
				break;
			}
			break;

		case 1: //�v���C���[�ԍ�
			switch (this->drawPlayerInfo.PlayerNum)
			{
			case 1: //1P
				rtv = imageTable[4];
				break;

			case 2: //2P
				rtv = imageTable[5];
				break;

			case 3:	//3P
				rtv = imageTable[6];
				break;

			case 4: //4P
				rtv = imageTable[7];
				break;
			}
			break;

		case 2: //����
			switch (this->drawPlayerInfo.rank)
			{
			case 1:	//1��(�В�)
				rtv = imageTable[8];
				break;

			case 2:	//2��(����)
				rtv = imageTable[9];
				break;

			case 3:	//3��(�ے�)
				rtv = imageTable[10];
				break;

			case 4:	//4��(���Ј�)
				rtv = imageTable[11];
				break;
			}
			break;

		case 3: //�X�R�A
			break;
		}

		return rtv;
	}
	//-------------------------------------------------------------------
	//�`�悷��v���C���[�̏����擾����
	void Object::setPlayerInformation(const Result::Object::PlayerInformation& playerInfo)
	{
		this->drawPlayerInfo.PlayerNum = playerInfo.PlayerNum;
		this->drawPlayerInfo.rank = playerInfo.rank;
		this->drawPlayerInfo.score = playerInfo.score;
	}
	//-------------------------------------------------------------------
	//�`��ʒu��ݒ肷��
	void Object::setDrawPos(const ML::Vec2& playerPos, const ML::Vec2& playerNumPos, const ML::Vec2& rankPos, const ML::Vec2& scorePos)
	{
		this->drawPos.displayPlayerPos = playerPos;
		this->drawPos.playerNumPos = playerNumPos;
		this->drawPos.rankPos = rankPos;
		this->drawPos.scorePos = scorePos;
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