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
	//���\�[�X�}�l�[�W���擾
	ResourceManager& res = ResourceManager::GetInstance();

	//�L���X�R�A�摜
	killScoreImage_ = res.Load(ResourceManager::SRC::KILL_SCORE).handleId_;

	RuleBase::Init();

	//���Ԑ����ݒ�
	timeLimit_ = TIME_LIMIT;

    return true;
}

void RuleTime::Update(void)
{
	auto playerIns = sceneGame_->GetPlayer();

	if (timeLimit_.Min <= 0 && timeLimit_.Sec <= 0)
	{
		//���̏��ʂŏ�����
		InitRank();
		
		for (auto player : playerIns)
		{
			//�X�R�A�ɂ���ď��ʂ�ύX
			ScoreCompare(player);
		}

		for (auto player : playerIns)
		{
			//�X�R�A���������ǂ����𒲂ׂ�@�����Ȃ珇�ʂ����킹��
			IsSameScore(player);
		}

		//�Q�[���̏I�����f
		sceneGame_->EndGame();

		return;
	}

	//�X�e�[�W�k���̃t���O
	if (!isNarrowStage_
		&& (timeLimit_.Min <= NARROW_LIMIT.Min
			&& timeLimit_.Sec <= NARROW_LIMIT.Sec))
	{
		isNarrowStage_ = true;
	}

	//�b���ƕ����̒���
	if (timeLimit_.Sec > SEC_LIMIT)
	{
		timeLimit_.Min += timeLimit_.Sec / SEC_2_MIN;
		timeLimit_.Sec = timeLimit_.Sec % SEC_2_MIN;
	}

	if (GetNowCount() - secCount_ > ONE_SEC)
	{
		//�b���J�E���g������
		secCount_ = GetNowCount();

		//���Ԑ����J�E���g
		timeLimit_.Sec --;
	}

	//���̈ʂɐi��
	if (timeLimit_.Sec < 0)
	{
		timeLimit_.Sec = SEC_LIMIT;
		timeLimit_.Min--;
	}

	RuleBase::Update();
}

void RuleTime::Draw(void)
{
	//�F
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

	//�X�e�[�^�X���̕`��
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
				, "�~%d"
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
				, "�~%d"
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
		//���g�Ƃ͔�r���Ȃ�
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//�������Q�ƃv���C���[�̕������_�������@���@�������Q�ƃv���C���[�̕������ʂ����Ȃ�
			if (_player->GetScore() < player->GetScore()
				&& _player->GetRank() < player->GetRank())
			{
				//���ʂ̓���ւ�
				int change = _player->GetRank();
				_player->SetRank(player->GetRank());
				player->SetRank(change);
			}
		}
	}
}

void RuleTime::IsSameScore(Player* _player)
{
	//�v���C���[
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//���g�Ƃ͔�r���Ȃ�
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//�����ƎQ�ƃv���C���[�̃X�R�A�������@���@�������Q�ƃv���C���[�̕������ʂ�����
			if (_player->GetScore() == player->GetScore()
				&& _player->GetRank() > player->GetRank())
			{
				//�����̏��ʂ��Q�ƃv���C���[�ɍ��킹��
				_player->SetRank(player->GetRank());
			}
		}
	}
}
