#include <DxLib.h>
#include "../Application.h"
#include"../Utility/Utility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/SceneManager.h"
#include "../Common/CommonData.h"
#include "SceneSelect.h"

SceneSelect::SceneSelect()
{
}

SceneSelect::~SceneSelect()
{
}

bool SceneSelect::Init(void)
{
	//サウンドのインスタンス
	sound_ = new SoundManager();
	sound_->SoundInit();

	//サウンドのロード
	sound_->BGMLoad(SoundManager::BGM_TYPE::SELECT);
	sound_->SELoad(SoundManager::SE_TYPE::CLICK);
	sound_->SELoad(SoundManager::SE_TYPE::CANCEL);

	//BGM再生
	sound_->PlayBGM(SoundManager::BGM_TYPE::SELECT, DX_PLAYTYPE_LOOP);

	//画像の読み込み
	bgImages_[BACKGROUND_TYPE::BACK] = LoadGraph((Application::PATH_IMAGE + "SelectBack.png").c_str());
	bgImages_[BACKGROUND_TYPE::SKY] = LoadGraph((Application::PATH_IMAGE + "SelectBackSky.png").c_str());
	bgImages_[BACKGROUND_TYPE::CLOUD] = LoadGraph((Application::PATH_IMAGE + "SelectBackCloud.png").c_str());
	cloudPos_ = { 0.0f,0.0f };

	bpImages_[CommonData::BATTLE_PATTERN::P1C1] = LoadGraph((Application::PATH_IMAGE + "BattleP1C1.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P1C2] = LoadGraph((Application::PATH_IMAGE + "BattleP1C2.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P1C3] = LoadGraph((Application::PATH_IMAGE + "BattleP1C3.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P2] = LoadGraph((Application::PATH_IMAGE + "BattleP2.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P2C1] = LoadGraph((Application::PATH_IMAGE + "BattleP2C1.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P2C2] = LoadGraph((Application::PATH_IMAGE + "BattleP2C2.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P3] = LoadGraph((Application::PATH_IMAGE + "BattleP3.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P3C1] = LoadGraph((Application::PATH_IMAGE + "BattleP3C1.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P4] = LoadGraph((Application::PATH_IMAGE + "BattleP4.png").c_str());

	arrowImage_ = LoadGraph((Application::PATH_IMAGE + "arrow.png").c_str());

	padNumImage_ = LoadGraph((Application::PATH_IMAGE + "PadNum.png").c_str());

	diffImages_[CommonData::DIFFICULTY::EASY] = LoadGraph((Application::PATH_IMAGE + "difficultyEasy.png").c_str());
	diffImages_[CommonData::DIFFICULTY::NORMAL] = LoadGraph((Application::PATH_IMAGE + "difficultyNormal.png").c_str());
	diffImages_[CommonData::DIFFICULTY::HARD] = LoadGraph((Application::PATH_IMAGE + "difficultyHard.png").c_str());


	ruleImages_[CommonData::RULE::TIME] = LoadGraph((Application::PATH_IMAGE + "RuleScore.png").c_str());
	ruleImages_[CommonData::RULE::LIFE] = LoadGraph((Application::PATH_IMAGE + "RuleLife.png").c_str());
	ruleImages_[CommonData::RULE::BREAK_TILE] = LoadGraph((Application::PATH_IMAGE + "RuleTileBreak.png").c_str());

	ruleExplainImages_[CommonData::RULE::TIME] = LoadGraph((Application::PATH_IMAGE + "RuleScoreExplain.png").c_str());
	ruleExplainImages_[CommonData::RULE::LIFE] = LoadGraph((Application::PATH_IMAGE + "RuleLifeExplain.png").c_str());
	ruleExplainImages_[CommonData::RULE::BREAK_TILE] = LoadGraph((Application::PATH_IMAGE + "RuleTileBreakExplain.png").c_str());

	tutorialImage_ = LoadGraph((Application::PATH_IMAGE + "Tutorial.png").c_str());

	//モデルの読み込み
	pModel_[static_cast<int>(CommonData::TYPE::P1) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenBlue.mv1").c_str());
	pModel_[static_cast<int>(CommonData::TYPE::P2) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenRed.mv1").c_str());
	pModel_[static_cast<int>(CommonData::TYPE::P3) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenGreen.mv1").c_str());
	pModel_[static_cast<int>(CommonData::TYPE::P4) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenYellow.mv1").c_str());

	//変数の初期化
	//-----------------------
	selectItem_ = SELECT_ITEM::BATTLE_PATTERN;
	playerNum_ = 1;
	cpuNum_ = CommonData::CPU_NUM::C1;
	compareCpuNum_ = 0;
	isVib_ = true;
	arrowSize_ = ARROW_DEFAULT_SIZE;
	step_ = 0.0f;

	pScl_ = VScale(Utility::VECTOR_ONE,2.0f);
	pRot_ = Utility::VECTOR_ZERO;
	pPos_ = Utility::VECTOR_ZERO;
	stepAnim_ = 0.0f;

	if (GetJoypadNum() >= 1)
	{
		selectPadNum_ = 1;
	}
	else
	{
		selectPadNum_ = 0;
	}

	selectPattern_ = CommonData::BATTLE_PATTERN::P1C1;
	selectRule_ = CommonData::RULE::NONE;
	
	imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };
	cursorPos_ = imgsPos_;
	
	distance_ = Application::SCREEN_SIZE_X / static_cast<int>(CommonData::TYPE::P4);

	int padFontSize = 30;
	padFont_ = CreateFontToHandle(NULL, padFontSize, 10);

	//フォントデータ用
	int fontSize = 300;
	fontHandle_ = CreateFontToHandle(NULL, fontSize, 3);

	for (int i = 0; i < static_cast<int>(CommonData::TYPE::P4); i++)
	{
		//モデルの位置、大きさ、角度
		MV1SetPosition(pModel_[i], pPos_);
		MV1SetScale(pModel_[i], pScl_);
		MV1SetRotationXYZ(pModel_[i], pRot_);

		animAttachNo_[i] = MV1AttachAnim(pModel_[i], static_cast<int>(ANIM_NUM::WALK));
		animTotalTime_[i] = MV1GetAttachAnimTotalTime(pModel_[i], animAttachNo_[i]);
	}

	//関数ポインタ初期化
	itemUpdate_.emplace(SELECT_ITEM::BATTLE_PATTERN, std::bind(&SceneSelect::BattlePatternUpdate, this));
	itemUpdate_.emplace(SELECT_ITEM::CHECK_PAD, std::bind(&SceneSelect::CheckPadUpdate, this));
	itemUpdate_.emplace(SELECT_ITEM::DIFFICULTY, std::bind(&SceneSelect::DifficultyUpdate, this));
	itemUpdate_.emplace(SELECT_ITEM::RULE, std::bind(&SceneSelect::RuleUpdate, this));

	itemDraw_.emplace(SELECT_ITEM::BATTLE_PATTERN, std::bind(&SceneSelect::BattlePatternDraw, this));
	itemDraw_.emplace(SELECT_ITEM::CHECK_PAD, std::bind(&SceneSelect::CheckPadDraw, this));
	itemDraw_.emplace(SELECT_ITEM::DIFFICULTY, std::bind(&SceneSelect::DifficultyDraw, this));
	itemDraw_.emplace(SELECT_ITEM::RULE, std::bind(&SceneSelect::RuleDraw, this));

	return true;
}

void SceneSelect::Update(void)
{
	//入力マネージャーのインスタンス
	auto ins = InputManager::GetInstance();

	//現在のパッド接続数
	int padNum = GetJoypadNum();

	//次に進めるか
	bool isNextSelect = true;

	//雲の動き
	cloudPos_.x--;
	if (cloudPos_.x < -Application::SCREEN_SIZE_X)
	{
		cloudPos_.x = 0;
	}

	//時間進行
	step_ += Utility::DELTA_TIME;

	//矢印の大きさ
	arrowSize_ = ARROW_DEFAULT_SIZE + Utility::Deg2RadF(sinf(step_ * ARROW_CHANGE_SIZE_SPEED) * ARROW_CHANGE_SIZE);

	//項目ごとの更新
	itemUpdate_[selectItem_]();
}

void SceneSelect::Draw(void)
{
	//現在のパッド接続数
	int padNum = GetJoypadNum();

	//フォントデータ用
	int fontSize = 0;

	//色
	unsigned int color;

	//背景画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImages_[BACKGROUND_TYPE::SKY], true, false);
	DrawRotaGraph(cloudPos_.x, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImages_[BACKGROUND_TYPE::CLOUD], true, false);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImages_[BACKGROUND_TYPE::CLOUD], true, false);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X * 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImages_[BACKGROUND_TYPE::CLOUD], true, false);
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImages_[BACKGROUND_TYPE::BACK], true, false);

	//選択項目ごとの描画
	itemDraw_[selectItem_]();

	//フォントの解放
	DeleteFontToHandle(fontHandle_);
}

bool SceneSelect::Release(void)
{
	//サウンドの解放
	sound_->SoundRelease();
	delete sound_;
	sound_ = nullptr;

	//画像の解放
	for (int bg = 0; bg < static_cast<int>(BACKGROUND_TYPE::MAX); bg++)
	{
		DeleteGraph(bgImages_[static_cast<BACKGROUND_TYPE>(bg)]);
	}

	for (int bp = 0; bp < static_cast<int>(CommonData::BATTLE_PATTERN::MAX); bp++)
	{
		DeleteGraph(bpImages_[static_cast<CommonData::BATTLE_PATTERN>(bp)]);
	}

	DeleteGraph(arrowImage_);
	
	DeleteGraph(padNumImage_);

	for(int rule = 0;rule<static_cast<int>(CommonData::RULE::BREAK_TILE);rule++)
	{
		DeleteGraph(ruleImages_[static_cast<CommonData::RULE>(rule)]);
		DeleteGraph(ruleExplainImages_[static_cast<CommonData::RULE>(rule)]);
	}

	DeleteGraph(tutorialImage_);

	//モデルの解放
	for (int pl = 0; pl < static_cast<int>(CommonData::TYPE::P4); pl++)
	{
		MV1DeleteModel(pModel_[pl]);
	}

	//フォントデータ解放
	DeleteFontToHandle(fontHandle_);
	DeleteFontToHandle(padFont_);

	return true;
}

void SceneSelect::CursorDraw(int _sizeX, int _sizeY)
{
	if (static_cast<int>(step_ * Utility::DEFAULT_FPS / 20) % 2 == 0)
	{
		DrawBox(cursorPos_.x - CURSOR_SIZE - _sizeX / 2
			, cursorPos_.y - CURSOR_SIZE - _sizeY / 2
			, cursorPos_.x + CURSOR_SIZE + _sizeX / 2
			, cursorPos_.y + CURSOR_SIZE + _sizeY / 2
			, 0x44ff44, true);
	}
}

void SceneSelect::BattlePatternUpdate(void)
{
	//入力マネージャーのインスタンス
	auto ins = InputManager::GetInstance();

	//現在のパッド接続数
	int padNum = GetJoypadNum();

	//次に進めるか
	bool isNextSelect = true;

	//画像の位置
	imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };

	//カーソル位置
	cursorPos_ = imgsPos_;

	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_RIGHT))
	{
		//次の対戦人数を選択
		selectPattern_ = static_cast<CommonData::BATTLE_PATTERN>(static_cast<int>(selectPattern_) + 1);

		if (selectPattern_ > CommonData::BATTLE_PATTERN::P4)
		{
			selectPattern_ = CommonData::BATTLE_PATTERN::P1C1;
		}
	}
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_LEFT))
	{
		//前の対戦人数を選択
		selectPattern_ = static_cast<CommonData::BATTLE_PATTERN>(static_cast<int>(selectPattern_) - 1);

		if (selectPattern_ < CommonData::BATTLE_PATTERN::P1C1)
		{
			selectPattern_ = CommonData::BATTLE_PATTERN::P4;
		}
	}

	//決定
	switch (selectPattern_)
	{
	case CommonData::BATTLE_PATTERN::P1C1:
	case CommonData::BATTLE_PATTERN::P1C2:
	case CommonData::BATTLE_PATTERN::P1C3:
		break;

	case CommonData::BATTLE_PATTERN::P2:
	case CommonData::BATTLE_PATTERN::P2C1:
	case CommonData::BATTLE_PATTERN::P2C2:
		if (padNum < 2)
		{
			isNextSelect = false;
		}
		break;

	case CommonData::BATTLE_PATTERN::P3:
	case CommonData::BATTLE_PATTERN::P3C1:
		if (padNum < 3)
		{
			isNextSelect = false;
		}
		break;

	case CommonData::BATTLE_PATTERN::P4:
		if (padNum < 4)
		{
			isNextSelect = false;
		}
		break;
	}

	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_1) && isNextSelect)
	{
		//決定音
		sound_->PlaySE(SoundManager::SE_TYPE::CLICK, DX_PLAYTYPE_BACK, CLICK_VOLUME);

		//パッドをつないでいないとき
		if (padNum == 0)
		{
			//敵の難易度決めに移行
			selectItem_ = SELECT_ITEM::DIFFICULTY;

			return;
		}

		//パッドの確認に移行
		selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);
	}
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_2))
	{
		//キャンセル音
		sound_->PlaySE(SoundManager::SE_TYPE::CANCEL, DX_PLAYTYPE_BACK, CANCEL_VOLUME);

		//タイトルへ移行
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
	}

	//決定
	switch (selectPattern_)
	{
	case CommonData::BATTLE_PATTERN::P1C1:
	case CommonData::BATTLE_PATTERN::P1C2:
	case CommonData::BATTLE_PATTERN::P1C3:
		playerNum_ = static_cast<int>(CommonData::TYPE::P1);
		break;

	case CommonData::BATTLE_PATTERN::P2:
	case CommonData::BATTLE_PATTERN::P2C1:
	case CommonData::BATTLE_PATTERN::P2C2:
		playerNum_ = static_cast<int>(CommonData::TYPE::P2);
		break;

	case CommonData::BATTLE_PATTERN::P3:
	case CommonData::BATTLE_PATTERN::P3C1:
		playerNum_ = static_cast<int>(CommonData::TYPE::P3);
		break;

	case CommonData::BATTLE_PATTERN::P4:
		playerNum_ = static_cast<int>(CommonData::TYPE::P4);
		break;
	}

	switch (selectPattern_)
	{
	case CommonData::BATTLE_PATTERN::P2:
	case CommonData::BATTLE_PATTERN::P3:
	case CommonData::BATTLE_PATTERN::P4:

		cpuNum_ = CommonData::CPU_NUM::NONE;
		break;

	case CommonData::BATTLE_PATTERN::P1C1:
	case CommonData::BATTLE_PATTERN::P2C1:
	case CommonData::BATTLE_PATTERN::P3C1:

		cpuNum_ = CommonData::CPU_NUM::C1;
		break;

	case CommonData::BATTLE_PATTERN::P1C2:
	case CommonData::BATTLE_PATTERN::P2C2:

		cpuNum_ = CommonData::CPU_NUM::C2;
		break;

	case CommonData::BATTLE_PATTERN::P1C3:

		cpuNum_ = CommonData::CPU_NUM::C3;
		break;
	}
}

