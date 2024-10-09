//-------------------------------------------------------------------
//UI�}�l�[�W���[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "../Task_Game.h"
#include  "Task_UIManager.h"
#include  "Task_EasingLogo.h"
#include  "Task_FightLogo.h"
#include  "Task_TimeLimitBar.h"

namespace UIManager
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
		res = Resource::Create();

		//���f�[�^������
		phase = Phase::RuleShow;

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && nextTaskCreate) {
			//�������p���^�X�N�̐���
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto g = ge->GetTask<Game::Object>("�{��");
		switch (ge->gameState) {
		case MyPG::MyGameEngine::GameState::Start:

			switch (phase) {
			case Phase::RuleShow:
				ShowRule();
				phase = Phase::RuleEasing;
				break;

			case Phase::RuleEasing:
				if (ge->GetTask<EasingLogo::Object>(EasingLogo::defGroupName, EasingLogo::defName) == nullptr) {
					phase = Phase::StartShow;
				}
				break;

			case Phase::StartShow:
				CreateFightLogo();
				phase = Phase::StartEasing;
				break;

			case Phase::StartEasing:
				if (ge->GetTask<FightLogo::Object>(FightLogo::defGroupName, FightLogo::defName) == nullptr) {
					phase = Phase::FinishShow;
					ge->gameState = MyPG::MyGameEngine::GameState::Game;
				}
				break;
			}
			break;

		case MyPG::MyGameEngine::GameState::Game:

			break;

		case MyPG::MyGameEngine::GameState::Finish:
			switch (phase) {
			case Phase::FinishShow:
				ShowFinish();
				phase = Phase::FinishEasing;
				break;

			case Phase::FinishEasing:
				if (ge->GetTask<EasingLogo::Object>(EasingLogo::defGroupName, EasingLogo::defName) == nullptr) {
					phase = Phase::RuleShow;//�g���܂킵
					auto bar = ge->GetTask<TimeLimitBar::Object>(TimeLimitBar::defGroupName, TimeLimitBar::defName);
					bar->Kill();
				}
				break;
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//�X�e�[�W������`��
	EasingLogo::Object::SP Object::ShowRule()
	{
		//�V���O���g��
		if (ge->GetTask<EasingLogo::Object>(EasingLogo::defGroupName, EasingLogo::defName) == nullptr) {
			return EasingLogo::Object::Spawn(false);
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//Finish��`��
	EasingLogo::Object::SP  Object::ShowFinish()
	{
		//�V���O���g��
		if (ge->GetTask<EasingLogo::Object>(EasingLogo::defGroupName, EasingLogo::defName) == nullptr) {
			return EasingLogo::Object::Spawn(true);
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//�^�C�����~�b�g�̃Q�[�W��`��
	TimeLimitBar::Object::SP  Object::CreateTimeLimitBar(const ML::Vec2& pos, const int& time)
	{
		//�V���O���g��
		if (ge->GetTask<TimeLimitBar::Object>(TimeLimitBar::defGroupName, TimeLimitBar::defName) == nullptr) {
			return TimeLimitBar::Object::Create(pos, time);
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//�uFight�v�`��
	FightLogo::Object::SP  Object::CreateFightLogo()
	{
		//�V���O���g��
		if (ge->GetTask<FightLogo::Object>(FightLogo::defGroupName, FightLogo::defName) == nullptr) {
			return FightLogo::Object::Create(true);
		}
		return nullptr;
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
		return  Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {}
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
	Resource::~Resource() { Finalize(); }
}