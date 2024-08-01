//-------------------------------------------------------------------
//リザルト画面のUI管理
//-------------------------------------------------------------------
#include  "../MyPG.h"
#include  "Task_ResultUIManager.h"

#include  "../easing.h"
#include  "../sound.h"

namespace  ResultUIManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->prefaceImage = DG::Image::Create("./data/image/ResultAnnouncement.png");
		this->buttonImage_A = DG::Image::Create("./data/image/button/double/xbox_button_color_b.png");
		this->buttonImage_A_Outline = DG::Image::Create("./data/image/button/double/xbox_button_color_b_outline.png");
		this->thankyouImage = DG::Image::Create("./data/image/ThankyouForPlaying.png");
		this->directToTitleImage = DG::Image::Create("./data/image/DirectToTitle.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->prefaceImage.reset();
		this->buttonImage_A.reset();
		this->buttonImage_A_Outline.reset();
		this->thankyouImage.reset();
		this->directToTitleImage.reset();
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

		//★データ初期化
		this->render2D_Priority[1] = 0.6f;
		this->displayPattern = DecideDisplayPattern(); //表示パターン決定
		
		//★タスクの生成

		//☆イージング
		//文字画像移動用
		//「結果発表」移動用
		easing::Set("StartMovePrefaceImage", easing::CIRCOUT, static_cast<float>(ge->screen2DWidth + srcValues[0][2] * 2), ge->screen2DWidth / 2.f, this->gameFps, "EndMovePrefaceImage");
		easing::Set("EndMovePrefaceImage", easing::CIRCIN, ge->screen2DWidth / 2.f, static_cast<float>(-srcValues[0][2] * 2), this->gameFps);

		//☆SE
		//キャラクター表示時のSE
		se::LoadFile("CharacterAppearanceSE", "./data/sound/se/Ending/maou_se_onepoint03.wav");
		se::SetVolume("CharacterAppearanceSE", 10);

		//1位表示時のSE
		se::LoadFile("FirstPlayerDisplaySE", "./data/sound/se/Ending/歓声と拍手.wav");
		se::SetVolume("FirstPlayerDisplaySE", 100);

		return  true;
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
		//☆Task_Resultの状態毎の処理を行う
		ProcessEachResultState();
		//イージングを動かす
		easing::UpDate();

		//☆描画が終わったことをTask_Resultに伝える
		if (hasEndedDrawing == true)
		{
			//Task_Resultの情報を取得する
			auto result = ge->GetTask<Result::Object>("リザルト画面");
			if (result == nullptr)
			{
				return;
			}

			//Task_Resultの関数を使用し、次の状態に変更できるようにする
			result->ChangeableToNextState();

			//描画終了判定用の変数をfalseに戻す
			this->hasEndedDrawing = false;
			this->isChangedFalse_hasEndedDrawing = true; //hasEndedDrawingをfalseにしたのでtrueに変更
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//☆Task_Resultの状態毎の描画
		DrawingProcessingEachResultState();
	}
	//-------------------------------------------------------------------
	//Task_Resultの状態毎の処理
	void Object::ProcessEachResultState()
	{
		//Task_Resultの情報を取得
		auto result = ge->GetTask<Result::Object>("リザルト画面");
		if (result == nullptr)
		{
			return;
		}

		//状態毎の処理
		switch (result->resultState)
		{
		case Result::Object::ResultState::Preface:
			//☆リザルトが始まった瞬間に行う処理
			if (this->resultStart == true)
			{
				//☆イージング開始
				easing::Start("StartMovePrefaceImage");

				this->resultStart = false;
			}

			//☆イージングで座標移動
			//Readyを動かす
			this->prefaceImagePos.x = easing::GetPos("StartMovePrefaceImage");
			if (easing::GetState("StartMovePrefaceImage") == easing::EQ_STATE::EQ_END) //イージング「ReadyStart」が終わったら
			{
				this->prefaceImagePos.x = easing::GetPos("EndMovePrefaceImage");
			}
			break;

		case Result::Object::ResultState::ResultAnnouncement:	//結果発表
			this->drawUpToCount++;
			this->animationCount++;

			//表示用プレイヤーーの生成
			this->GenerateDisplayPlayer();
			break;

		case Result::Object::ResultState::End:
			this->animationCount++;
			break;
		}

	}
	//-------------------------------------------------------------------
	//表示用プレイヤーの生成
	void Object::GenerateDisplayPlayer()
	{
		//Task_Resultの情報を取得
		auto result = ge->GetTask<Result::Object>("リザルト画面");
		if (result == nullptr)
		{
			return;
		}

		//表示用プレイヤーの生成タイミングをパターン化し、displayPatternの値で決定する
		switch (this->displayPattern)
		{
		case 0: //1位四人
			//1位の画像のプレイヤーを四人生成する
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//1位の生成
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
				for (int i = 0; i < 4; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i], playerNumberPosition[i], rankSentencePosition[i], scoreDrawPosition[i]);
				}
			}
			break;

		case 1: //1位三人、4位一人
			//1位の画像のプレイヤーを三人、4位の画像のプレイヤーを一人生成する
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//4位の生成
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(3.f * gameFps))
			{
				//1位の生成
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
				for (int i = 0; i < 3; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i], playerNumberPosition[i], rankSentencePosition[i], scoreDrawPosition[i]);
				}
			}
			break;

		case 2: //1位二人、3位二人
			//1位の画像のプレイヤーを二人、3位の画像のプレイヤーを二人生成する
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//3位の生成
				se::Play("CharacterAppearanceSE");
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i + 2]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i + 2], playerNumberPosition[i + 2], rankSentencePosition[i + 2], scoreDrawPosition[i + 2]);
				}
			}
			if (drawUpToCount == static_cast<int>(3.f * gameFps))
			{
				//1位の生成
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i], playerNumberPosition[i], rankSentencePosition[i], scoreDrawPosition[i]);
				}
			}
			break;

		case 3: //1位二人、3位一人、4位一人
			//1位の画像のプレイヤーを二人、3位の画像のプレイヤーを一人、4位の画像のプレイヤーを一人生成する
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//4位の生成
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//3位の生成
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[2]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[2], playerNumberPosition[2], rankSentencePosition[2], scoreDrawPosition[2]);
			}
			if (drawUpToCount == static_cast<int>(4.f * gameFps))
			{
				//1位の生成
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i], playerNumberPosition[i], rankSentencePosition[i], scoreDrawPosition[i]);
				}
			}
			break;

		case 4: //1位一人、2位三人
			//1位の画像のプレイヤーを一人、2位の画像のプレイヤーを三人生成する
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//2位の生成
				se::Play("CharacterAppearanceSE");
				for (int i = 0; i < 3; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i + 1]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i + 1], playerNumberPosition[i + 1], rankSentencePosition[i + 1], scoreDrawPosition[i + 1]);
				}
			}
			if (drawUpToCount == static_cast<int>(3.f * gameFps))
			{
				//1位の生成
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[0]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[0], playerNumberPosition[0], rankSentencePosition[0], scoreDrawPosition[0]);
			}
			break;

		case 5: //1位一人、2位二人、4位一人
			//1位の画像のプレイヤーを一人、2位の画像のプレイヤーを二人、4位の画像のプレイヤーを一人生成する
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//4位の生成
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(3.f * gameFps))
			{
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
				//2位の生成
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i + 1]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i + 1], playerNumberPosition[i + 1], rankSentencePosition[i + 1], scoreDrawPosition[i + 1]);
				}

				//1位の生成
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[0]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[0], playerNumberPosition[0], rankSentencePosition[0], scoreDrawPosition[0]);
			}
			break;

		case 6: //1位一人、2位一人、3位二人
			//1位の画像のプレイヤーを一人、2位の画像のプレイヤーを一人、3位の画像のプレイヤーを二人生成する
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//3位の生成
				se::Play("CharacterAppearanceSE");
				for (int i = 0; i < 2; ++i)
				{
					auto displayPlayer = ResultUIPlayer::Object::Create(true);
					displayPlayer->setPlayerInformation(result->playersInfo[i + 2]);
					displayPlayer->setDrawPos(playerRankEachPlayerPosition[i + 2], playerNumberPosition[i + 2], rankSentencePosition[i + 2], scoreDrawPosition[i + 2]);
				}
			}
			if (drawUpToCount == static_cast<int>(3.f * gameFps))
			{
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
				//2位の生成
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[1]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[1], playerNumberPosition[1], rankSentencePosition[1], scoreDrawPosition[1]);

				//1位の生成
				displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[0]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[0], playerNumberPosition[0], rankSentencePosition[0], scoreDrawPosition[0]);
			}
			break;

		case 7: //同率なし
			//1位の画像のプレイヤーを一人、2位の画像のプレイヤーを一人、3位の画像のプレイヤーを一人、4位の画像のプレイヤーを一人生成する
			if (drawUpToCount == static_cast<int>(1.f * gameFps))
			{
				//4位の生成
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[3]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[3], playerNumberPosition[3], rankSentencePosition[3], scoreDrawPosition[3]);

			}
			if (drawUpToCount == static_cast<int>(2.f * gameFps))
			{
				//3位の生成
				se::Play("CharacterAppearanceSE");
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[2]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[2], playerNumberPosition[2], rankSentencePosition[2], scoreDrawPosition[2]);
			}
			if (drawUpToCount == static_cast<int>(4.f * gameFps))
			{
				se::Play("CharacterAppearanceSE");
				se::Play("FirstPlayerDisplaySE");
				//2位の生成
				auto displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[1]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[1], playerNumberPosition[1], rankSentencePosition[1], scoreDrawPosition[1]);

				//1位の生成
				displayPlayer = ResultUIPlayer::Object::Create(true);
				displayPlayer->setPlayerInformation(result->playersInfo[0]);
				displayPlayer->setDrawPos(playerRankEachPlayerPosition[0], playerNumberPosition[0], rankSentencePosition[0], scoreDrawPosition[0]);
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//Task_Resultの状態毎の描画処理
	void Object::DrawingProcessingEachResultState()
	{
		//Task_Resultの情報を取得
		auto result = ge->GetTask<Result::Object>("リザルト画面");
		if (result == nullptr)
		{
			return;
		}

		//描画情報準備
		ML::Box2D draw = {};
		ML::Box2D src = {};

		//状態毎の描画処理
		switch (result->resultState)
		{
		case Result::Object::ResultState::Preface:				//前置き
			//☆描画
			//描画情報設定
			draw = ML::Box2D(this->drawValues[0][0], this->drawValues[0][1], this->drawValues[0][2], this->drawValues[0][3]);
			src = ML::Box2D(this->srcValues[0][0], this->srcValues[0][1], this->srcValues[0][2], this->srcValues[0][3]);
			draw.Offset(this->prefaceImagePos);

			this->res->prefaceImage->Draw(draw, src);

			if (easing::GetState("EndMovePrefaceImage") == easing::EQ_STATE::EQ_END) //「結果発表」の画像の移動が終わったら
			{
				if (this->isChangedFalse_hasEndedDrawing == false) //状態がPrefaceの時、1回だけhasEndedDrawingがtrueになるようにする
				{
					this->hasEndedDrawing = true;
				}
			}
			break;

		case Result::Object::ResultState::ResultAnnouncement:	//結果発表
			//☆描画
			if (this->displayPattern == 0)
			{
				if (drawUpToCount >= static_cast<int>(3.f * gameFps))
				{
					//☆Aボタンを描画
					//描画情報設定
					int animationNum = this->animationCount / 15;
					animationNum %= 2;
					draw = ML::Box2D(this->drawValues[1][0], this->drawValues[1][1], this->drawValues[1][2], this->drawValues[1][3]);
					src = ML::Box2D(this->srcValues[1][0], this->srcValues[1][1], this->srcValues[1][2], this->srcValues[1][3]);

					if (animationNum == 0)	//Aボタンの画像描画
					{
						this->res->buttonImage_A->Draw(draw, src);
					}
					else					//Aボタン外枠のみの画像描画
					{
						this->res->buttonImage_A_Outline->Draw(draw, src);
					}

					//描画が終わったことを伝える
					if (this->isChangedFalse_hasEndedDrawing == false) //状態がResultAnnouncementの時、1回だけhasEndedDrawingがtrueになるようにする
					{
						this->hasEndedDrawing = true;
					}
				}
			}
			else
			{
				if (drawUpToCount >= static_cast<int>(5.f * gameFps))
				{
					//☆Aボタン描画
					//描画情報設定
					int animationNum = this->animationCount / 15;
					animationNum %= 2;
					draw = ML::Box2D(this->drawValues[1][0], this->drawValues[1][1], this->drawValues[1][2], this->drawValues[1][3]);
					src = ML::Box2D(this->srcValues[1][0], this->srcValues[1][1], this->srcValues[1][2], this->srcValues[1][3]);

					if (animationNum == 0)	//Aボタンの画像描画
					{
						this->res->buttonImage_A->Draw(draw, src);
					}
					else					//Aボタン外枠のみの画像描画
					{
						this->res->buttonImage_A_Outline->Draw(draw, src);
					}

					//描画が終わったことを伝える
					if (this->isChangedFalse_hasEndedDrawing == false) //状態がResultAnnouncementの時、1回だけhasEndedDrawingがtrueになるようにする
					{
						this->hasEndedDrawing = true;
					}
				}
			}
			break;

		case Result::Object::ResultState::End:
			//☆「遊んでくれてありがとう」と描画
			//描画情報設定
			draw = ML::Box2D(this->drawValues[2][0], this->drawValues[2][1], this->drawValues[2][2], this->drawValues[2][3]);
			src = ML::Box2D(this->srcValues[2][0], this->srcValues[2][1], this->srcValues[2][2], this->srcValues[2][3]);

			this->res->thankyouImage->Draw(draw, src);
			
			//☆「Aボタンでタイトルへ」と描画
			//Aボタン描画
			//描画情報設定
			int animationNum = this->animationCount / 15;
			animationNum %= 2;
			draw = ML::Box2D(this->drawValues[3][0], this->drawValues[3][1], this->drawValues[3][2], this->drawValues[3][3]);
			src = ML::Box2D(this->srcValues[1][0], this->srcValues[1][1], this->srcValues[1][2], this->srcValues[1][3]);

			if (animationNum == 0)	//Aボタンの画像描画
			{
				this->res->buttonImage_A->Draw(draw, src);
			}
			else					//Aボタン外枠のみの画像描画
			{
				this->res->buttonImage_A_Outline->Draw(draw, src);
			}

			//「ボタンでタイトルへ」描画
			//描画情報設定
			draw = ML::Box2D(this->drawValues[4][0], this->drawValues[4][1], this->drawValues[4][2], this->drawValues[4][3]);
			src = ML::Box2D(this->srcValues[3][0], this->srcValues[3][1], this->srcValues[3][2], this->srcValues[3][3]);

			this->res->directToTitleImage->Draw(draw, src);

			//描画が終わったことを伝える
			if (this->isChangedFalse_hasEndedDrawing == false) //状態がResultAnnouncementの時、1回だけhasEndedDrawingがtrueになるようにする
			{
				this->hasEndedDrawing = true;
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//状態更新時にするデータ(変数)の変更
	void Object::DataChangesWhenUpdatingState()
	{
		this->isChangedFalse_hasEndedDrawing = false;
		this->drawUpToCount = 0;
		this->animationCount = 0;
	}
	//-------------------------------------------------------------------
	//結果発表時の表示パターンを決める
	int Object::DecideDisplayPattern()
	{
		//Task_Resultから情報を取得
		auto result = ge->GetTask<Result::Object>("リザルト画面");
		if (result == nullptr)
		{
			return 0;
		}

		int rankCount[4] = {}; //要素数0を1位、1を2位・・・として順位の数をカウントする

		//☆順位の数のカウント
		for (int i = 0; i < sizeof(result->playersInfo) / sizeof(result->playersInfo[0]); ++i)
		{
			switch (result->playersInfo[i].rank)
			{
			case 1:
				rankCount[0]++;
				break;

			case 2:
				rankCount[1]++;
				break;

			case 3:
				rankCount[2]++;
				break;

			case 4:
				rankCount[3]++;
				break;
			}
		}

		//☆表示パターン決定
		int rtv = 0;
		const int patternNumber = 8;			//パターンの数
		int countPattern[patternNumber][4] = {	//カウントのパターン
			{4, 0, 0, 0}, //パターン0　1位四人
			{3, 0, 0, 1}, //パターン1　1位三人、4位一人
			{2, 0, 2, 0}, //パターン2　1位二人、3位二人
			{2, 0, 1, 1}, //パターン3　1位二人、3位一人、4位一人
			{1, 3, 0, 0}, //パターン4　1位一人、2位三人
			{1, 2, 0, 1}, //パターン5　1位一人、2位二人、4位一人
			{1, 1, 2, 0}, //パターン6　1位一人、2位一人、3位二人
			{1, 1, 1, 1}  //パターン7　同率なし
		};
		int sameCount = 0;	//順位のカウントとパターンの値があっていたら+1する　sameCount == 4になったパターンの数を返り値とする
		
		for (int i = 0; i < patternNumber; ++i)
		{
			//カウントとパターンを照らし合わせる
			for (int j = 0; j < sizeof(rankCount) / sizeof(rankCount[0]); ++j)
			{
				if (rankCount[j] == countPattern[i][j])
				{
					sameCount++;
				}
			}

			//カウントとパターンの全ての値が同じだったら、そのパターンを返り値として設定し、for文を抜ける
			if (sameCount == 4) 
			{
				rtv = i;
				break;
			}
			else //値が一つでも違ったらカウントをリセットする
			{
				sameCount = 0;
			}
		}
		
		return rtv;
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
	Object::Object() 
		:
		drawUpToCount(0),
		animationCount(0),
		gameFps(60),
		hasEndedDrawing(false),
		isChangedFalse_hasEndedDrawing(false),
		displayPattern(0),
		resultStart(true)
	{	}
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