void SceneSelect::CheckPadUpdate(void)
{
	//入力マネージャーのインスタンス
	auto ins = InputManager::GetInstance();

	//現在のパッド接続数
	int padNum = GetJoypadNum();

	//モデルの位置、大きさ、角度
	MV1SetPosition(pModel_[selectPadNum_ - 1], pPos_);
	MV1SetScale(pModel_[selectPadNum_ - 1], pScl_);
	MV1SetRotationXYZ(pModel_[selectPadNum_ - 1], pRot_);

	//アニメーション再生
	//-------------------------------------------

	//アニメーション時間の進行
	stepAnim_ += (speedAnim_ * Utility::DELTA_TIME);
	if (stepAnim_ > animTotalTime_[selectPadNum_ - 1])
	{
		//ループ再生
		stepAnim_ = 0.0f;
	}

	//再生するアニメーション時間の設定
	MV1SetAttachAnimTime(pModel_[selectPadNum_ - 1], animAttachNo_[selectPadNum_ - 1], stepAnim_);

	//アニメーション速度
	speedAnim_ = SPEED_ANIM;

	//パッドをつないでいないとき
	if (padNum == 0)
	{
		//敵の難易度決めに移行
		selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);
	}
	//パッドをつないでいるとき
	else if (playerNum_ >= selectPadNum_)
	{
		//バイブレーションをするか
		if (isVib_)
		{
			//パッドのバイブレーションを始める
			StartJoypadVibration(selectPadNum_, VIB_STR, VIB_TIME);

			//バイブレーション終了
			isVib_ = false;
		}

		//選択中のパッドがAボタンを押したら
		if (ins.IsJoypadKeyPush(selectPadNum_, PAD_INPUT_1))
		{
			//決定音
			sound_->PlaySE(SoundManager::SE_TYPE::CLICK, DX_PLAYTYPE_BACK, CLICK_VOLUME);

			//パッドのバイブレーションを止める
			StopJoypadVibration(selectPadNum_);

			//次のパッドへ
			selectPadNum_++;

			//バイブレーションを始める
			isVib_ = true;
		}
		//選択中のパッドがBボタンを押したら
		else if (ins.IsJoypadKeyPush(selectPadNum_, PAD_INPUT_2))
		{
			//キャンセル音
			sound_->PlaySE(SoundManager::SE_TYPE::CANCEL, DX_PLAYTYPE_BACK, CANCEL_VOLUME);

			//パッドのバイブレーションを止める
			StopJoypadVibration(selectPadNum_);

			//前のパッドへ
			selectPadNum_--;

			//バイブレーションを始める
			isVib_ = true;
		}

		//全員の確認が終わった
		if (playerNum_ < selectPadNum_)
		{
			//選択権を1Pに渡す
			selectPadNum_ = padNum > 0 ? SELECT_PAD_P1 : SELECT_KEYBOARD;


			//CPUの難易度決めに移行
			selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);
		}
		//戻る動作をした
		else if (selectPadNum_ <= 0)
		{
			//選択権を1Pに渡す
			selectPadNum_ = padNum > 0 ? SELECT_PAD_P1 : SELECT_KEYBOARD;

			//バイブレーションフラグの初期化
			isVib_ = true;

			//対戦人数決めに戻る
			selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) - 1);
		}
	}
	//すべてのパッドの確認が終わったら
	else
	{
		//選択権を1Pに渡す
		selectPadNum_ = padNum > 0 ? SELECT_PAD_P1 : SELECT_KEYBOARD;

		//CPUの難易度決めに移行
		selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);
	}
}

