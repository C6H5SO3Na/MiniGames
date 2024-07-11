//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_brush.h"
#include  "Task_stain.h"

namespace  brush
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/brush.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		this->render2D_Priority[1] = -0.6f;
		this->hitBase = ML::Box2D(-125, -25, 250, 50);
		this->pos.x = 0;
		this->pos.y = 0;
		this->speed = 6.0f;
		/*this->controller = ge->in1;*/
		

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
		auto inp = this->controller->GetState();

		if (inp.LStick.volume > 0)
		{

			float x = cos(inp.LStick.angleDYP) * this->speed;
			float y = sin(inp.LStick.angleDYP) * this->speed;
			this->moveVec = ML::Vec2(x, y);
		}
		else
		{
			this->moveVec = ML::Vec2(0, 0);
		}


		this->pos += this->moveVec;
		if (this->pos.x + this->moveVec.x <= this->minPosX) {
			this->pos.x = this->minPosX;
		}
		if (this->pos.y + this->moveVec.y <= this->minPosY) {
			this->pos.y = this->minPosY;
		}
		if (this->pos.x + this->moveVec.x >= this->maxPosX) {
			this->pos.x = this->maxPosX;
		}
		if (this->pos.y + this->moveVec.y >= this->maxPosY){
			this->pos.y = this->maxPosY;
		}


		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
		auto stains = ge->GetTasks<stain::Object>("�悲��");
		for (auto s = stains->begin(); s != stains->end(); s++)
		{
			auto you = (*s)->hitBase.OffsetCopy((*s)->pos);
			if (you.Hit(me))
			{
				(*s)->Kill();
			}
		}
		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src(0, 0, 1000, 200);
		this->res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	void Object::Positionalise(int PlayerNum)
	{
		ML::Box2D PlayerArea(PlayerNum % 2 * (1980 / 2), PlayerNum / 2 * (1080 / 2), (1980 / 2), (1080 / 2));
		pos.x = PlayerArea.x + (hitBase.w/2);
		pos.y = PlayerArea.y + (hitBase.h/2);
		minPosX = PlayerArea.x + (hitBase.w / 2);
		minPosY = PlayerArea.y + (hitBase.h / 2);
		maxPosX = PlayerArea.x + PlayerArea.w - (hitBase.w / 2);
		maxPosY = PlayerArea.y + PlayerArea.h - (hitBase.h / 2);
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