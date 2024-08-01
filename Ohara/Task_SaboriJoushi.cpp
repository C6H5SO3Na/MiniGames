//-------------------------------------------------------------------
//�T�{��~�j�Q�[���̏�i
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriJoushi.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriGame.h"

#include  "../randomLib.h"
#include  "../sound.h"

namespace  SaboriJoushi
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->image = DG::Image::Create("./data/image/game_boss.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->image.reset();
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
		this->state = State::JWork;
		this->changeStateTime = GetRandom(2.f, 4.f); //�d������Ԃ���T�{��m�F�̗\�������ԂɕύX����܂ł̎��Ԃ̐ݒ�
		
		//���^�X�N�̐���

		//��SE
		se::LoadFile("FoundSaboriSE", "./data/sound/se/SaboriGame/���C�I���̖���1.wav");
		se::SetVolume("FoundSaboriSE", 100);

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
		auto game = ge->GetTask<SaboriGame::Object>(SaboriGame::defGroupName, SaboriGame::defName);

		//���ڂ�~�j�Q�[���^�X�N���擾�ł��Ă��邩�m�F
		if (game == nullptr)
		{
			return;
		}

		//�~�j�Q�[�����̏���
		if (game->isInGame == true)
		{
			this->moveCnt++;

			//��Ԕ��f
			this->Think();
			//��ԂɑΉ�����s������
			this->Move();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���`��
		DrawInformation drawImage = GetDrawImage();
		drawImage.draw.Offset(this->pos);

		this->res->image->Draw(drawImage.draw, drawImage.src);
	}
	//-------------------------------------------------------------------
	//��i�̏�Ԑ���
	void Object::Think()
	{
		State nowState = this->state; //�Ƃ肠�������݂̏�Ԃ���

		//���[�V�����̐؂�ւ�
		switch (nowState)
		{
		case State::JWork:						//�d�������
			if (this->moveCnt >= (int)(this->changeStateTime * this->gameFps)) { nowState = State::JBeforeCheckingForSabori; } //�T�{��m�F�̗\�������Ԃ� ���j�^�[FPS�ɃQ�[�����ˑ����Ȃ��悤�ɂ��邽�߂ɏ������� * GetFps() / (float)gameFps ��ǉ�����
			break;

		case State::JBeforeCheckingForSabori:	//�T�{��m�F�̗\��������
			if (this->moveCnt >= (int)(this->changeStateTime * this->gameFps)) //���j�^�[FPS�ɃQ�[�����ˑ����Ȃ��悤�ɂ��邽�߂ɏ������� * GetFps() / (float)gameFps ��ǉ�����
			{
				//�������_���őJ�ڂ����Ԃ�ύX����
				//�J�ڂ����Ԃ�����
				this->changeStateKinds = GetRandom(0, 3);

				//1/4�Ŏd������ԁA3/4�ŃT�{��m�F��Ԃ�
				if (this->changeStateKinds == 0) { nowState = State::JWork; } //�d������Ԃ�
				else{ nowState = State::JCheckForSabori; } //�T�{��m�F��Ԃ�
			}
			break;

		case State::JCheckForSabori:			//�T�{��m�F���
			if (this->moveCnt >= (int)(this->changeStateTime * this->gameFps)) { nowState = State::JWork; } //�d������Ԃ� ���j�^�[FPS�ɃQ�[�����ˑ����Ȃ��悤�ɂ��邽�߂ɏ������� * GetFps() / (float)gameFps ��ǉ�����
			if (this->hasFoundPlayerSabori) { nowState = State::JFoundSabori; } //�T�{�蔭����Ԃ�
			break;

		case State::JFoundSabori:				//�T�{�蔭�����
			if (this->moveCnt >= 1 * this->gameFps) { nowState = State::JWork; } //�d������Ԃ� ���j�^�[FPS�ɃQ�[�����ˑ����Ȃ��悤�ɂ��邽�߂ɏ������� * GetFps() / (float)gameFps ��ǉ�����
			break;
		}

		//��ԍX�V
		this->UpdateState(nowState);
	}
	//-------------------------------------------------------------------
	//��Ԗ��̍s������
	void Object::Move()
	{
		switch (this->state)
		{
		case State::JWork:						//�d�������
			//���d������Ԃ���T�{��m�F�̗\�������ԂɕύX����܂ł̎��Ԃ̐ݒ�
			if (this->moveCnt == 0) //��Ԃ��؂�ւ�������̂ݏ�������
			{
				//���Ԑݒ�
				this->changeStateTime = GetRandom(2.f, 4.f); //2�`4�b
			}
			break;

		case State::JBeforeCheckingForSabori:	//�T�{��m�F�̗\��������
			//���T�{��m�F�̗\�������Ԃ���T�{��m�F��Ԃ܂��͎d������ԂɕύX����܂ł̎��Ԃ̐ݒ�
			if (this->moveCnt == 0) //��Ԃ��؂�ւ�������̂ݏ�������
			{
				//���Ԑݒ�
				this->changeStateTime = GetRandom(1.f, 2.f); //1�`2�b
			}
			break;

		case State::JCheckForSabori:			//�T�{��m�F���
			//���T�{��m�F��Ԃ���d������ԂɕύX����܂ł̎��Ԃ̐ݒ�
			if (this->moveCnt == 0) //��Ԃ��؂�ւ�������̂ݏ�������
			{
				//���Ԑݒ�
				this->changeStateTime = GetRandom(1.f, 2.f); //1�`3�b
			}

			//���v���C���[�����ڂ��Ԃ��ǂ������肷��
			CheckPlayerSabori();
			break;

		case State::JFoundSabori:				//�T�{�蔭�����
			//���T�{��𔭌������Ƃ��ɍs������
			if (this->hasFoundPlayerSabori)
			{
				this->hasFoundPlayerSabori = false;
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//�A�j���[�V��������
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(static_cast<int>(-218 / 1.5f), static_cast<int>(-246 / 1.5f), static_cast<int>(436 / 1.5f), static_cast<int>(492 / 1.5f)), ML::Box2D(0, 0, 436, 492)},		//�d�����
			{ ML::Box2D(static_cast<int>(-218 / 1.5f), static_cast<int>(-246 / 1.5f), static_cast<int>(436 / 1.5f), static_cast<int>(492 / 1.5f)), ML::Box2D(436, 0,436, 492) },	//�T�{��m�F�̗\��������
			{ ML::Box2D(static_cast<int>(-218 / 1.5f), static_cast<int>(-246 / 1.5f), static_cast<int>(436 / 1.5f), static_cast<int>(492 / 1.5f)), ML::Box2D(1308, 0, 436, 492) },	//�T�{��m�F���
			{ ML::Box2D(static_cast<int>(-218 / 1.5f), static_cast<int>(-246 / 1.5f), static_cast<int>(436 / 1.5f), static_cast<int>(492 / 1.5f)), ML::Box2D(1744, 0, 436, 492)},	//�T�{�蔭�����
		};

		DrawInformation rtv;
		switch (this->state)
		{
		case State::JWork:						//�d�������
			rtv = imageTable[0];
			break;

		case State::JBeforeCheckingForSabori:	//�T�{��m�F�̗\��������
			rtv = imageTable[1];
			break;

		case State::JCheckForSabori:			//�T�{��m�F���
			rtv = imageTable[2];
			break;

		case State::JFoundSabori:				//�T�{�蔭�����
			rtv = imageTable[3];
			break;
		}

		return rtv;
	}
	//-------------------------------------------------------------------
	//�v���C���[�����ڂ��Ԃ��ǂ������肷��
	void Object::CheckPlayerSabori()
	{
		//�v���C���[�S�Ă𒊏o����
		auto players = ge->GetTasks<SaboriPlayer::Object>("�v���C���[");
		for (auto p = players->begin(); p != players->end(); ++p)
		{
			if ((*p)->state == State::PSabori)
			{
				//SE��炷
				se::Play("FoundSaboriSE");
				//��i�����ڂ�𔭌�
				this->hasFoundPlayerSabori = true;
				//�v���C���[�����ڂ�ɋC�Â����
				(*p)->noticedToSabori = true;
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
	Object::Object() 
		:
		changeStateTime(0),
		changeStateKinds(0),
		hasFoundPlayerSabori(false)
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