void SceneSelect::DifficultyUpdate(void)
{
	//入力マネージャーのインスタンス
	auto ins = InputManager::GetInstance();

	//画像の位置
	imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };

	//カーソル位置
	cursorPos_ = imgsPos_;

	//CPUがいないなら
	if (cpuNum_ == CommonData::CPU_NUM::NONE)
	{
		//スキップ
		selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);
	}

	//選択欄を右に
	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_RIGHT))
	{
		//右に一つずらす
		selectDifficulty_ = static_cast<CommonData::DIFFICULTY>(static_cast<int>(selectDifficulty_) + 1);

		//端なら
		if (static_cast<int>(selectDifficulty_) > static_cast<int>(CommonData::DIFFICULTY::HARD))
		{
			//左端に戻す
			selectDifficulty_ = CommonData::DIFFICULTY::EASY;
		}
	}
	//選択欄を左に
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_LEFT))
	{
		//左に一つずらす
		selectDifficulty_ = static_cast<CommonData::DIFFICULTY>(static_cast<int>(selectDifficulty_) - 1);

		//恥なら
		if (static_cast<int>(selectDifficulty_) < static_cast<int>(CommonData::DIFFICULTY::EASY))
		{
			//右端に戻す
			selectDifficulty_ = CommonData::DIFFICULTY::HARD;
		}
	}

	//決定
	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_1))
	{
		//決定音
		sound_->PlaySE(SoundManager::SE_TYPE::CLICK, DX_PLAYTYPE_BACK, CLICK_VOLUME);

		//次のCPUの設定に移行
		compareCpuNum_++;

		//一体分の難易度を渡す
		CommonData::GetInstance().SetDifficulty(selectDifficulty_);

		//初期化
		selectDifficulty_ = CommonData::DIFFICULTY::EASY;
	}
	//キャンセル
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_2))
	{
		//キャンセル音
		sound_->PlaySE(SoundManager::SE_TYPE::CANCEL, DX_PLAYTYPE_BACK, CANCEL_VOLUME);

		//前のCPUの設定に移行
		compareCpuNum_--;

		//最初のCPU以外なら
		if (compareCpuNum_ >= 0)
		{
			//一体分の難易度を削除
			CommonData::GetInstance().DeleteDiff();

			//初期化
			selectDifficulty_ = CommonData::DIFFICULTY::EASY;
		}

		//画像の位置
		imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };

		//カーソル位置
		cursorPos_ = imgsPos_;
	}

	//最初のCPUの設定からキャンセルしたなら
	if (compareCpuNum_ < 0)
	{
		//変数の初期化
		compareCpuNum_ = 0;

		//対戦人数決めに戻る
		selectItem_ = SELECT_ITEM::BATTLE_PATTERN;

		//画像の位置
		imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };

		//カーソル位置
		cursorPos_ = imgsPos_;
	}
	//全CPUの設定が終わった
	else if (compareCpuNum_ >= static_cast<int>(cpuNum_))
	{
		//変数の初期化
		compareCpuNum_ = 0;

		//ルール決めに進む
		selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);

		//画像の位置
		imgsPos_ = { (Application::SCREEN_SIZE_X - BOARD_SIZE_X) +
			((BOARD_SIZE_X - ((RULE_SIZE_X) * static_cast<int>(CommonData::RULE::BREAK_TILE) +
				(RULE_DIS_X * (static_cast<int>(CommonData::RULE::BREAK_TILE) - 1)))) / 2)
			,Application::SCREEN_SIZE_Y / 2 - RULE_SIZE_Y };

		//カーソルの位置
		cursorPos_ = { imgsPos_.x + ((RULE_SIZE_X + RULE_DIS_X) * (static_cast<int>(selectRule_) - 1)),imgsPos_.y };
	}
}

