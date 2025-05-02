#include"../Application.h"
#include"../Object/Player.h"
#include "RuleBreakTile.h"

RuleBreakTile::RuleBreakTile(SceneGame* parent) : RuleBase(parent)
{
	sceneGame_ = parent;
}

bool RuleBreakTile::Init(void)
{
	//タイル数画像
	tileNumImage_ = LoadGraph((Application::PATH_IMAGE + "TileNum.png").c_str());

	RuleBase::Init();

	//時間制限設定
	timeLimit_ = TIME_LIMIT;

	return false;
}

void RuleBreakTile::Update(void)
{
	auto playerIns = sceneGame_->GetPlayer();

	if (timeLimit_.Min <= 0 && timeLimit_.Sec <= 0)
	{
		//仮の順位で初期化
		InitRank();

		for (auto player : playerIns)
		{
			//スコアによって順位を変更
			BrokeTimeNumCompare(player);
		}

		for (auto player : playerIns)
		{
			//スコアが同じかどうかを調べる　同じなら順位を合わせる
			IsSameBrokeTileNum(player);
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
		timeLimit_.Sec--;
	}

	//次の位に進む
	if (timeLimit_.Sec < 0)
	{
		timeLimit_.Sec = SEC_LIMIT;
		timeLimit_.Min--;
	}

	RuleBase::Update();
}

void RuleBreakTile::Draw(void)
{
	//色
	unsigned int color = NodyUtility::COLOR_WHITE;

	//ステータス欄の基本位置
	statePos_ = { STATE_POS_X,STATE_POS_Y };

	//プレイヤーの数
	size_t size = sceneGame_->GetPlayer().size();

	//ステータス欄の描画
	for (int i = 0; i < size; i++)
	{
		//左半分
		if (i < static_cast<int>(CommonData::TYPE::P4) / 2)
		{
			//ステータス欄画像
			DrawRotaGraph(statePos_.x + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, stateImages_[i], true);
			
			//破壊タイル画像
			DrawRotaGraph(statePos_.x + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, tileNumImage_, true);

			//タイル数
			DrawFormatStringToHandle(statePos_.x + ((STATE_SIZE_X + STATE_DIS) * i) + SCORE_2_TILE_DIS
				, statePos_.y - SCORE_SIZE / 2
				, color
				, scoreFont_
				, "×%d"
				, sceneGame_->GetBrokeTileNum(sceneGame_->GetPlayer()[i]));
		}
		//右半分
		else
		{
			//ステータス欄画像
			DrawRotaGraph(statePos_.x + STATE_DIS_TO_TIME + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, stateImages_[i], true);

			//破壊タイル画像
			DrawRotaGraph(statePos_.x + STATE_DIS_TO_TIME + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, tileNumImage_, true);
			
			//タイル数
			DrawFormatStringToHandle(statePos_.x + STATE_DIS_TO_TIME + ((STATE_SIZE_X + STATE_DIS) * i) + SCORE_2_TILE_DIS
				, statePos_.y - SCORE_SIZE / 2
				, color
				, scoreFont_
				, "×%d"
				, sceneGame_->GetBrokeTileNum(sceneGame_->GetPlayer()[i]));
		}
	}

	//タイマー
	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_X / 2 - timeFontSize_ * TIME_SIZE_PER
		, timeFontSize_ / 2
		, color
		, timeFont_
		, "%.2d:%.2d"
		, timeLimit_.Min, timeLimit_.Sec
	);

	//基本描画
	RuleBase::Draw();
}

void RuleBreakTile::BrokeTimeNumCompare(Player* _player)
{
	//プレイヤー
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//自身とは比較しない
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//自分より参照プレイヤーの方が多くタイルを壊した　かつ　自分より参照プレイヤーの方が順位が下なら
			if (_player->GetBrokeTileNum() < player->GetBrokeTileNum()
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

void RuleBreakTile::IsSameBrokeTileNum(Player* _player)
{
	//プレイヤー
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//自身とは比較しない
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//自分と参照プレイヤーの壊したタイルの数が同じ　かつ　自分より参照プレイヤーの方が順位が高い
			if (_player->GetBrokeTileNum() == player->GetBrokeTileNum()
				&& _player->GetRank() > player->GetRank())
			{
				//自分の順位を参照プレイヤーに合わせる
				_player->SetRank(player->GetRank());
			}
		}
	}
}
