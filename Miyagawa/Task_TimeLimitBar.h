#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�c�莞�Ԃ̃o�[
//-------------------------------------------------------------------
#include "../BChara.h"
#include  "Task_EasingLogo.h"

namespace TimeLimitBar
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("UI");	//�O���[�v��
	const  string  defName("�c�莞�ԃo�[");//�^�X�N��
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

		DG::Image::SP img;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
		//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		static  Object::SP Create(const ML::Vec2& pos);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������

		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);

		ML::Box2D srcBase;
		ML::Box2D drawBase;
		int remainingCnt;
		float maxCnt;
		int minPower;
		float gaugeAmount;
		EasingLogo::Object::WP finish;//Finish���S

		ML::Point barSize = { 96,32 };

		void Received() override{}

		void DrawFlame() const;//�g�`��
		void DrawGauge() const;//�Q�[�W�`��
	};
}