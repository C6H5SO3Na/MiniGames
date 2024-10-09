//-------------------------------------------------------------------
//�c�莞�Ԃ̃o�[
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_TimeLimitBar.h"
#include  "Task_UIManager.h"
#include  "../Task_Game.h"

namespace TimeLimitBar
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/bar.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		img.reset();
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
		render2D_Priority[1] = 0.01f;
		srcBase = ML::Box2D(0, 0, 96, 32);

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
		switch (ge->gameState) {
		case MyPG::MyGameEngine::GameState::Start:
			gaugeAmount = static_cast<float>(remainingCnt) / maxCnt;
			break;
		case MyPG::MyGameEngine::GameState::Game:
			--remainingCnt;
			if (remainingCnt <= 0) {
				ge->hasAllClearedGame = true;
			}
			gaugeAmount = static_cast<float>(remainingCnt) / maxCnt;
			break;

		case MyPG::MyGameEngine::GameState::Finish:
			break;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		DrawFlame();
		DrawGauge();
	}
	//-------------------------------------------------------------------
	//�o�[�̘g�`��
	void Object::DrawFlame() const
	{
		ML::Box2D src(0, 0, srcBase.w, srcBase.h);
		ML::Box2D draw(-srcBase.w * 6 / 2, -srcBase.h * 2 / 2, srcBase.w * 6, srcBase.h * 2);
		draw.Offset(pos);
		res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//�o�[�̃Q�[�W�`��
	void Object::DrawGauge() const
	{
		int gSize = static_cast<int>(srcBase.w * gaugeAmount);
		ML::Box2D src(0, srcBase.h, gSize, srcBase.h);
		ML::Box2D draw(-srcBase.w * 6 / 2, -src.h * 2 / 2, gSize * 6, srcBase.h * 2);
		draw.Offset(pos);
		res->img->Draw(draw, src);
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
	Object::Object() :gaugeAmount(0.f), maxCnt(0), minPower(0), remainingCnt(0) {}
	//-------------------------------------------------------------------
	Object::SP Object::Create(const ML::Vec2& pos, const int& time)
	{
		auto gauge = Create(true);
		gauge->pos = pos;
		gauge->remainingCnt = time;
		gauge->maxCnt = gauge->remainingCnt;
		return gauge;
	}
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