void SceneSelect::RuleUpdate(void)
{
	//入力マネージャーのインスタンス
	auto ins = InputManager::GetInstance();

	//画像の位置
	imgsPos_ = { (Application::SCREEN_SIZE_X - BOARD_SIZE_X) +
		((BOARD_SIZE_X - ((RULE_SIZE_X) * static_cast<int>(CommonData::RULE::BREAK_TILE) +
			(RULE_DIS_X * (static_cast<int>(CommonData::RULE::BREAK_TILE) - 1)))) / 2)
		,Application::SCREEN_SIZE_Y / 2 - RULE_SIZE_Y };

	//カーソルの位置
	cursorPos_ = { imgsPos_.x + ((RULE_SIZE_X + RULE_DIS_X) * (static_cast<int>(selectRule_) - 1)),imgsPos_.y };

	//戻った時の初期化
	if (selectRule_ == CommonData::RULE::NONE)
	{
		CommonData::GetInstance().SetRule(selectRule_);
		selectRule_ = CommonData::RULE::TIME;
	}

	//右を押した
	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_RIGHT))
	{
		//次のルールを選択
		selectRule_ = static_cast<CommonData::RULE>(static_cast<int>(selectRule_) + 1);

		//最後のルールまで行ってたなら
		if (selectRule_ > CommonData::RULE::BREAK_TILE)
		{
			//最初のルールに戻る
			selectRule_ = CommonData::RULE::TIME;
		}
	}
	//左を押した
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_LEFT))
	{
		//前のルールを選択
		selectRule_ = static_cast<CommonData::RULE>(static_cast<int>(selectRule_) - 1);

		//最初のルールまで行ってたなら
		if (selectRule_ < CommonData::RULE::TIME)
		{
			//最後のルールに戻る
			selectRule_ = CommonData::RULE::BREAK_TILE;
		}
	}

	//決定
	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_1))
	{
		//決定音
		sound_->PlaySE(SoundManager::SE_TYPE::CLICK, DX_PLAYTYPE_BACK, CLICK_VOLUME);

		//対戦人数の最終決定
		CommonData::GetInstance().SetBattlePattern(selectPattern_);

		//ルールの最終決定
		CommonData::GetInstance().SetRule(selectRule_);

		//ゲームシーンに移る
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
	}
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_2))
	{
		//キャンセル音
		sound_->PlaySE(SoundManager::SE_TYPE::CANCEL, DX_PLAYTYPE_BACK, CANCEL_VOLUME);

		//プレイヤー数決めに戻る
		selectItem_ = SELECT_ITEM::BATTLE_PATTERN;

		//CPU難易度の配列を削除
		CommonData::GetInstance().DeleteDiffAll();

		//画像の位置
		imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };

		//カーソル位置
		cursorPos_ = imgsPos_;
	}
}

