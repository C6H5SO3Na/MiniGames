//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_StageBrushTeeth.h"
#include  "Task_brush.h"
#include  "Task_StainManager.h"
#include  "Task_CommonItemManager02.h"
#include  "../Task_Game.h"
#include  "../sound.h"

namespace  StageBrushTeeth
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->bgImg = DG::Image::Create("./data/image/mirror.png");
		this->teethImg = DG::Image::Create("./data/image/mouth.png");
		this->controllerMark = DG::Image::Create("./data/image/LeftStickAllDirection.png");
		this->PlayerNum = DG::Image::Create("./data/image/PlayerNumber.png");
		ge->debugRectLoad();
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->bgImg.reset();
		this->teethImg.reset();
		this->controllerMark.reset();
		this->PlayerNum.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//BGM
		bgm::LoadFile("stage2_bgm", "./data/sound/bgm/Stage2_Short60_yuttariDIY_02.mp3");
		bgm::Play("stage2_bgm");


		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 0.9f;
		this->state = Phase::Game;
		this->timeCnt = 0;
		this->clearCount = 0;
		this->animCnt = 0;

		//���^�X�N�̐���
		/*auto brush = brush::Object::Create(true);*/
		/*auto stainmanager = StainManager::Object::Create(true);*/
		auto commonmanager = CommonItemManager02::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�u���V��");
		ge->KillAll_G("�悲��}�l�[�W���[");
		ge->KillAll_G("�悲��");
		ge->KillAll_G("���ʃA�C�e���}�l�[�W���[02");
		ge->KillAll_G("�X�e�[�W������");

		bgm::Stop("stage2_bgm");
		se::Stop("Kirakira");
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			Game::Object::CreateTask(2);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		timeCnt++;
		animCnt++;
		//�A�j���X�V
		if (this->animCnt >= 15)
		{
			this->animCnt = 0;
			this->animIndex++;
			if (this->animIndex >= 2)
			{
				this->animIndex = 0;
			}
		}
		switch (this->state) {
		case Phase::Game:
			CheckClear();
			if (timeCnt >= 20 * 60)
			{
				MarkCount();
				this->state = Phase::Clear;
			}

			break;

		case Phase::Clear:
			Kill();
			break;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D src(0, 0, 3300, 2550);
		this->res->bgImg->Draw(draw, src);

		ML::Box2D draw_2(1920 / 2, 0, 1920 / 2, 1080 / 2);
		this->res->bgImg->Draw(draw_2, src);

		ML::Box2D draw_3(0, 1080 / 2, 1920 / 2, 1080 / 2);
		this->res->bgImg->Draw(draw_3, src);

		ML::Box2D draw_4(1920 / 2, 1080 / 2, 1920 / 2, 1080 / 2);
		this->res->bgImg->Draw(draw_4, src);

		ML::Box2D draw2(1920 / 10, 10, 1280/2, 1080/2 - 10 * 2);
		ML::Box2D src2(0, 0, 3300, 2550);
		this->res->teethImg->Draw(draw2, src2);

		ML::Box2D draw3(1920 / 2 + 1920 / 10, 10, 1280/2, 1080/2 - 10 * 2);
		this->res->teethImg->Draw(draw3, src2);

		ML::Box2D draw4(1920 / 10, 1080/2 + 10, 1280/2, 1080/2 - 10 * 2);
		this->res->teethImg->Draw(draw4, src2);

		ML::Box2D draw5(1920 / 2 + 1920 / 10, 1080/2 + 10, 1280/2, 1080/2 - 10 * 2);
		this->res->teethImg->Draw(draw5, src2);
		ge->debugRectDraw();

		//�R���g���[���[�}�[�N
		ML::Box2D Draw(1920 / 2 - 150 / 2, 1080 / 2 - 150 / 2, 150, 150);
		int srcX = animIndex % 2 * 128;
		int srcY = animIndex / 2 * 128;
		ML::Box2D Src(srcX, srcY, 128, 128);
		this->res->controllerMark->Draw(Draw, Src);

		//�v���C���[�i���o�[
		ML::Box2D draw01(0, 1080 / 2 - 105, 715 / 4, 105);
		ML::Box2D src01(0, 0, 715 / 4 - 20, 105);
		this->res->PlayerNum->Draw(draw01, src01);

		ML::Box2D draw02(1920 - 715 / 4 - 10, 1080 / 2 - 105, 715 / 4, 105);
		ML::Box2D src02(715 / 4 - 20, 0, 715 / 4, 105);
		this->res->PlayerNum->Draw(draw02, src02);

		ML::Box2D draw03(0, 1080 - 105 - 10, 715 / 4, 105);
		ML::Box2D src03(715 / 2 - 10, 0, 715 / 4, 105);
		this->res->PlayerNum->Draw(draw03, src03);

		ML::Box2D draw04(1920 - 715 / 4 - 10, 1080 - 105 - 10, 715 / 4, 105);
		ML::Box2D src04(715 / 4 * 3 - 10, 0, 715 / 4, 105);
		this->res->PlayerNum->Draw(draw04, src04);

	}
	//-------------------------------------------------------------------
	//�Q�[���{�҂̏���
	void  Object::CheckClear()
	{
		auto stainManager = ge->GetTasks<StainManager::Object>("�悲��}�l�[�W���[");
		auto com = ge->GetTask<CommonItemManager02::Object>("���ʃA�C�e���}�l�[�W���[02");
		for (auto it = stainManager->begin(); it != stainManager->end(); it++)
		{
			if ((*it)->IsClear()) {
				clearCount++;
				if (!(*it)->isScoreAdd) {
					ge->AddScore((*it)->id, com->addscore[com->rank]);
					com->rank++;
					(*it)->isScoreAdd = true;
				}		
			}
		}
		if (clearCount == 4) {// if all players were clear
			state = Phase::Clear;
		}
		else
		{
			clearCount = 0;// if not all players were clear this frame, reset clearcount
		}
	}
	//-------------------------------------------------------------------
	//�S���N���A��̏���
	void  Object::MarkCount()
	{
		auto stainManager = ge->GetTasks<StainManager::Object>("�悲��}�l�[�W���[");
		auto com = ge->GetTask<CommonItemManager02::Object>("���ʃA�C�e���}�l�[�W���[02");
		for (auto it = stainManager->begin(); it != stainManager->end(); it++)
		{
			if (!(*it)->isScoreAdd) {
				ge->AddScore((*it)->id, com->addscore[com->rank]);
				(*it)->isScoreAdd = true;
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
	Object::Object() {	}
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