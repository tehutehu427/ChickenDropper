#include"../Application.h"
#include"../Object/Player.h"
#include "RuleBreakTile.h"

RuleBreakTile::RuleBreakTile(SceneGame* parent) : RuleBase(parent)
{
	sceneGame_ = parent;
}

bool RuleBreakTile::Init(void)
{
	//�^�C�����摜
	tileNumImage_ = LoadGraph((Application::PATH_IMAGE + "TileNum.png").c_str());

	RuleBase::Init();

	//���Ԑ����ݒ�
	timeLimit_ = TIME_LIMIT;

	return false;
}

void RuleBreakTile::Update(void)
{
	auto playerIns = sceneGame_->GetPlayer();

	if (timeLimit_.Min <= 0 && timeLimit_.Sec <= 0)
	{
		//���̏��ʂŏ�����
		InitRank();

		for (auto player : playerIns)
		{
			//�X�R�A�ɂ���ď��ʂ�ύX
			BrokeTimeNumCompare(player);
		}

		for (auto player : playerIns)
		{
			//�X�R�A���������ǂ����𒲂ׂ�@�����Ȃ珇�ʂ����킹��
			IsSameBrokeTileNum(player);
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
		timeLimit_.Sec--;
	}

	//���̈ʂɐi��
	if (timeLimit_.Sec < 0)
	{
		timeLimit_.Sec = SEC_LIMIT;
		timeLimit_.Min--;
	}

	RuleBase::Update();
}

void RuleBreakTile::Draw(void)
{
	//�F
	unsigned int color = NodyUtility::COLOR_WHITE;

	//�X�e�[�^�X���̊�{�ʒu
	statePos_ = { STATE_POS_X,STATE_POS_Y };

	//�v���C���[�̐�
	size_t size = sceneGame_->GetPlayer().size();

	//�X�e�[�^�X���̕`��
	for (int i = 0; i < size; i++)
	{
		//������
		if (i < static_cast<int>(CommonData::TYPE::P4) / 2)
		{
			//�X�e�[�^�X���摜
			DrawRotaGraph(statePos_.x + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, stateImages_[i], true);
			
			//�j��^�C���摜
			DrawRotaGraph(statePos_.x + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, tileNumImage_, true);

			//�^�C����
			DrawFormatStringToHandle(statePos_.x + ((STATE_SIZE_X + STATE_DIS) * i) + SCORE_2_TILE_DIS
				, statePos_.y - SCORE_SIZE / 2
				, color
				, scoreFont_
				, "�~%d"
				, sceneGame_->GetBrokeTileNum(sceneGame_->GetPlayer()[i]));
		}
		//�E����
		else
		{
			//�X�e�[�^�X���摜
			DrawRotaGraph(statePos_.x + STATE_DIS_TO_TIME + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, stateImages_[i], true);

			//�j��^�C���摜
			DrawRotaGraph(statePos_.x + STATE_DIS_TO_TIME + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, tileNumImage_, true);
			
			//�^�C����
			DrawFormatStringToHandle(statePos_.x + STATE_DIS_TO_TIME + ((STATE_SIZE_X + STATE_DIS) * i) + SCORE_2_TILE_DIS
				, statePos_.y - SCORE_SIZE / 2
				, color
				, scoreFont_
				, "�~%d"
				, sceneGame_->GetBrokeTileNum(sceneGame_->GetPlayer()[i]));
		}
	}

	//�^�C�}�[
	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_X / 2 - timeFontSize_ * TIME_SIZE_PER
		, timeFontSize_ / 2
		, color
		, timeFont_
		, "%.2d:%.2d"
		, timeLimit_.Min, timeLimit_.Sec
	);

	//��{�`��
	RuleBase::Draw();
}

void RuleBreakTile::BrokeTimeNumCompare(Player* _player)
{
	//�v���C���[
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//���g�Ƃ͔�r���Ȃ�
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//�������Q�ƃv���C���[�̕��������^�C�����󂵂��@���@�������Q�ƃv���C���[�̕������ʂ����Ȃ�
			if (_player->GetBrokeTileNum() < player->GetBrokeTileNum()
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

void RuleBreakTile::IsSameBrokeTileNum(Player* _player)
{
	//�v���C���[
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//���g�Ƃ͔�r���Ȃ�
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//�����ƎQ�ƃv���C���[�̉󂵂��^�C���̐��������@���@�������Q�ƃv���C���[�̕������ʂ�����
			if (_player->GetBrokeTileNum() == player->GetBrokeTileNum()
				&& _player->GetRank() > player->GetRank())
			{
				//�����̏��ʂ��Q�ƃv���C���[�ɍ��킹��
				_player->SetRank(player->GetRank());
			}
		}
	}
}