void SceneSelect::BattlePatternDraw(void)
{
	//カーソル描画
	CursorDraw(BATTLE_NUM_SIZE_X, BATTLE_NUM_SIZE_Y);

	//対戦人数の選択肢
	DrawRotaGraph(imgsPos_.x, imgsPos_.y, 1.0, 0.0, bpImages_[selectPattern_], true);

	//対戦人数の文字用画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 2)
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT
		, 1.0
		, 0.0
		, padNumImage_
		, true);

	//対戦人数
	DrawFormatStringToHandle(Application::SCREEN_SIZE_X / 2 - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) - BATTLE_PAT_SHIFT_POS_X
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT - BATTLE_PAT_SHIFT_POS_Y
		, Utility::COLOR_BLACK
		, padFont_
		, " 対戦人数\nPlayer ×%d\n CPU ×%d"
		, playerNum_
		, cpuNum_);

	//コントローラーの接続数画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 + (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 2)
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT
		, 1.0
		, 0.0
		, padNumImage_
		, true);

	//コントローラーの接続数
	DrawFormatStringToHandle(Application::SCREEN_SIZE_X / 2 + (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) - PADNUM_SHIFT_POS_X
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT - PADNUM_SHIFT_POS_Y
		, Utility::COLOR_BLACK
		, padFont_
		, "接続されている\nコントローラー\n   現在%d台"
		, GetJoypadNum());

	//右矢印画像
	DrawRotaGraph(imgsPos_.x + (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) - ARROW_SHIFT_POS_X
		, imgsPos_.y + ARROW_SHIFT_POS_Y
		, arrowSize_
		, 0.0
		, arrowImage_
		, true);

	//左矢印画像
	DrawRotaGraph(imgsPos_.x - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) + ARROW_SHIFT_POS_X
		, imgsPos_.y + ARROW_SHIFT_POS_Y
		, arrowSize_
		, 0.0
		, arrowImage_
		, true
		, true);
}

