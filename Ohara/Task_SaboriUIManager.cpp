//-------------------------------------------------------------------
//�T�{��~�j�Q�[����UI�Ǘ�
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriUIManager.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriGame.h"

namespace  SaboriUIManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		playerNumberImage = DG::Image::Create("./data/image/PlayerNumber.png");
		totalSaboriTimeImage = DG::Image::Create("./data/image/TextImage/TotalSaboriTimeFont.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		playerNumberImage.reset();
		totalSaboriTimeImage.reset();
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

		//�~�j�Q�[�������^�X�N����f�[�^���擾����
		auto game = ge->GetTask<SaboriGame::Object>(SaboriGame::defGroupName, SaboriGame::defName);
		if (game)
		{
			playerCount = game->GetPlayerCount();
		}

		//�T�{�荇�v���Ԃ̕`��ʒu�����߂�
		DecideTotalSaboriTimeDrawPos(playerCount);

		//�v���C���[�ԍ��̕`��ʒu�����߂�
		DecidePlayerNumbersDrawInfo_pos(playerCount);

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
		//���T�{�荇�v���Ԃ̕`��
		//�v���C���[�S�Ă𒊏o����
		auto players = ge->GetTasks<SaboriPlayer::Object>("�v���C���[");
		//�v���C���[�̐��������[�v����
		int loopCount = 0; //���[�v�����񐔂̃J�E���g
		if (players)
		{
			for (auto p = players->begin(); p != players->end(); ++p)
			{
				//���v���ԗp�v���C���[�ԍ��`��
				DrawPlayerNumber_TotalSaboriTime(loopCount);

				//���v�T�{�莞�ԕ`��
				DrawTotalSaboriTime((*p)->totalSaboriTime, loopCount);

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
	//�T�{�荇�v���Ԃ̔��ʗp�v���C���[�ԍ��̕`��
	void Object::DrawPlayerNumber_TotalSaboriTime(const int loopCount)
	{
		ML::Box2D playerNumberDraw = playerNumbersDrawInfo_TotalSaboriTime[loopCount].draw;
		playerNumberDraw.Offset(playerNumbersDrawInfo_TotalSaboriTime[loopCount].pos);
		ML::Box2D playerNumberSrc = playerNumbersDrawInfo_TotalSaboriTime[loopCount].src;

		//�`��
		this->res->playerNumberImage->Draw(playerNumberDraw, playerNumberSrc);
	}

	//-------------------------------------------------------------------
	//���v�T�{�莞�Ԃ̕`��
	void Object::DrawTotalSaboriTime(const float totalSaboriTime, const int loopCount)
	{
		//�����_��`�悵�����𔻒肷��ϐ���false�ɖ߂�
		bool shouldDrewDecimalPoint = false;

		//���v�T�{�莞�Ԃ̏����_���ʂ܂ł𕪉����Ċi�[����
		sprintf(totalSaboriTimeText, "%.1f", totalSaboriTime); 

		//�؂���ʒu�A�\���ʒu�����߂ĕ`�悷��
		for (int i = 0; i < static_cast<int>(size(totalSaboriTimeText)) - 1; ++i)
		{
			//src��x,y�̒l�ݒ�
			int src_x = (totalSaboriTimeText[i] - '0') * 32;	// ML::Box2D src��x�̒l
			int src_y = 0;										// ML::Box2D src��y�̒l
			//����i�Ԗڂ̔z��̒��g���u.�v��������y�̒l��ύX����
			if (totalSaboriTimeText[i] == '.')
			{
				src_x = 0;
				src_y = 32;
			}

			//�����_�̕`��ʒu�ݒ�
			int offValue_x = 0;	// �u.�v�̕`��ł����x���W�̒l

			//�u.�v��`�悵����̕`��ʒu�����炷�l�ݒ�
			if (shouldDrewDecimalPoint)
			{
				offValue_x = -32; // 32��src��w�l
			}

			//����i�Ԗڂ̔z��̒��g���u.�v�̎��A�`��ʒu�����炷�l��ݒ�
			if (totalSaboriTimeText[i] == '.')
			{
				offValue_x = -(32 / 2); // 32��src��w�l
				shouldDrewDecimalPoint = true;
			}

			//�v���C���[���Ƃ̃X�R�A�\�����킩��ʒu�ɕ`�悳���悤�ɂ���
			int drawingPositionCorrectionEachPlayer_x = playerNumbersDrawInfo_TotalSaboriTime[loopCount].pos.x + 75;	// �v���C���[���Ƃ̕`��ʒu�␳ 75�̓T�{�荇�v���Ԃ̔��ʗp�v���C���[�ԍ��̕`�����-50�����������+25���邽�߂̒l

			ML::Box2D src(src_x, src_y, 32, 32);
			ML::Box2D draw((src.w * 2) * i + offValue_x + drawingPositionCorrectionEachPlayer_x, 100, src.w * 2, src.h * 2); // x���W�̓v���C���[�ԍ��`��ʒu�̌�둤�Ay���W�̓v���C���[�ԍ��`��ʒu�Ɠ����ʒu�ɕ`�悳���悤�ɂ���

			this->res->totalSaboriTimeImage->Draw(draw, src);
		}
	}

	//-------------------------------------------------------------------
	//�v���C���[�ԍ��̈ʒu��V�ԃv���C���[�̐l���ɉ����Č��߂�
	void Object::DecidePlayerNumbersDrawInfo_pos(const int playerCount)
	{
		//�v�Z�Ɏg���ϐ��錾
		int sourceNumeratorValue = 0;	// ���ɂȂ镪�q�̒l
		int valueIncreasePerLoop = 0;	// ���[�v���Ƃɑ��₷�l

		//�v�Z�Ɏg���l�����߂�
		switch (playerCount)
		{
		case 1:
			sourceNumeratorValue = 4;
			break;

		case 2:
			sourceNumeratorValue = 2;
			valueIncreasePerLoop = 4;
			break;

		case 3:
			sourceNumeratorValue = 1;
			valueIncreasePerLoop = 3;
			break;

		case 4:
			sourceNumeratorValue = 1;
			valueIncreasePerLoop = 2;
			break;
		}

		//playerNumbersDrawInfo.pos��ݒ�
		for (int i = 0; i < playerCount; ++i)
		{
			playerNumbersDrawInfo[i].pos = ML::Vec2(ge->screen2DWidth * (sourceNumeratorValue + (valueIncreasePerLoop * i)) / 8.f, ge->screen2DHeight - 480.f);
		}
	}

	//-------------------------------------------------------------------
	//���v�T�{�莞�Ԃ̕`��ʒu�����߂�
	void Object::DecideTotalSaboriTimeDrawPos(const int playerCount)
	{
		//�v�Z�Ɏg���ϐ��錾
		int sourceNumeratorValue = 0;	// ���ɂȂ镪�q�̒l
		int valueIncreasePerLoop = 0;	// ���[�v���Ƃɑ��₷�l

		//�v�Z�Ɏg���l�����߂�
		switch (playerCount)
		{
		case 1:
			sourceNumeratorValue = 6;
			break;

		case 2:
			sourceNumeratorValue = 3;
			valueIncreasePerLoop = 6;
			break;

		case 3:
			sourceNumeratorValue = 2;
			valueIncreasePerLoop = 4;
			break;

		case 4:
			sourceNumeratorValue = 1;
			valueIncreasePerLoop = 3;
			break;
		}

		//playerNumbersDrawInfo_TotalSaboriTime.pos��ݒ�
		for (int i = 0; i < playerCount; ++i)
		{
			playerNumbersDrawInfo_TotalSaboriTime[i].pos = ML::Vec2(ge->screen2DWidth * (sourceNumeratorValue + (valueIncreasePerLoop * i)) / 12.f - 75.f, 100.f + (53.f / 1.5f));
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
		//�v���C���[���ʔԍ��֌W---------------------------------------------------------------------------------------
		playerNumbersDrawInfo{
			{ ML::Box2D(-78, -53, 155, 105), ML::Box2D(0, 0, 155, 105), ML::Vec2(0, 0) },	// 1P
			{ ML::Box2D(-96, -53, 192, 105), ML::Box2D(155, 0, 192, 105), ML::Vec2(0, 0) },	// 2P
			{ ML::Box2D(-88, -53, 175, 105), ML::Box2D(347, 0, 175, 105), ML::Vec2(0, 0) },	// 3P
			{ ML::Box2D(-97, -53, 193, 105), ML::Box2D(522, 0, 193, 105), ML::Vec2(0, 0) }	// 4P
		},
		playerNumbersDrawInfo_TotalSaboriTime{
			{ ML::Box2D(static_cast<int>(-78 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(155 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(0, 0, 155, 105), ML::Vec2(0, 0)},			// 1P
			{ ML::Box2D(static_cast<int>(-96 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(192 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(155, 0, 192, 105), ML::Vec2(0, 0)},	// 2P
			{ ML::Box2D(static_cast<int>(-88 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(175 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(347, 0, 175, 105), ML::Vec2(0, 0)},	// 3P
			{ ML::Box2D(static_cast<int>(-97 / 1.5f), static_cast<int>(-53 / 1.5f), static_cast<int>(193 / 1.5f), static_cast<int>(105 / 1.5f)), ML::Box2D(522, 0, 193, 105), ML::Vec2(0, 0)}	// 4P
		},
		playerCount(1),
		//�T�{�荇�v���ԕ`��֌W---------------------------------------------------------------------------------------
		totalSaboriTimeText()
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