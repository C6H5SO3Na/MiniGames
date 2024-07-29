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
		this->playerImage = DG::Image::Create("./data/image/Ohara/testImage/testSquare.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->playerImage.reset();
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

		//�����ʉ摜�̕`��

		//���X�R�A�̕`��

	}
	//-------------------------------------------------------------------
	//�A�j���[�V��������@imageNum��0 = �v���C���[�A1 = �v���C���[�̔ԍ��A2 = ���ʁ@�̉摜����
	Object::DrawInformation Object::GetDrawImages(int imageNum)
	{
		DrawInformation imageTable[] = {
			//�v���C���[
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(200, 0, 100, 100) },	//�v�f��0 1�ʃv���C���[
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(100, 0, 100, 100) },	//�v�f��1 2�ʃv���C���[
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(300, 0, 100, 100) },	//�v�f��2 3�ʃv���C���[
			{ ML::Box2D(-50, -50, 100, 100), ML::Box2D(0, 0, 100, 100)},	//�v�f��3 4�ʃv���C���[

			//�v���C���[�̔ԍ�

			//����

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

		case 1: //�v���C���[�ԍ�
			switch (this->drawPlayerInfo.PlayerNum)
			{
			case 1: //1P
				break;

			case 2: //2P
				break;

			case 3:	//3P
				break;

			case 4: //4P
				break;
			}
			break;

		case 2: //����
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