//-------------------------------------------------------------------
//�R���g���[���[�o�^���
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_SubscribeController.h"
#include  "Task_SubscribeControllerBG.h"
#include  "../Task_Game.h"
#include  "../sound.h"
#include  "../easing.h"

namespace SubscribeController
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
		render2D_Priority[1] = 0.f;
		TestFont = DG::Font::Create("�l�r �S�V�b�N", 30, 30);
		ge->players.clear();

		//�R���e�i�ɒǉ�
		inputs.push_back(ge->in1);
		inputs.push_back(ge->in2);
		inputs.push_back(ge->in3);
		inputs.push_back(ge->in4);

		//���^�X�N�̐���
		//�w�i
		//���X�N���[�������
		for (int i = 0; i < 3; ++i)
		{
			auto BG = SubscribeControllerBG::Object::Create(true);
			if (BG)
			{
				BG->pos.x = static_cast<float>(i * 1600);
			}
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{

		ge->KillAll_G("�R���g���[���[�o�^���");
		//���f�[�^���^�X�N���
		if (!ge->QuitFlag() && nextTaskCreate) {
			//�������p���^�X�N�̐���
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		easing::UpDate();//�K�{
		for (int i = 0; i < inputs.size(); ++i) {
			//���̃^�X�N��
			if (inputs[i]->GetState().ST.down && subscribeCnt > 0) {
				Kill();
			}

			//�o�^ 
			if (inputs[i]->GetState().B1.down) {
				easing::Set("Reaction" + to_string(controllerIndex[i]), easing::QUADOUT, 0, 50, 5, "ReactionEnd" + to_string(controllerIndex[i]));
				easing::Set("ReactionEnd" + to_string(controllerIndex[i]), easing::QUADOUT, 50, 0, 5);
				easing::Start("Reaction" + to_string(controllerIndex[i]));
				if (!isPushButton[i]) {
					Subscribe(inputs[i], isPushButton[i], i);
				}
			}
		}

		for (int i = 0; i < ge->players.size(); ++i) {
			if (easing::GetState("ReactionEnd" + to_string(controllerIndex[i])) == easing::EQ_STATE::EQ_END) {
				easingPos[controllerIndex[i]] = 0.f;
			}
			else if (easing::GetState("Reaction" + to_string(controllerIndex[i])) == easing::EQ_STATE::EQ_END) {
				easingPos[controllerIndex[i]] = easing::GetPos("ReactionEnd" + to_string(controllerIndex[i]));
			}
			else
			{
				easingPos[controllerIndex[i]] = easing::GetPos("Reaction" + to_string(controllerIndex[i]));
			}
		}

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		for (int i = 0; i < ge->players.size(); ++i) {
			if (!isPushButton[controllerIndex[i]]) { continue; }
			ML::Box2D textBox(100 * i, 100 + easingPos[i], 100, 100);
			string text = to_string(i + 1) + "P";
			TestFont->Draw(textBox, text);
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Subscribe(XI::GamePad::SP controller, bool& isPush, const int& i)
	{
		isPush = true;
		ge->players.push_back(controller);
		controllerIndex[subscribeCnt] = i;
		++subscribeCnt;
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