//-------------------------------------------------------------------
//��H���~�j�Q�[���̃v���C���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_OguiPlayer.h"
#include  "Task_OguiFood.h"
#include  "Task_OguiGame.h"

#include  "../sound.h"

namespace  OguiPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->image = DG::Image::Create("./data/image/game_otsan_eat_new.png");
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
		this->state = State::PWait;
		
		//���^�X�N�̐���

		//��SE
		se::LoadFile("eatSE", "./data/sound/se/OguiGame/Motion-Eating01-2.wav");
		se::SetVolume("eatSE", 50);

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
		auto game = ge->GetTask<OguiGame::Object>(OguiGame::defGroupName, OguiGame::defName);

		//��H���~�j�Q�[���^�X�N���擾�ł��Ă��邩�m�F
		if (game == nullptr)
		{
			return;
		}

		//�~�j�Q�[�����̏���
		if (game->isInGame == true)
		{
			this->moveCnt++;
			this->animationCount++;

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
		State nowState = this->state; //�Ƃ肠�������݂̏�Ԃ���

		//���[�V�����̐؂�ւ�
		switch (nowState)
		{
		case State::PWait:	//�ҋ@���
			if (input.B1.down && this->existFood == true) { nowState = State::PEat; }
			break;

		case State::PEat:	//�H�������
			if (this->moveCnt == 30 || this->existFood == false) { nowState = State::PWait; }
			break;
		}

		//��ԍX�V
		this->UpdateState(nowState);
	}
	//-------------------------------------------------------------------
	//��Ԗ��̍s������
	void Object::Move()
	{
		auto input = this->controller->GetState();

		switch (this->state)
		{
		case State::PWait:
			//���Q�[���{�ҊJ�n���Ɉ�x�����s������
			if (this->isPlayStart == false)
			{
				//�{�^���̕`����J�n����
				this->buttonDrawPos = ML::Vec2(this->pos.x, this->pos.y - 400);
				this->isStartButtonDraw = true;

				this->isPlayStart = true;
			}
			break;

		case State::PEat:	//�H�������
			if (input.B1.down)
			{
				//�������̎c�ʂ����炷
				//�S�Ă̗������擾
				auto foods = ge->GetTasks<OguiFood::Object>("�M�~�b�N");
				for (auto f = foods->begin(); f != foods->end(); ++f)
				{
					if (this->playerNum == (*f)->playerNum)
					{
						//������H�ׂ�SE��炷
						se::Play("eatSE");
						//�����̗ʂ����炷
						(*f)->ReduceHP(this->attack);
						//�����̗ʂ����炵����H������Ԃ��p������悤�ɂ���
						this->moveCnt = 0;
						return;
					}
				}
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//�A�j���[�V��������
	Object::DrawInformation Object::GetDrawImage()
	{
		DrawInformation imageTable[] = {
			{ ML::Box2D(-374 / 2, -512 / 2, 748 / 2, 1024 / 2), ML::Box2D(0, 0, 748, 1024) },	//�ҋ@�A�H����2
			{ ML::Box2D(-374 / 2, -512 / 2, 748 / 2, 1024 / 2), ML::Box2D(748, 0, 748, 1024) },	//�H����1
		};

		DrawInformation rtv;
		int animationNum; //�A�j���[�V�����p
		switch (this->state)
		{
		case State::PWait:		//�ҋ@���
			rtv = imageTable[0];
			break;

		case State::PEat:		//�H�������
			animationNum = (8 + this->animationCount) / 8; //�H����1����`����n�߂�
			animationNum %= 2;
			rtv = imageTable[animationNum];
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
			case State::PWait:	//�ҋ@���
				this->res->buttonImage_A_Outline->Draw(drawButtonImage.draw, drawButtonImage.src);
				break;

			case State::PEat:	//�H�������
				int animationNum = this->animationCount / 5;
				animationNum %= 2;

				if (animationNum == 0)	//A�{�^���̉摜�`��
				{
					this->res->buttonImage_A->Draw(drawButtonImage.draw, drawButtonImage.src);
				}
				else					//A�{�^���O�g�݂̂̉摜�`��
				{
					this->res->buttonImage_A_Outline->Draw(drawButtonImage.draw, drawButtonImage.src);
				}
				break;
			}
		}
	}
	//-------------------------------------------------------------------
	//�����̑��݂��Ă��邩�̏����擾
	void Object::SetExistFood(bool foodExistenceInformation)
	{
		this->existFood = foodExistenceInformation;
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
		attack(1),
		eatFoodCount(0),
		existFood(false),
		isPlayStart(false),
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