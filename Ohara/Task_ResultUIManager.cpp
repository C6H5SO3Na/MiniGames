//-------------------------------------------------------------------
//���U���g��ʂ�UI�Ǘ�
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ResultUIManager.h"

namespace  ResultUIManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->prefaceImage = DG::Image::Create("./data/Image/Ohara/Result Announcement.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->prefaceImage.reset();
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
		this->displayPattern = DecideDisplayPattern(); //�\���p�^�[������
		
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
		//��Task_Result�̏�Ԗ��̏������s��
		ProcessEachResultState();

		//���`�悪�I��������Ƃ�Task_Result�ɓ`����
		if (hasEndedDrawing == true)
		{
			//Task_Result�̏����擾����
			auto result = ge->GetTask<Result::Object>("���U���g���");
			if (result == nullptr)
			{
				return;
			}

			//Task_Result�̊֐����g�p���A���̏�ԂɕύX�ł���悤�ɂ���
			result->ChangeableToNextState();

			//�`��I������p�̕ϐ���false�ɖ߂�
			this->hasEndedDrawing = false;
			this->isChangedFalse_hasEndedDrawing = true; //hasEndedDrawing��false�ɂ����̂�true�ɕύX
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//��Task_Result�̏�Ԗ��̕`��
		DrawingProcessingEachResultState();
	}
	//-------------------------------------------------------------------
	//Task_Result�̏�Ԗ��̏���
	void Object::ProcessEachResultState()
	{
		//Task_Result�̏����擾
		auto result = ge->GetTask<Result::Object>("���U���g���");
		if (result == nullptr)
		{
			return;
		}

		//��Ԗ��̏���
		switch (result->resultState)
		{
		case Result::Object::ResultState::ResultAnnouncement:	//���ʔ��\
			this->drawUpToCount++;

			//�\���p�v���C���[�[�̐���
			this->GenerateDisplayPlayer();
			break;
		}

	}
	//-------------------------------------------------------------------
	//�\���p�v���C���[�̐���
	void Object::GenerateDisplayPlayer()
	{
		//Task_Result�̏����擾
		auto result = ge->GetTask<Result::Object>("���U���g���");
		if (result == nullptr)
		{
			return;
		}

		//�\���p�v���C���[�̐����^�C�~���O���p�^�[�������AdisplayPattern�̒l�Ō��肷��
		switch (this->displayPattern)
		{
		case 0: //1�ʎl�l
			//1�ʂ̉摜�̃v���C���[���l�l��������
			if (drawUpToCount == static_cast<int>(3.f * gameFps))
			{
				//1�ʂ̐���
				for (int i = 0; i < 4; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i], playerNumberPosition[i], rankSentencePosition[i], scoreDrawPosition[i]);
				}
			}
			break;

		case 1: //1�ʎO�l�A4�ʈ�l
			//1�ʂ̉摜�̃v���C���[���O�l�A4�ʂ̉摜�̃v���C���[����l��������
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//4�ʂ̐���
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(5.f * gameFps))
			{
				//1�ʂ̐���
				for (int i = 0; i < 3; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i], playerNumberPosition[i], rankSentencePosition[i], scoreDrawPosition[i]);
				}
			}
			break;

		case 2: //1�ʓ�l�A3�ʓ�l
			//1�ʂ̉摜�̃v���C���[���l�A3�ʂ̉摜�̃v���C���[���l��������
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//3�ʂ̐���
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i + 2]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i + 2], playerNumberPosition[i + 2], rankSentencePosition[i + 2], scoreDrawPosition[i + 2]);
				}
			}
			if (drawUpToCount == static_cast<int>(5.f * gameFps))
			{
				//1�ʂ̐���
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i], playerNumberPosition[i], rankSentencePosition[i], scoreDrawPosition[i]);
				}
			}
			break;

		case 3: //1�ʓ�l�A3�ʈ�l�A4�ʈ�l
			//1�ʂ̉摜�̃v���C���[���l�A3�ʂ̉摜�̃v���C���[����l�A4�ʂ̉摜�̃v���C���[����l��������
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//4�ʂ̐���
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(4.f * gameFps))
			{
				//3�ʂ̐���
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[2]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[2], playerNumberPosition[2], rankSentencePosition[2], scoreDrawPosition[2]);
			}
			if (drawUpToCount == static_cast<int>(6.f * gameFps))
			{
				//1�ʂ̐���
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i], playerNumberPosition[i], rankSentencePosition[i], scoreDrawPosition[i]);
				}
			}
			break;

		case 4: //1�ʈ�l�A2�ʎO�l
			//1�ʂ̉摜�̃v���C���[����l�A2�ʂ̉摜�̃v���C���[���O�l��������
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//2�ʂ̐���
				for (int i = 0; i < 3; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i + 1]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i + 1], playerNumberPosition[i + 1], rankSentencePosition[i + 1], scoreDrawPosition[i + 1]);
				}
			}
			if (drawUpToCount == static_cast<int>(5.f * gameFps))
			{
				//1�ʂ̐���
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[0]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[0], playerNumberPosition[0], rankSentencePosition[0], scoreDrawPosition[0]);
			}
			break;

		case 5: //1�ʈ�l�A2�ʓ�l�A4�ʈ�l
			//1�ʂ̉摜�̃v���C���[����l�A2�ʂ̉摜�̃v���C���[���l�A4�ʂ̉摜�̃v���C���[����l��������
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//4�ʂ̐���
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(5.f * gameFps))
			{
				//2�ʂ̐���
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i + 1]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i + 1], playerNumberPosition[i + 1], rankSentencePosition[i + 1], scoreDrawPosition[i + 1]);
				}

				//1�ʂ̐���
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[0]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[0], playerNumberPosition[0], rankSentencePosition[0], scoreDrawPosition[0]);
			}

		case 6: //1�ʈ�l�A2�ʈ�l�A3�ʓ�l
			//1�ʂ̉摜�̃v���C���[����l�A2�ʂ̉摜�̃v���C���[����l�A3�ʂ̉摜�̃v���C���[���l��������
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//3�ʂ̐���
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i + 2]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i + 2], playerNumberPosition[i + 2], rankSentencePosition[i + 2], scoreDrawPosition[i + 2]);
				}
			}
			if (drawUpToCount == static_cast<int>(5.f * gameFps))
			{
				//2�ʂ̐���
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[1]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[1], playerNumberPosition[1], rankSentencePosition[1], scoreDrawPosition[1]);

				//1�ʂ̐���
				displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[0]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[0], playerNumberPosition[0], rankSentencePosition[0], scoreDrawPosition[0]);
			}
			break;

		case 7: //�����Ȃ�
			//1�ʂ̉摜�̃v���C���[����l�A2�ʂ̉摜�̃v���C���[����l�A3�ʂ̉摜�̃v���C���[����l�A4�ʂ̉摜�̃v���C���[����l��������
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//4�ʂ̐���
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(4.f * gameFps))
			{
				//3�ʂ̐���
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[2]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[2], playerNumberPosition[2], rankSentencePosition[2], scoreDrawPosition[2]);
			}
			if (drawUpToCount == static_cast<int>(6.f * gameFps))
			{
				//2�ʂ̐���
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[1]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[1], playerNumberPosition[1], rankSentencePosition[1], scoreDrawPosition[1]);

				//1�ʂ̐���
				displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[0]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[0], playerNumberPosition[0], rankSentencePosition[0], scoreDrawPosition[0]);
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//Task_Result�̏�Ԗ��̕`�揈��
	void Object::DrawingProcessingEachResultState()
	{
		//Task_Result�̏����擾
		auto result = ge->GetTask<Result::Object>("���U���g���");
		if (result == nullptr)
		{
			return;
		}

		//�`���񏀔�
		int srcValue[4] = {};
		int drawValue[4] = {};
		ML::Box2D draw = {};
		ML::Box2D src = {};

		//��Ԗ��̕`�揈��
		switch (result->resultState)
		{
		case Result::Object::ResultState::Preface:				//�O�u��
			//���`��
			//�`����ݒ�
			for (int i = 0; i < 4; ++i)
			{
				srcValue[i] = this->srcValues[0][i];
				drawValue[i] = this->drawValues[0][i];
			}
			draw = { drawValue[0], drawValue[1], drawValue[2], drawValue[3] };
			src = {srcValue[0], srcValue[1], srcValue[2], srcValue[3]};

			this->res->prefaceImage->Draw(draw, src);

			//�C�[�W���O����ꂽ��A�C�[�W���O���I������Ƃ���Task_Result���́A�u�{�^���������Ƃ��̃^�X�N�́A��ʊO�ɏo�Ă����C�[�W���O���N�����鏈���v�i���̏������V�������j��ʂ�悤�ɂȂ�ϐ���true�ɂ���
			if (this->isChangedFalse_hasEndedDrawing == false) //��Ԃ�Preface�̎��A1�񂾂�hasEndedDrawing��true�ɂȂ�悤�ɂ���
			{
				this->hasEndedDrawing = true;
			}
			break;

		case Result::Object::ResultState::ResultAnnouncement:	//���ʔ��\
			//���`��
			if (this->displayPattern == 0)
			{
				if (drawUpToCount >= static_cast<int>(4.f * gameFps))
				{
					//�u?�{�^���������Ď��ցv�ƕ`��

					//�`�悪�I��������Ƃ�`����
					if (this->isChangedFalse_hasEndedDrawing == false) //��Ԃ�ResultAnnouncement�̎��A1�񂾂�hasEndedDrawing��true�ɂȂ�悤�ɂ���
					{
						this->hasEndedDrawing = true;
					}
				}
			}
			else
			{
				if (drawUpToCount >= static_cast<int>(7.f * gameFps))
				{
					//�u?�{�^���������Ď��ցv�ƕ`��

					//�`�悪�I��������Ƃ�`����
					if (this->isChangedFalse_hasEndedDrawing == false) //��Ԃ�ResultAnnouncement�̎��A1�񂾂�hasEndedDrawing��true�ɂȂ�悤�ɂ���
					{
						this->hasEndedDrawing = true;
					}
				}
			}
			break;

		case Result::Object::ResultState::End:
			//�u�V��ł���Ă��肪�Ƃ��v�ƕ`��
			//�u?�{�^���������ă^�C�g���ցv�ƕ`��
			
			//�`�悪�I��������Ƃ�`����
			if (this->isChangedFalse_hasEndedDrawing == false) //��Ԃ�ResultAnnouncement�̎��A1�񂾂�hasEndedDrawing��true�ɂȂ�悤�ɂ���
			{
				this->hasEndedDrawing = true;
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//��ԍX�V���ɂ���f�[�^(�ϐ�)�̕ύX
	void Object::DataChangesWhenUpdatingState()
	{
		this->isChangedFalse_hasEndedDrawing = false;
	}
	//-------------------------------------------------------------------
	//���ʔ��\���̕\���p�^�[�������߂�
	int Object::DecideDisplayPattern()
	{
		//Task_Result��������擾
		auto result = ge->GetTask<Result::Object>("���U���g���");
		if (result == nullptr)
		{
			return 0;
		}

		int rankCount[4] = {}; //�v�f��0��1�ʁA1��2�ʁE�E�E�Ƃ��ď��ʂ̐����J�E���g����

		//�����ʂ̐��̃J�E���g
		for (int i = 0; i < sizeof(result->playersInfo) / sizeof(result->playersInfo[0]); ++i)
		{
			switch (result->playersInfo[i].rank)
			{
			case 1:
				rankCount[0]++;
				break;

			case 2:
				rankCount[1]++;
				break;

			case 3:
				rankCount[2]++;
				break;

			case 4:
				rankCount[3]++;
				break;
			}
		}

		//���\���p�^�[������
		int rtv = 0;
		const int patternNumber = 8;			//�p�^�[���̐�
		int countPattern[patternNumber][4] = {	//�J�E���g�̃p�^�[��
			{4, 0, 0, 0}, //�p�^�[��0�@1�ʎl�l
			{3, 0, 0, 1}, //�p�^�[��1�@1�ʎO�l�A4�ʈ�l
			{2, 0, 2, 0}, //�p�^�[��2�@1�ʓ�l�A3�ʓ�l
			{2, 0, 1, 1}, //�p�^�[��3�@1�ʓ�l�A3�ʈ�l�A4�ʈ�l
			{1, 3, 0, 0}, //�p�^�[��4�@1�ʈ�l�A2�ʎO�l
			{1, 2, 0, 1}, //�p�^�[��5�@1�ʈ�l�A2�ʓ�l�A4�ʈ�l
			{1, 1, 2, 0}, //�p�^�[��6�@1�ʈ�l�A2�ʈ�l�A3�ʓ�l
			{1, 1, 1, 1}  //�p�^�[��7�@�����Ȃ�
		};
		int sameCount = 0;	//���ʂ̃J�E���g�ƃp�^�[���̒l�������Ă�����+1����@sameCount == 4�ɂȂ����p�^�[���̐���Ԃ�l�Ƃ���
		
		for (int i = 0; i < patternNumber; ++i)
		{
			//�J�E���g�ƃp�^�[�����Ƃ炵���킹��
			for (int j = 0; j < sizeof(rankCount) / sizeof(rankCount[0]); ++j)
			{
				if (rankCount[j] == countPattern[i][j])
				{
					sameCount++;
				}
			}

			//�J�E���g�ƃp�^�[���̑S�Ă̒l��������������A���̃p�^�[����Ԃ�l�Ƃ��Đݒ肵�Afor���𔲂���
			if (sameCount == 4) 
			{
				rtv = i;
				break;
			}
			else //�l����ł��������J�E���g�����Z�b�g����
			{
				sameCount = 0;
			}
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
		drawUpToCount(0),
		gameFps(60),
		hasEndedDrawing(false),
		isChangedFalse_hasEndedDrawing(false),
		displayPattern(0)
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