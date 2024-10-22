//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_StainManager.h"
#include  "Task_stain.h"
#include  "../randomLib.h"
#include  "Task_CommonItemManager02.h"
#include  "Task_StageBrushTeeth.h"

namespace  StainManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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

		
		this->minPosX = 0;
		this->maxPosX = 0;
		this->minPosY = 0;
		this->maxPosY = 0;

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	void Object::Positionalise(int PlayerNum)
	{
		const int num = 170;
		ML::Vec2 pos[][2] = {
			{ ML::Vec2(0.f, 0.f), ML::Vec2(1980 / 2, 1080 / 2) },
			{ ML::Vec2(1980 / 2, 0.f), ML::Vec2(1980, 1080 / 2) },
			{ ML::Vec2(0.f, 1080 / 2), ML::Vec2(1980 / 2, 1080) },
			{ ML::Vec2(1980 / 2, 1080 / 2), ML::Vec2(1980, 1080) },
		};
		const int min = 0;
		const int max = 1;

		/*ML::Box2D StainArea(PlayerNum % 2 * (1980 / 2), PlayerNum / 2 * (1080 / 2), (1980 / 2), (1080 / 2));*/
		minPosX = pos[PlayerNum][min].x + num + 40;//StainArea.x;
		minPosY = pos[PlayerNum][min].y + num - 50;//StainArea.y;
		maxPosX = pos[PlayerNum][max].x - 16 - num - 40;//StainArea.x + StainArea.w - 32;
		maxPosY = pos[PlayerNum][max].y - 16 - num + 100;//StainArea.y + StainArea.h - 32;
	}
	//-------------------------------------------------------------------
	void Object::CreateStain()
	{
		for (int i = 0; i < 10; ++i)
		{
			auto position = GetStainPos(positions);
			positions.push_back(position);
		}

		//★データ初期化
		for (int i = 0; i < 10; ++i)
		{
			auto s = stain::Object::Create(true);
			s->pos = positions[i];
			s->minX = minPosX;
			s->minY = minPosY;
			s->maxX = maxPosX;
			s->maxY = maxPosY;
			s->id = this->id;//GIVE STAIN A ID
			stains.push_back(s);
		}
	}

	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto stains = ge->GetTasks<stain::Object>("よごれ");
		auto com = ge->GetTask<CommonItemManager02::Object>("共通アイテムマネージャー02");
		int KillNum = 0;
		for (auto s = stains->begin(); s != stains->end(); s++)
		{
			if ((*s)->id == this->id) {//if stain and manager have same id
				KillNum++;
			}
		}
		if (KillNum == 0 && !isClear) //チェッククリア //if stain that have same id is null make isClear to true 
		{
			isClear = true;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	ML::Vec2 Object::GetStainPos(vector<ML::Vec2>& positions)
	{
		float x = GetRandom(this->minPosX, this->maxPosX);
		float y = GetRandom(this->minPosY, this->maxPosY);

		int w = 32;
		int h = 32;
		auto hit = ML::Box2D(-w / 2, -h / 2, w, h);

		// 重なっていないかチェック
		auto me = hit.OffsetCopy(x, y);
		for (auto p = positions.begin(); p != positions.end(); p++)
		{
			auto you = hit.OffsetCopy((*p));
			if (you.Hit(me)) {
				return GetStainPos(positions);
			}
		}

		return ML::Vec2(x, y);
	}
	//-------------------------------------------------------------------
	
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