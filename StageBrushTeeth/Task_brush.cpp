//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_brush.h"
#include  "Task_stain.h"
#include "../sound.h"

namespace  brush
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/game_brush.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//SE
		se::LoadFile("Kirakira", "./data/sound/se/BrushTeethGame/きらきら輝く1.wav");
		se::LoadFile("brushing", "./data/sound/se/BrushTeethGame/歯磨き.wav");
		se::SetVolume("Kirakira", 20);
		se::SetVolume("brushing", 100);

		//★データ初期化
		this->render2D_Priority[1] = -0.6f;
		this->hitBase = ML::Box2D(70, -25, 80, 50);
		this->drawBase = ML::Box2D(-158, -35, 316, 70);
		this->pos.x = 0;
		this->pos.y = 0;
		this->speed = 6.0f;
		/*this->controller = ge->in1;*/
		

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			se::Stop("brushing");
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
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

		if (inp.LStick.volume != 0)
		{
			se::PlayLoop("brushing");
		}
		else
		{
			se::Stop("brushing");
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
		//ge->debugRect(me, 5);
		auto stains = ge->GetTasks<stain::Object>("よごれ");
		for (auto s = stains->begin(); s != stains->end(); s++)
		{
			auto you = (*s)->hitBase.OffsetCopy((*s)->pos);
			if (you.Hit(me))
			{
				se::Play("Kirakira");
				(*s)->Kill();
			}
		}

		//3人以下の場合に次に行けるように(応急措置)
		if (inp.SE.down) {
			ge->KillAll_G("ステージ歯磨き");
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src(0, 0, 945, 210);
		this->res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	void Object::Positionalise(int PlayerNum)
	{
		ML::Box2D PlayerArea(PlayerNum % 2 * (1980 / 2), PlayerNum / 2 * (1080 / 2), (1980 / 2), (1080 / 2));
		pos.x = PlayerArea.x + (drawBase.w/2);
		pos.y = PlayerArea.y + (drawBase.h/2);
		minPosX = PlayerArea.x + (drawBase.w / 2) - 100;
		minPosY = PlayerArea.y + (drawBase.h / 2);
		maxPosX = PlayerArea.x + PlayerArea.w - (drawBase.w / 2) - 25;
		maxPosY = PlayerArea.y + PlayerArea.h - (drawBase.h / 2);
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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