void SceneSelect::CheckPadDraw(void)
{
	//フォント
	int fontSize = 70;
	fontHandle_ = CreateFontToHandle(NULL, fontSize, 3, DX_FONTTYPE_EDGE);
	int color = Utility::COLOR_WHITE;

	switch (selectPadNum_)
	{
	case 1:
		color = 0x0000ff;
		break;

	case 2:
		color = 0xff0000;
		break;

	case 3:
		color = 0x00ff00;
		break;

	case 4:
		color = 0xffff00;
		break;
	}

	MV1DrawModel(pModel_[selectPadNum_ - 1]);

	DrawFormatStringFToHandle(fontSize * 5.5f, Application::SCREEN_SIZE_Y / (3.0f / 2.0f) - fontSize, color, fontHandle_, "     P%d Press A!!", selectPadNum_);
}

void SceneSelect::DifficultyDraw(void)
{
	//カーソル
	CursorDraw(DIFFICULTY_SIZE_X, DIFFICULTY_SIZE_Y);

	//フォント
	int fontSize = 30;
	fontHandle_ = CreateFontToHandle(NULL, fontSize, 3, DX_FONTTYPE_EDGE);
	int color = 0xffffff;

	switch (selectPattern_)
	{
	case CommonData::BATTLE_PATTERN::P1C1:
	case CommonData::BATTLE_PATTERN::P1C2:
	case CommonData::BATTLE_PATTERN::P1C3:
		if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C1)))
		{
			color = 0xff0000;
		}
		else if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C2)))
		{
			color = 0x00ff00;
		}
		else if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C3)))
		{
			color = 0xffff00;
		}

		break;

	case CommonData::BATTLE_PATTERN::P2C1:
	case CommonData::BATTLE_PATTERN::P2C2:
		if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C1)))
		{
			color = 0x00ff00;
		}
		else if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C2)))
		{
			color = 0xffff00;
		}

		break;

	case CommonData::BATTLE_PATTERN::P3C1:
		if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C1)))
		{
			color = 0xffff00;
		}

		break;
	}

	//対戦人数の文字用画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 2)
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT
		, 1.0
		, 0.0
		, padNumImage_
		, true);

	//対戦人数
	DrawFormatStringToHandle(Application::SCREEN_SIZE_X / 2 - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) - DIFFICULTY_SHIFT_SIZE_X
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT - DIFFICULTY_SHIFT_SIZE_Y
		, color
		, fontHandle_
		, "プレイヤー%dの強さを\n 選択してください"
		, static_cast<int>(playerNum_) + (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_)));

	//右矢印画像
	DrawRotaGraph(imgsPos_.x + (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) - ARROW_SHIFT_POS_X
		, imgsPos_.y + ARROW_SHIFT_POS_Y
		, arrowSize_
		, 0.0
		, arrowImage_
		, true);

	//左矢印画像
	DrawRotaGraph(imgsPos_.x - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) + ARROW_SHIFT_POS_X
		, imgsPos_.y + ARROW_SHIFT_POS_Y
		, arrowSize_
		, 0.0
		, arrowImage_
		, true
		, true);

	DrawRotaGraph(imgsPos_.x, imgsPos_.y, 1.0, 0.0, diffImages_[selectDifficulty_], true);
}

void SceneSelect::RuleDraw(void)
{
	//カーソル
	CursorDraw(RULE_SIZE_X, RULE_SIZE_Y);

	//フォント
	int fontSize = 100;
	fontHandle_ = CreateFontToHandle(NULL, fontSize, 3);

	for (int r = 0; r < static_cast<int>(CommonData::RULE::BREAK_TILE); r++)
	{
		DrawRotaGraph(imgsPos_.x + ((RULE_SIZE_X + RULE_DIS_X) * r), imgsPos_.y, 1.0, 0.0, ruleImages_[static_cast<CommonData::RULE>(r + 1)], true, false);
	}

	DrawRotaGraph(imgsPos_.x + (RULE_SIZE_X + RULE_DIS_X), Application::SCREEN_SIZE_Y / 2 + RULE_DIS_Y, 1.0, 0.0, ruleExplainImages_[selectRule_], true);
}
