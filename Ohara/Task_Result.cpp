//-------------------------------------------------------------------
//�ŏI���U���g��ʃ^�X�N
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_Result.h"
#include  "Task_ResultUIManager.h"
#include  "Task_ResultBG.h"

#include  "../Task_Title.h"
#include  "../sound.h"

namespace  Result
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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

		for (int i = 0; i < playerNum; ++i) {
			playersInfo.push_back({ i + 1, 0, 0 });
		}

		//���X�R�A�v�Z
		Ranking();

		//���^�X�N�̐���
		//UI�Ǘ�
		ResultUIManager::Object::Create(true);

		//�w�i
		//2�̉摜�ŉ��X�N���[�������
		for (int i = 0; i < 2; ++i)
		{
			auto BG = ResultBG::Object::Create(true);
			if (BG)
			{
				BG->pos.x = static_cast<float>(i * ge->screen2DWidth);
			}
		}

		//��BGM
		bgm::LoadFile("ResultBGM", "./data/sound/bgm/�G���f�B���O_tanoshiibouken.mp3");
		bgm::VolumeControl("ResultBGM", 95);
		bgm::Play("ResultBGM");

		//��SE
		se::LoadFile("PushButtonSE", "./data/sound/se/Common/����{�^��������40.wav");
		se::SetVolume("PushButtonSE", 100);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("���U���g���");
		ge->KillAll_G("�Ǘ�");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//��BGM�̏I��
			bgm::Stop("ResultBGM");

			//�������p���^�X�N�̐���
			auto next = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�����̏�ԂɕύX�ł���悤�ɂȂ�����A���̏�Ԃ܂ł̃J�E���g���n�߂�
		if (nextStateGoIs == true)
		{
			this->countUpToStateChange++;
		}

		//��Ԕ��f
		ResultStateTransition();
		//��ԂɑΉ����鏈��
		ProcessEachState();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//���U���g�̏�ԑJ��
	void Object::ResultStateTransition()
	{
		ResultState nowState = this->resultState; //�Ƃ肠�������݂̏�Ԃ���

		//��Ԃ̐؂�ւ�
		switch (nowState)
		{
		case ResultState::Preface:				//�O�u��
			if (this->nextStateGoIs == true)
			{
				for (int i = 0; i < playerNum; ++i)
				{
					//���ʔ��\��
					if (ge->players[i]->GetState().B2.down)
					{
						nowState = ResultState::ResultAnnouncement; nextStateGoIs = false;
					}
				}
				if (this->countUpToStateChange >= (int)(0.5f * gameFps)) { nowState = ResultState::ResultAnnouncement; nextStateGoIs = false; } //���ʔ��\��
			}
			break;

		case ResultState::ResultAnnouncement:	//���ʔ��\
			if (this->nextStateGoIs == true)
			{
				for (int i = 0; i < playerNum; ++i)
				{
					//���ʔ��\�I����
					if (ge->players[i]->GetState().B2.down)
					{
						//SE��炷
						se::Play("PushButtonSE");

						nowState = ResultState::End; nextStateGoIs = false;
					}
				}
			}
			break;
		}

		//��ԍX�V
		UpdateResultState(nowState);
	}
	//-------------------------------------------------------------------
	//���U���g�̏�ԕύX������
	void Object::UpdateResultState(ResultState nowState)
	{
		if (nowState != this->resultState)
		{
			this->resultState = nowState;
			this->countUpToStateChange = 0;

			//ResultUIManager�̕����X�V���s��
			auto UImanager = ge->GetTask<ResultUIManager::Object>("�Ǘ�");
			if (UImanager == nullptr)
			{
				return;
			}

			UImanager->DataChangesWhenUpdatingState();
		}
	}
	//-------------------------------------------------------------------
	//��Ԗ��̏���
	void Object::ProcessEachState()
	{
		switch (this->resultState)
		{
		case ResultState::Preface:				//�O�u��
			//Task_resultUIManager�ŃC�[�W���O����ꂽ��A�{�^����������Task_resultUIManager�́A��ʊO�ɏo�Ă����C�[�W���O���N�����鏈������������
			break;

		case ResultState::End:					//���ʔ��\�I��
			//�\���p�v���C���[������
			if (shouldKillTask == true)
			{
				ge->KillAll_G("��ʕ\���p");
				shouldKillTask = false;
			}

			//�^�C�g���ɖ߂�
			if (this->nextStateGoIs == true)
			{
				//�^�C�g����ʂɑJ��
				for (int i = 0; i < playerNum; ++i)
				{
					if (ge->players[i]->GetState().B2.down) {
						//SE��炷
						se::Play("PushButtonSE");

						ge->StartCounter("test", 0);
					}
					if (ge->getCounterFlag("test") == ge->LIMIT) {
						Kill();
					}
				}
			}

			break;
		}
	}
	//-------------------------------------------------------------------
	//���̏�ԂɕύX�ł���悤�ɂ���
	void Object::ChangeableToNextState()
	{
		this->nextStateGoIs = true;
	}
	//-------------------------------------------------------------------
	//���ʌ��߂̏���
	void Object::Ranking()
	{
		//MyPG����X�R�A���擾
		for (int i = 0; i < playersInfo.size(); ++i)
		{
			playersInfo[i].score = ge->GetScore(i);
		}

		//�X�R�A����������playersInfo����ёւ�
		sort(playersInfo.begin(), playersInfo.end(), [&](const PlayerInformation& scoreA, const PlayerInformation& scoreB) { return compare(scoreA, scoreB); });

		//���ʂ����߂�
		int currentRank = 1; //����鏇��
		for (int i = 0; i < playersInfo.size(); ++i)
		{
			//2��ڈȍ~�̃��[�v�ŁA�X�R�A�̒l���O�̃X�R�A�Ɠ����łȂ���Γ���鏇�ʂ�+1����
			if (i > 0 && playersInfo[i].score != playersInfo[i - 1].score)
			{
				currentRank = i + 1;
			}

			//���ʂ�����
			playersInfo[i].rank = currentRank;
		}
	}
	//-------------------------------------------------------------------
	//scoreA��scoreB�ɓ��ꂽ�ϐ���score�Ŕ�r���AscoreA���傫����true��Ԃ�
	bool Object::compare(const PlayerInformation& scoreA, const PlayerInformation& scoreB)
	{
		return scoreA.score > scoreB.score;
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
		resultState(ResultState::Preface),
		nextStateGoIs(false),
		countUpToStateChange(0),
		gameFps(60),
		shouldKillTask(true),
		playerNum(ge->players.size())
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