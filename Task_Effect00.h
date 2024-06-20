#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�G�t�F�N�g00
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Effect00
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("effect");	//�O���[�v��
	const  string  defName("effect");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
		//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		//�ύX������������������������������������������������������
	public:
		DG::Image::SP  img;
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		ML::Vec2	pos;		//�L�����N�^�ʒu
		ML::Vec2	moveVec;	//�ړ������x�N�g��
		ML::Vec2	graVec;		//�d�͊e�����ɏ�����
		int			resSizeX = 0, resSizeY = 0;//�G�t�F�N�g�摜��R�}�T�C�Y
		int			drawSizeX = 0, drawSizeY = 0;//�G�t�F�N�g�`���R�}�T�C�Y

		float		scale = 0.f;		//�g�嗦
		float		addScale = 0.f;	//�g�嗦���Z

		float		angle = 0.f;		//����
		float		addAngle = 0.f;	//�p�x���Z��

		bool		AddRender = false;	//���Z�����t���O
		float		alpha = 0.f;		//������
		float		addAlpha = 0.f;	//���Z��

		int			count = 0;		//�t���[���J�E���g
		int			countMax = 0;	//���Ńt���[��

		float		animno = 0.f;		//�A�j���[�V�����R�}��
		float		addAnim = 0.f;	//�A�j���[�V�������Z
		int			animMax = 0;	//�I�����B�R�}
		int			animResNum = 0;	//���\�[�X�̐܂�Ԃ���

		bool fade = false;

		std::string filename;
	};
}