#include"../Application.h"
#include"../Manager/Resource.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SceneManager.h"
#include"../Object/Player.h"
#include "RuleTime.h"

RuleTime::RuleTime(SceneGame* parent) : RuleBase(parent)
{
	sceneGame_ = parent;
	killScoreImage_ = -1;
}

bool RuleTime::Init(void)
{
	//リソースマネージャ取得
	ResourceManager& res = ResourceManager::GetInstance();

	//キルスコア画像
	killScoreImage_ = res.Load(ResourceManager::SRC::KILL_SCORE).handleId_;

	RuleBase::Init();

	//時間制限設定
	timeLimit_ = TIME_LIMIT;

    return true;
}

void RuleTime::Update(void)
{
	auto playerIns = sceneGame_->GetPlayer();

	if (timeLimit_.Min <= 0 && timeLimit_.Sec <= 0)
	{
		//仮の順位で初期化
		InitRank();
		
		for (auto player : playerIns)
		{
			//スコアによって順位を変更
			ScoreCompare(player);
		}

		for (auto player : playerIns)
		{
			//スコアが同じかどうかを調べる　同じなら順位を合わせる
			IsSameScore(player);
		}

		//ゲームの終了判断
		sceneGame_->EndGame();

		return;
	}

	//ステージ縮小のフラグ
	if (!isNarrowStage_
		&& (timeLimit_.Min <= NARROW_LIMIT.Min
			&& timeLimit_.Sec <= NARROW_LIMIT.Sec))
	{
		isNarrowStage_ = true;
	}

	//秒数と分数の調整
	if (timeLimit_.Sec > SEC_LIMIT)
	{
		timeLimit_.Min += timeLimit_.Sec / SEC_2_MIN;
		timeLimit_.Sec = timeLimit_.Sec % SEC_2_MIN;
	}

	if (GetNowCount() - secCount_ > ONE_SEC)
	{
		//秒数カウント初期化
		secCount_ = GetNowCount();

		//時間制限カウント
		timeLimit_.Sec --;
	}

	//次の位に進む
	if (timeLimit_.Sec < 0)
	{
		timeLimit_.Sec = SEC_LIMIT;
		timeLimit_.Min--;
	}

	RuleBase::Update();
}

void RuleTime::Draw(void)
{
	//色
	unsigned int color = Utility::COLOR_WHITE;

	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_X / 2 - timeFontSize_ * TIME_SIZE_PER
		, timeFontSize_ / 2
		, color
		, timeFont_
		, "%.2d:%.2d"
		, timeLimit_.Min, timeLimit_.Sec
	);

	statePos_ = { STATE_POS_X,STATE_POS_Y };

	size_t size = sceneGame_->GetPlayer().size();

	//ステータス欄の描画
	for (int i = 0; i < size; i++)
	{
		if (i < static_cast<int>(CommonData::TYPE::P4) / 2)
		{
			DrawRotaGraph(statePos_.x + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, stateImages_[i], true);
			DrawRotaGraph(statePos_.x + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, killScoreImage_, true);
			DrawFormatStringToHandle(statePos_.x + ((STATE_SIZE_X + STATE_DIS) * i) + SCORE_2_KILLSCORE_DIS
				, statePos_.y - SCORE_SIZE / 2
				, color
				, scoreFont_
				, "×%d"
				, sceneGame_->GetPlayerScore(sceneGame_->GetPlayer()[i]));
		}
		else
		{
			DrawRotaGraph(statePos_.x + STATE_DIS_TO_TIME + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, stateImages_[i], true);
			DrawRotaGraph(statePos_.x + STATE_DIS_TO_TIME + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, killScoreImage_, true);
			DrawFormatStringToHandle(statePos_.x + STATE_DIS_TO_TIME + ((STATE_SIZE_X + STATE_DIS) * i) + SCORE_2_KILLSCORE_DIS
				, statePos_.y - SCORE_SIZE / 2
				, color
				, scoreFont_
				, "×%d"
				, sceneGame_->GetPlayerScore(sceneGame_->GetPlayer()[i]));
		}
	}

	RuleBase::Draw();
}

void RuleTime::ScoreCompare(Player* _player)
{
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//自身とは比較しない
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//自分より参照プレイヤーの方が得点が高い　かつ　自分より参照プレイヤーの方が順位が下なら
			if (_player->GetScore() < player->GetScore()
				&& _player->GetRank() < player->GetRank())
			{
				//順位の入れ替え
				int change = _player->GetRank();
				_player->SetRank(player->GetRank());
				player->SetRank(change);
			}
		}
	}
}

void RuleTime::IsSameScore(Player* _player)
{
	//プレイヤー
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//自身とは比較しない
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//自分と参照プレイヤーのスコアが同じ　かつ　自分より参照プレイヤーの方が順位が高い
			if (_player->GetScore() == player->GetScore()
				&& _player->GetRank() > player->GetRank())
			{
				//自分の順位を参照プレイヤーに合わせる
				_player->SetRank(player->GetRank());
			}
		}
	}
}
