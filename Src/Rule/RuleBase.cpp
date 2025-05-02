#include"../Application.h"
#include"../Common/Easing.h"
#include"../Object/Player.h"
#include "RuleBase.h"

RuleBase::RuleBase(SceneGame* parent)
{
	sceneGame_ = parent;
}

RuleBase::~RuleBase(void)
{
}

bool RuleBase::Init(void)
{
	//ステータス欄の読み込み
	stateImages_[static_cast<int>(CommonData::TYPE::P1) - 1] = LoadGraph((Application::PATH_IMAGE + "State Blue.png").c_str());
	stateImages_[static_cast<int>(CommonData::TYPE::P2) - 1] = LoadGraph((Application::PATH_IMAGE + "State Red.png").c_str());
	stateImages_[static_cast<int>(CommonData::TYPE::P3) - 1] = LoadGraph((Application::PATH_IMAGE + "State Green.png").c_str());
	stateImages_[static_cast<int>(CommonData::TYPE::P4) - 1] = LoadGraph((Application::PATH_IMAGE + "State Yellow.png").c_str());

	//範囲縮小警告の画像読み込み
	narrowInfoImage_ = LoadGraph((Application::PATH_IMAGE + "NarrowInfo.png").c_str());

	//時間カウント
	secCount_ = GetNowCount();

	//タイマーフォント用
	timeFontSize_ = TIME_SIZE;
	timeFontThickness_ = TIME_THICKNESS;
	timeFont_ = CreateFontToHandle(NULL, timeFontSize_, timeFontThickness_, DX_FONTTYPE_EDGE);

	//スコアフォント用
	scoreFontSize_ = SCORE_SIZE;
	scoreFontThickness_ = SCORE_THICKNESS;
	scoreFont_ = CreateFontToHandle(NULL, scoreFontSize_, scoreFontThickness_, DX_FONTTYPE_EDGE);

	//ステージ縮小のフラグ
	isNarrowStage_ = false;

	//変数の初期化
	narrowInfoPos_ = 0.0f;
	narrowInfoTime_ = 0.0f;

	return true;
}

void RuleBase::Update(void)
{
	if (!isNarrowStage_)
	{
		return;
	}

	if (narrowInfoTime_ < NARROW_INFO_TIME)
	{
		narrowInfoTime_++;
		narrowInfoPos_ = SineOut(narrowInfoTime_
			, NARROW_INFO_TIME
			, Application::SCREEN_SIZE_X + NARROW_INFO_SIZE_X / 2
			, -NARROW_INFO_SIZE_X);
	}
}

void RuleBase::Draw(void)
{
	if (!isNarrowStage_)
	{
		return;
	}

	//範囲縮小警告
	DrawRotaGraph(narrowInfoPos_, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, narrowInfoImage_, true);
}

bool RuleBase::Release(void)
{
	//フォントの解放
	DeleteFontToHandle(timeFont_);
	DeleteFontToHandle(scoreFont_);

	return true;
}

bool RuleBase::GetIsNarrowStage(void)
{
	return isNarrowStage_;
}

void RuleBase::InitRank(void)
{
	//プレイヤー
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//仮順位で初期化
		player->SetRank(static_cast<int>(player->GetCharaNum()));
	}
}
