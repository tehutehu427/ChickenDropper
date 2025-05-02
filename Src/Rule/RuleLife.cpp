#include"../Application.h"
#include"../Object/Player.h"
#include"RuleLife.h"

RuleLife::RuleLife(SceneGame* parent) : RuleBase(parent)
{
	sceneGame_ = parent;
}

bool RuleLife::Init(void)
{
	//体力画像
	lifeImages_[static_cast<int>(CommonData::TYPE::P1) - 1] = LoadGraph((Application::PATH_IMAGE + "Life Blue.png").c_str());
	lifeImages_[static_cast<int>(CommonData::TYPE::P2) - 1] = LoadGraph((Application::PATH_IMAGE + "Life Red.png").c_str());
	lifeImages_[static_cast<int>(CommonData::TYPE::P3) - 1] = LoadGraph((Application::PATH_IMAGE + "Life Green.png").c_str());
	lifeImages_[static_cast<int>(CommonData::TYPE::P4) - 1] = LoadGraph((Application::PATH_IMAGE + "Life Yellow.png").c_str());
	
	//基本初期化
	RuleBase::Init();

	//変数の初期化
	statePos_ = { 0.0f,0.0f };

	return true;
}

void RuleLife::Update(void)
{
	//プレイヤーのやられた人数を取得
	int deadPlayer = 0;
	auto playerIns = sceneGame_->GetPlayer();
	size_t size = playerIns.size();

	for (auto player : playerIns)
	{
		//プレイヤーがやられたか
		if (player->GetState() == Player::STATE::DEAD)
		{
			//やられた人数増加
			deadPlayer++;
		}
	}

	//プレイヤーが1人を除きやられた
	if (deadPlayer >= static_cast<int>(size) - 1)
	{
		for (auto player : playerIns)
		{
			//死亡状態かどうか
			if (player->GetState() != Player::STATE::DEAD)
			{
				//死亡時間計測
				player->SetDeadTime();
			}

			//仮順位で初期化
			InitRank();
		}

		for (auto player : playerIns)
		{
			//死亡時間で比較
			DeadTimeCompare(player);
		}

		//一度比較が終わった後で
		for (auto player : playerIns)
		{
			//死亡時間が同時かどうかも比較
			IsSameDeadTime(player);
		}

		//ゲーム終了
		sceneGame_->EndGame();
		return;
	}

	//ステージ縮小のフラグ
	if (!isNarrowStage_
		&& (timeCnt_.Min >= NARROW_LIMIT.Min
			&& timeCnt_.Sec >= NARROW_LIMIT.Sec)
		|| deadPlayer >= 1)
	{
		//縮小開始
		isNarrowStage_ = true;
	}

	//秒数と分数の調整
	if (timeCnt_.Sec > SEC_LIMIT)
	{
		timeCnt_.Min += timeCnt_.Sec / SEC_2_MIN;
		timeCnt_.Sec = timeCnt_.Sec % SEC_2_MIN;
	}

	//秒数カウンタ
	if (GetNowCount() - secCount_ > ONE_SEC)
	{
		//秒数カウント初期化
		secCount_ = GetNowCount();

		//タイマー
		timeCnt_.Sec++;
	}
	
	//基本更新
	RuleBase::Update();
}

void RuleLife::Draw(void)
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
			//ステータス欄
			DrawRotaGraph(statePos_.x + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, stateImages_[i], true);

			for (int l = 0; l < sceneGame_->GetPlayer()[i]->GetLife(); l++)
			{
				//残りライフ
				DrawRotaGraph((statePos_.x + STATE_SIZE_X / 2 - (LIFE_SIZE_X / 2 + LIFE_TO_STATE_POS)) 
					+ ((STATE_SIZE_X + STATE_DIS) * i) 
					- l * (LIFE_SIZE_X + LIFE_DIS)
					, statePos_.y
					, 1.0
					, 0.0
					, lifeImages_[i]
					, true);
			}
		}
		else
		{
			//ステータス欄
			DrawRotaGraph(statePos_.x + STATE_DIS_TO_TIME +((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, stateImages_[i], true);

			for (int l = 0; l < sceneGame_->GetPlayer()[i]->GetLife(); l++)
			{
				//残りライフ
				DrawRotaGraph((statePos_.x + STATE_DIS_TO_TIME + STATE_SIZE_X / 2 - (LIFE_SIZE_X / 2 + LIFE_TO_STATE_POS))
					+ ((STATE_SIZE_X + STATE_DIS) * i)
					- l * (LIFE_SIZE_X + LIFE_DIS)
					, statePos_.y
					, 1.0
					, 0.0
					, lifeImages_[i]
					, true);
			}
		}
	}

	//タイマー
	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_X / 2 - timeFontSize_ * TIME_SIZE_PER
		, timeFontSize_ / 2
		, color
		, timeFont_
		, "%.2d:%.2d"
		, timeCnt_.Min, timeCnt_.Sec
	);

	//基本描画
	RuleBase::Draw();
}

void RuleLife::DeadTimeCompare(Player* _player)
{
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//自身とは比較しない
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//自分より参照プレイヤーの方が長く生きた　かつ　自分より参照プレイヤーの方が順位が下なら
			if (_player->GetDeadTime() < player->GetDeadTime()
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

void RuleLife::IsSameDeadTime(Player* _player)
{
	//プレイヤー
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//自身とは比較しない
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//自分と参照プレイヤーのやられた時間が同じ　かつ　自分より参照プレイヤーの方が順位が高い
			if (_player->GetDeadTime() == player->GetDeadTime()
				&& _player->GetRank() > player->GetRank())
			{
				//自分の順位を参照プレイヤーに合わせる
				_player->SetRank(player->GetRank());
			}
		}
	}
}
