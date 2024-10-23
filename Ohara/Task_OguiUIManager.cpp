//-------------------------------------------------------------------
//��H���~�j�Q�[����UI�Ǘ�
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiUIManager.h"
#include  "Task_OguiGame.h"
#include  "Task_OguiPlayer.h"

namespace  OguiUIManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		playerNumberImage = DG::Image::Create("./data/image/PlayerNumber.png");
		eatFoodCountImage = DG::Image::Create("./data/image/TextImage/OguiFont.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		playerNumberImage.reset();
		eatFoodCountImage.reset();
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
		testFont = DG::Font::Create("�l�r �S�V�b�N", 30, 50);

		//�~�j�Q�[�������^�X�N����f�[�^���擾����
		auto game = ge->GetTask<OguiGame::Object>(OguiGame::defGroupName, OguiGame::defName);
		if (game)
		{
			playerCount = game->GetPlayerCount();
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���H�ׂ��������̕`��
		//�v���C���[�S�Ă𒊏o����
		auto players = ge->GetTasks<OguiPlayer::Object>("�v���C���[");
		//�v���C���[�̐��������[�v����
		int loopCount = 0; //���[�v�����񐔂̃J�E���g
		if (players)
		{
			for (auto p = players->begin(); p != players->end(); ++p)
			{
				//�v���C���[���ʗp�ԍ��`��
				DrawPlayerNumber_eatFoodCount(loopCount);
				/*testFont->Draw(ML::Box2D(45 + ge->screen2DWidth * loopCount / 4, 90, ge->screen2DWidth, ge->screen2DHeight),
					to_string((int)(*p)->playerNum) + "P:" + to_string((*p)->eatFoodCount), ML::Color(1, 0, 0, 0)
				);*/

				//�H�ׂ������̐��`��

				//���[�v�񐔂̃J�E���g
				++loopCount;
			}
		}

		//���v���C���[�ԍ��̕`��
		this->DrawPlayerNumber();
	}

	//-------------------------------------------------------------------
	//������֐���
	//-------------------------------------------------------------------
	//�v���C���[�ԍ��̕`��
	void Object::DrawPlayerNumber()
	{
		//for (int i = 0; i < size(this->playerNumbersDrawInfo); ++i) // CPU�������͂��������g��
		for (int i = 0; i < playerCount; ++i)
		{
			ML::Box2D playerNumberDraw = this->playerNumbersDrawInfo[i].draw;
			playerNumberDraw.Offset(this->playerNumbersDrawInfo[i].pos);
			ML::Box2D playerNumberSrc = this->playerNumbersDrawInfo[i].src;

			//�`��
			this->res->playerNumberImage->Draw(playerNumberDraw, playerNumberSrc);
		}
	}

	//-------------------------------------------------------------------
	//�H�ׂ������̐��̔��ʗp�v���C���[�ԍ��̕`��
	void Object::DrawPlayerNumber_eatFoodCount(const int loopCount)
	{
		ML::Box2D playerNumberDraw = playerNumbersDrawInfo_eatFoodCount[loopCount].draw;
		playerNumberDraw.Offset(playerNumbersDrawInfo_eatFoodCount[loopCount].pos);
		ML::Box2D playerNumberSrc = playerNumbersDrawInfo_eatFoodCount[loopCount].src;

		//�`��
		this->res->playerNumberImage->Draw(playerNumberDraw, playerNumberSrc);
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
		//�v���C���[���ʔԍ��֌W
		playerNumbersDrawInfo{
			{ ML::Box2D(-78, -53, 155, 105), ML::Box2D(0, 0, 155, 105), ML::Vec2(ge->screen2DWidth / 8.f, ge->screen2DHeight / 2.f - 200.f) },			//1P
			{ ML::Box2D(-96, -53, 192, 105), ML::Box2D(155, 0, 192, 105), ML::Vec2(ge->screen2DWidth * 3.f / 8.f, ge->screen2DHeight / 2.f - 200.f) },	//2P
			{ ML::Box2D(-88, -53, 175, 105), ML::Box2D(347, 0, 175, 105), ML::Vec2(ge->screen2DWidth * 5.f / 8.f, ge->screen2DHeight / 2.f - 200.f) },	//3P
			{ ML::Box2D(-97, -53, 193, 105), ML::Box2D(522, 0, 193, 105), ML::Vec2(ge->screen2DWidth * 7.f / 8.f, ge->screen2DHeight / 2.f - 200.f) }	//4P
		},
		playerNumbersDrawInfo_eatFoodCount{
			{ ML::Box2D(static_cast<int>(-78 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(155 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(0, 0, 155, 105), ML::Vec2(ge->screen2DWidth / 12.f - 50.f, 100.f + (53.f / 1.5f))},			// 1P
			{ ML::Box2D(static_cast<int>(-96 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(192 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(155, 0, 192, 105), ML::Vec2(ge->screen2DWidth * 4.f / 12.f - 50.f, 100.f + (53.f / 1.5f))},	// 2P
			{ ML::Box2D(static_cast<int>(-88 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(175 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(347, 0, 175, 105), ML::Vec2(ge->screen2DWidth * 7.f / 12.f - 50.f, 100.f + (53.f / 1.5f))},	// 3P
			{ ML::Box2D(static_cast<int>(-97 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(193 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(522, 0, 193, 105), ML::Vec2(ge->screen2DWidth * 10.f / 12.f - 50.f, 100.f + (53.f / 1.5f))}	// 4P
		},
		playerCount(1)
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

/*����*/
/*����Initialize�ŃQ�[����V�ԃv���C���[�̐l����Task_OguiGame��getter�֐��Ŏ擾����*/