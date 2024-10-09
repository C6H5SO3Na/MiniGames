//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect00.h"

namespace  Effect00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�f�t�H���g
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
		render2D_Priority[1] = -1.f;

		//�ړ���
		pos = ML::Vec2(0, 0);
		moveVec = ML::Vec2(0,0);
		graVec = ML::Vec2(0, 0);

		//�p�x
		angle = 0.f;//ML::ToRadian((float)(rand() % 360));
		addAngle = 0.f;

		//��
		alpha = 1.f;
		addAlpha = 0.f;

		//����������@
		scale = 1.f;
		addScale = 0.f;

		//�J�E���g
		count = 0;
		countMax = 30;

		//���\�[�X�T�C�Y
		resSizeX = 256;
		resSizeY = 256;
		drawSizeX = 64;
		drawSizeY = 64;

		//�A�j��
		animno = 0;
		addAnim = 0.f;
		animMax = 0;
		animResNum = 1;

		filename = "";

		AddRender = false;

		fade = false;
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
//		img.reset();

		if (!ge->QuitFlag() && nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (fade == true) {
			alpha = sin(ML::ToRadian(angle));
			angle += addAngle;
			if (angle >= 180.f) {
				Kill();
			}
		}
		else {

			//�ړ�
			pos += moveVec;		//�ړ�
			moveVec += graVec;	//�d�͉��Z

			//�p�x
			angle += addAngle;
			//�����x
			alpha += addAlpha;
			//�g�嗦
			scale += addScale;

			//���Ńg���K�[�̓J�E���g�A�A�j���̂�
			//�J�E���g
			//�J�E���g�����ŏ���
			count++;
			//�J�E���g�̓A�j��������ꍇ�A���ŏ����ɂȂ�Ȃ�
			if (animMax == 0) {
				if (count == countMax) {
					Kill();
				}
			}
			//�A�j���[�V����
			//�A�j���I���ŏ���
			animno += addAnim;

			//�A�j����0�R�}�̏ꍇ�͏��ŏ����ɂȂ�Ȃ�
			if (animMax != 0) {
				if ((int)animno >= animMax) {
					Kill();
				}
			}
			//----------
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�肳�ꂽ�t�@�C���̃��[�h
//
		if (count == 0) {
			if (!filename.empty()) {
				img = DG::Image::Create(filename);
			}
			else {
				img = DG::Image::Create("./data/image/��-01.png");
			}
		}

		//Box2D��`��int x,y,w,h��float�ɂ��邱��
		ML::Box2D  draw((int)(-drawSizeX / 2 * scale) ,(int)( -drawSizeY / 2 * scale),
			(int)(drawSizeX * scale),(int)( drawSizeY * scale));
		draw.Offset(pos);
		//
		if (animResNum <= 0) animResNum = 1;
		ML::Box2D  src(((int)animno % animResNum) * resSizeX,
			((int)animno / animResNum) * resSizeY,
			resSizeX, resSizeY);

		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

		//�t�F�[�h�̍ۂɂ͗��p��
		if (fade != true) {
			img->Rotation(angle, ML::Vec2(draw.w / 2.0f, draw.h / 2.0f));
		}
		if (AddRender) {
			//���ݓo�^����Ă���o�b�t�@���������_�����O����B
			ge->dgi->Flush2D();
			ge->dgi->EffectState().BS_AlphaAdd();
		}
		img->Draw(draw, src, ML::Color(alpha, 1, 1, 1));

		img->Rotation(0, ML::Vec2(draw.w / 2.0f, draw.h / 2.0f));

		if (AddRender) {
			//������x�����_�����O
			ge->dgi->Flush2D();
			ge->dgi->EffectState().BS_Alpha();
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
				//�i���\�b�h�����ςȂ̂͋��o�[�W�����̃R�s�[�ɂ��o�O��������邽��
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
	Resource::~Resource() { Finalize(); }
}