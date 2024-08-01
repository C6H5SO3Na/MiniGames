//-------------------------------------------------------------------
//���U���g��ʂ�UI�Ǘ�
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ResultUIManager.h"

#include  "../easing.h"
#include  "../sound.h"

namespace  ResultUIManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->prefaceImage = DG::Image::Create("./data/image/ResultAnnouncement.png");
		this->buttonImage_A = DG::Image::Create("./data/image/button/double/xbox_button_color_b.png");
		this->buttonImage_A_Outline = DG::Image::Create("./data/image/button/double/xbox_button_color_b_outline.png");
		this->thankyouImage = DG::Image::Create("./data/image/ThankyouForPlaying.png");
		this->directToTitleImage = DG::Image::Create("./data/image/DirectToTitle.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->prefaceImage.reset();
		this->buttonImage_A.reset();
		this->buttonImage_A_Outline.reset();
		this->thankyouImage.reset();
		this->directToTitleImage.reset();
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
		this->displayPattern = DecideDisplayPattern(); //�\���p�^�[������
		
		//���^�X�N�̐���

		//���C�[�W���O
		//�����摜�ړ��p
		//�u���ʔ��\�v�ړ��p
		easing::Set("StartMovePrefaceImage", easing::CIRCOUT, static_cast<float>(ge->screen2DWidth + srcValues[0][2] * 2), ge->screen2DWidth / 2.f, this->gameFps, "EndMovePrefaceImage");
		easing::Set("EndMovePrefaceImage", easing::CIRCIN, ge->screen2DWidth / 2.f, static_cast<float>(-srcValues[0][2] * 2), this->gameFps);

		//��SE
		//�L�����N�^�[�\������SE
		se::LoadFile("CharacterAppearanceSE", "./data/sound/se/Ending/maou_se_onepoint03.wav");
		se::SetVolume("CharacterAppearanceSE", 10);

		//1�ʕ\������SE
		se::LoadFile("FirstPlayerDisplaySE", "./data/sound/se/Ending/�����Ɣ���.wav");
		se::SetVolume("FirstPlayerDisplaySE", 100);

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
		//�C�[�W���O�𓮂���
		easing::UpDate();

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
		case Result::Object::ResultState::Preface:
			//�����U���g���n�܂����u�Ԃɍs������
			if (this->resultStart == true)
			{
				//���C�[�W���O�J�n
				easing::Start("StartMovePrefaceImage");

				this->resultStart = false;
			}

			//���C�[�W���O�ō��W�ړ�
			//Ready�𓮂���
			this->prefaceImagePos.x = easing::GetPos("StartMovePrefaceImage");
			if (easing::GetState("StartMovePrefaceImage") == easing::EQ_STATE::EQ_END) //�C�[�W���O�uReadyStart�v���I�������
			{
				this->prefaceImagePos.x = easing::GetPos("EndMovePrefaceImage");
			}
			break;

		case Result::Object::ResultState::ResultAnnouncement:	//���ʔ��\
			this->drawUpToCount++;
			this->animationCount++;

			//�\���p�v���C���[�[�̐���
			this->GenerateDisplayPlayer();
			break;

		case Result::Object::ResultState::End:
			this->animationCount++;
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
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//1�ʂ̐���
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
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
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//4�ʂ̐���
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(3.f * gameFps))
			{
				//1�ʂ̐���
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
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
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//3�ʂ̐���
				se::Play("CharacterAppearanceSE");
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i + 2]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i + 2], playerNumberPosition[i + 2], rankSentencePosition[i + 2], scoreDrawPosition[i + 2]);
				}
			}
			if (drawUpToCount == static_cast<int>(3.f * gameFps))
			{
				//1�ʂ̐���
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
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
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//4�ʂ̐���
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//3�ʂ̐���
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[2]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[2], playerNumberPosition[2], rankSentencePosition[2], scoreDrawPosition[2]);
			}
			if (drawUpToCount == static_cast<int>(4.f * gameFps))
			{
				//1�ʂ̐���
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
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
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//2�ʂ̐���
				se::Play("CharacterAppearanceSE");
				for (int i = 0; i < 3; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i + 1]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i + 1], playerNumberPosition[i + 1], rankSentencePosition[i + 1], scoreDrawPosition[i + 1]);
				}
			}
			if (drawUpToCount == static_cast<int>(3.f * gameFps))
			{
				//1�ʂ̐���
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[0]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[0], playerNumberPosition[0], rankSentencePosition[0], scoreDrawPosition[0]);
			}
			break;

		case 5: //1�ʈ�l�A2�ʓ�l�A4�ʈ�l
			//1�ʂ̉摜�̃v���C���[����l�A2�ʂ̉摜�̃v���C���[���l�A4�ʂ̉摜�̃v���C���[����l��������
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//4�ʂ̐���
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(3.f * gameFps))
			{
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
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
			break;

		case 6: //1�ʈ�l�A2�ʈ�l�A3�ʓ�l
			//1�ʂ̉摜�̃v���C���[����l�A2�ʂ̉摜�̃v���C���[����l�A3�ʂ̉摜�̃v���C���[���l��������
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//3�ʂ̐���
				se::Play("CharacterAppearanceSE");
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i + 2]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i + 2], playerNumberPosition[i + 2], rankSentencePosition[i + 2], scoreDrawPosition[i + 2]);
				}
			}
			if (drawUpToCount == static_cast<int>(3.f * gameFps))
			{
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
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
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//4�ʂ̐���
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//3�ʂ̐���
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[2]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[2], playerNumberPosition[2], rankSentencePosition[2], scoreDrawPosition[2]);
			}
			if (drawUpToCount == static_cast<int>(4.f * gameFps))
			{
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
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
		ML::Box2D draw = {};
		ML::Box2D src = {};

		//��Ԗ��̕`�揈��
		switch (result->resultState)
		{
		case Result::Object::ResultState::Preface:				//�O�u��
			//���`��
			//�`����ݒ�
			draw = ML::Box2D(this->drawValues[0][0], this->drawValues[0][1], this->drawValues[0][2], this->drawValues[0][3]);
			src = ML::Box2D(this->srcValues[0][0], this->srcValues[0][1], this->srcValues[0][2], this->srcValues[0][3]);
			draw.Offset(this->prefaceImagePos);

			this->res->prefaceImage->Draw(draw, src);

			if (easing::GetState("EndMovePrefaceImage") == easing::EQ_STATE::EQ_END) //�u���ʔ��\�v�̉摜�̈ړ����I�������
			{
				if (this->isChangedFalse_hasEndedDrawing == false) //��Ԃ�Preface�̎��A1�񂾂�hasEndedDrawing��true�ɂȂ�悤�ɂ���
				{
					this->hasEndedDrawing = true;
				}
			}
			break;

		case Result::Object::ResultState::ResultAnnouncement:	//���ʔ��\
			//���`��
			if (this->displayPattern == 0)
			{
				if (drawUpToCount >= static_cast<int>(3.f * gameFps))
				{
					//��A�{�^����`��
					//�`����ݒ�
					int animationNum = this->animationCount / 15;
					animationNum %= 2;
					draw = ML::Box2D(this->drawValues[1][0], this->drawValues[1][1], this->drawValues[1][2], this->drawValues[1][3]);
					src = ML::Box2D(this->srcValues[1][0], this->srcValues[1][1], this->srcValues[1][2], this->srcValues[1][3]);

					if (animationNum == 0)	//A�{�^���̉摜�`��
					{
						this->res->buttonImage_A->Draw(draw, src);
					}
					else					//A�{�^���O�g�݂̂̉摜�`��
					{
						this->res->buttonImage_A_Outline->Draw(draw, src);
					}

					//�`�悪�I��������Ƃ�`����
					if (this->isChangedFalse_hasEndedDrawing == false) //��Ԃ�ResultAnnouncement�̎��A1�񂾂�hasEndedDrawing��true�ɂȂ�悤�ɂ���
					{
						this->hasEndedDrawing = true;
					}
				}
			}
			else
			{
				if (drawUpToCount >= static_cast<int>(5.f * gameFps))
				{
					//��A�{�^���`��
					//�`����ݒ�
					int animationNum = this->animationCount / 15;
					animationNum %= 2;
					draw = ML::Box2D(this->drawValues[1][0], this->drawValues[1][1], this->drawValues[1][2], this->drawValues[1][3]);
					src = ML::Box2D(this->srcValues[1][0], this->srcValues[1][1], this->srcValues[1][2], this->srcValues[1][3]);

					if (animationNum == 0)	//A�{�^���̉摜�`��
					{
						this->res->buttonImage_A->Draw(draw, src);
					}
					else					//A�{�^���O�g�݂̂̉摜�`��
					{
						this->res->buttonImage_A_Outline->Draw(draw, src);
					}

					//�`�悪�I��������Ƃ�`����
					if (this->isChangedFalse_hasEndedDrawing == false) //��Ԃ�ResultAnnouncement�̎��A1�񂾂�hasEndedDrawing��true�ɂȂ�悤�ɂ���
					{
						this->hasEndedDrawing = true;
					}
				}
			}
			break;

		case Result::Object::ResultState::End:
			//���u�V��ł���Ă��肪�Ƃ��v�ƕ`��
			//�`����ݒ�
			draw = ML::Box2D(this->drawValues[2][0], this->drawValues[2][1], this->drawValues[2][2], this->drawValues[2][3]);
			src = ML::Box2D(this->srcValues[2][0], this->srcValues[2][1], this->srcValues[2][2], this->srcValues[2][3]);

			this->res->thankyouImage->Draw(draw, src);
			
			//���uA�{�^���Ń^�C�g���ցv�ƕ`��
			//A�{�^���`��
			//�`����ݒ�
			int animationNum = this->animationCount / 15;
			animationNum %= 2;
			draw = ML::Box2D(this->drawValues[3][0], this->drawValues[3][1], this->drawValues[3][2], this->drawValues[3][3]);
			src = ML::Box2D(this->srcValues[1][0], this->srcValues[1][1], this->srcValues[1][2], this->srcValues[1][3]);

			if (animationNum == 0)	//A�{�^���̉摜�`��
			{
				this->res->buttonImage_A->Draw(draw, src);
			}
			else					//A�{�^���O�g�݂̂̉摜�`��
			{
				this->res->buttonImage_A_Outline->Draw(draw, src);
			}

			//�u�{�^���Ń^�C�g���ցv�`��
			//�`����ݒ�
			draw = ML::Box2D(this->drawValues[4][0], this->drawValues[4][1], this->drawValues[4][2], this->drawValues[4][3]);
			src = ML::Box2D(this->srcValues[3][0], this->srcValues[3][1], this->srcValues[3][2], this->srcValues[3][3]);

			this->res->directToTitleImage->Draw(draw, src);

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
		this->drawUpToCount = 0;
		this->animationCount = 0;
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
		animationCount(0),
		gameFps(60),
		hasEndedDrawing(false),
		isChangedFalse_hasEndedDrawing(false),
		displayPattern(0),
		resultStart(true)
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