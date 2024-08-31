//-------------------------------------------------------------------
//�T�{��~�j�Q�[���̃v���C���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SaboriPlayer.h"
#include  "Task_SaboriGame.h"

#include  "../fpscounter.h"
#include  "../sound.h"

namespace  SaboriPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->image = DG::Image::Create("./data/image/game_otsan_working.png");
		this->buttonImage_A = DG::Image::Create("./data/image/button/Double/xbox_button_color_a.png");
		this->buttonImage_A_Outline = DG::Image::Create("./data/image/button/Double/xbox_button_color_a_outline.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->image.reset();
		this->buttonImage_A.reset();
		this->buttonImage_A_Outline.reset();
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
		this->state = State::PWork;
		
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
		DrawInformation drawImage = this->GetDrawImage();
		drawImage.draw.Offset(this->pos);
		
		this->res->image->Draw(drawImage.draw, drawImage.src);

		//��A�{�^���`��
		this->DrawButton();
	}
	//-------------------------------------------------------------------
	//���݂̃v���C���[�̏�Ԑ���
	void Object::Think()
	{
		auto input = this->controller->GetState();
		State nowState = this->state;	//�Ƃ肠�������݂̏�Ԃ���

		//���[�V�����̐؂�ւ�
		switch (nowState)
		{
		case State::PWork:		//�d�������
			if (input.B1.on) { nowState = State::PSabori; } //�T�{���Ԃ�
			break;

		case State::PSabori:	//�T�{����
			if (input.B1.up) { nowState = State::PWork; } //�d������Ԃ�
			if (noticedToSabori) { nowState = State::PNoticed; } //�T�{��΂��Ԃ�
			break;

		case State::PNoticed:	//�T�{��΂���
			if (this->moveCnt >= this->gameFps * 3) { nowState = State::PWork; } //�d������Ԃ� ���j�^�[FPS�ɃQ�[�����ˑ����Ȃ��悤�ɂ��邽�߂ɏ������� * GetFps() / (float)gameFps ����
			break;
		}

		//��ԍX�V
		this->UpdateState(nowState);
	}
	//-------------------------------------------------------------------
	//��Ԗ��̍s������
	void Object::Move()
	{
		//���Q�[���{�ҊJ�n���Ɉ�x�����s������

		if (this->isPlayStartSE == false)
		{
			//SE�̖��O�t��
			this->workSEName = "WorkSE" + to_string((int)this->playerNum);
			this->saboriSEName = "SaoriSE" + to_string((int)this->playerNum);

			//SE�ݒ�
			//�d�������
			se::LoadFile(workSEName, "./data/sound/se/SaboriGame/PC-Keyboard05-14(Far-Hard).wav");
			se::SetVolume(workSEName, 100);

			//�T�{����
			se::LoadFile(saboriSEName, "./data/sound/se/SaboriGame/maou_se_8bit08.wav");
			se::SetVolume(saboriSEName, 1);

			//SE��炷
			se::PlayLoop(workSEName);

			//�{�^���̕`����J�n����
			this->buttonDrawPos = ML::Vec2(this->pos.x, this->pos.y - 400);
			this->isStartButtonDraw = true;

			this->isPlayStartSE = true;
		}

		switch (this->state)
		{
		case State::PWork:		//�d�������

			//����ԕύX����1�񂾂��s��
			if (this->moveCnt == 0)
			{
				//SE���~�߂�
				se::Stop(saboriSEName);

				//SE��炷
				se::PlayLoop(workSEName);
			}

			break;

		case State::PSabori:	//�T�{����
			//��ԕύX����1�񂾂��s������
			if (this->moveCnt == 0)
			{
				//SE���~�߂�
				se::Stop(workSEName);

				//SE��炷
				se::PlayLoop(saboriSEName);
			}
			
			this->totalSaboriTime += 1.f / gameFps; // / gameFps �� / GetFps() ���ɕύX���ă��j�^�[FPS�ɃQ�[�����ˑ����Ȃ��悤�ɂ���
			break;

		case State::PNoticed:
			//��ԕύX����1�񂾂��s������
			if (this->moveCnt == 0)
			{
				//SE���~�߂�
				se::Stop(saboriSEName);
			}
			this->noticedToSabori = false;
			break;
		}
	}
	//-------------------------------------------------------------------
	//�A�j���[�V��������
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(static_cast<int>(-250 / 1.5f) , static_cast<int>(-230 / 1.5f), static_cast<int>(500 / 1.5f), static_cast<int>(460 / 1.5f)), ML::Box2D(500, 0, 500, 460)},	//�d�����
			{ ML::Box2D(static_cast<int>(-250 / 1.5f) , static_cast<int>(-230 / 1.5f), static_cast<int>(500 / 1.5f), static_cast<int>(460 / 1.5f)), ML::Box2D(1000, 0, 500, 460) },	//�T�{����
			{ ML::Box2D(static_cast<int>(-250 / 1.5f) , static_cast<int>(-230 / 1.5f), static_cast<int>(500 / 1.5f), static_cast<int>(460 / 1.5f)), ML::Box2D(1500, 0, 500, 460) },	//�T�{��΂���
		};

		DrawInformation rtv;
		switch (this->state)
		{
		case State::PWork:		//�d�������
			rtv = imageTable[0];
			break;

		case State::PSabori:		//�T�{����
			rtv = imageTable[1];
			break;

		case State::PNoticed:	//�T�{��΂���
			rtv = imageTable[2];
			break;
		}

		return rtv;
	}
	//-------------------------------------------------------------------
	//�{�^���̕`�揈��
	void Object::DrawButton()
	{
		if (this->isStartButtonDraw == true)
		{
			DrawInformation drawButtonImage = { ML::Box2D(-64, -64, 128, 128), ML::Box2D(0, 0, 128, 128) };
			drawButtonImage.draw.Offset(this->buttonDrawPos);

			switch (this->state)
			{
			case State::PWork:		//�d�������
				this->res->buttonImage_A_Outline->Draw(drawButtonImage.draw, drawButtonImage.src);
				break;

			case State::PSabori:	//�T�{����
				this->res->buttonImage_A->Draw(drawButtonImage.draw, drawButtonImage.src);
				break;

			case State::PNoticed:
				this->res->buttonImage_A_Outline->Draw(drawButtonImage.draw, drawButtonImage.src);
				break;
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
		totalSaboriTime(0.f),
		noticedToSabori(false),
		isPlayStartSE(false),
		saboriSEName(""),
		workSEName(""),
		isStartButtonDraw(false),
		buttonDrawPos(0, 0)
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