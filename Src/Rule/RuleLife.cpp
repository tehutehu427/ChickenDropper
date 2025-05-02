#include"../Application.h"
#include"../Object/Player.h"
#include"RuleLife.h"

RuleLife::RuleLife(SceneGame* parent) : RuleBase(parent)
{
	sceneGame_ = parent;
}

bool RuleLife::Init(void)
{
	//�̗͉摜
	lifeImages_[static_cast<int>(CommonData::TYPE::P1) - 1] = LoadGraph((Application::PATH_IMAGE + "Life Blue.png").c_str());
	lifeImages_[static_cast<int>(CommonData::TYPE::P2) - 1] = LoadGraph((Application::PATH_IMAGE + "Life Red.png").c_str());
	lifeImages_[static_cast<int>(CommonData::TYPE::P3) - 1] = LoadGraph((Application::PATH_IMAGE + "Life Green.png").c_str());
	lifeImages_[static_cast<int>(CommonData::TYPE::P4) - 1] = LoadGraph((Application::PATH_IMAGE + "Life Yellow.png").c_str());
	
	//��{������
	RuleBase::Init();

	//�ϐ��̏�����
	statePos_ = { 0.0f,0.0f };

	return true;
}

void RuleLife::Update(void)
{
	//�v���C���[�̂��ꂽ�l�����擾
	int deadPlayer = 0;
	auto playerIns = sceneGame_->GetPlayer();
	size_t size = playerIns.size();

	for (auto player : playerIns)
	{
		//�v���C���[�����ꂽ��
		if (player->GetState() == Player::STATE::DEAD)
		{
			//���ꂽ�l������
			deadPlayer++;
		}
	}

	//�v���C���[��1�l���������ꂽ
	if (deadPlayer >= static_cast<int>(size) - 1)
	{
		for (auto player : playerIns)
		{
			//���S��Ԃ��ǂ���
			if (player->GetState() != Player::STATE::DEAD)
			{
				//���S���Ԍv��
				player->SetDeadTime();
			}

			//�����ʂŏ�����
			InitRank();
		}

		for (auto player : playerIns)
		{
			//���S���ԂŔ�r
			DeadTimeCompare(player);
		}

		//��x��r���I��������
		for (auto player : playerIns)
		{
			//���S���Ԃ��������ǂ�������r
			IsSameDeadTime(player);
		}

		//�Q�[���I��
		sceneGame_->EndGame();
		return;
	}

	//�X�e�[�W�k���̃t���O
	if (!isNarrowStage_
		&& (timeCnt_.Min >= NARROW_LIMIT.Min
			&& timeCnt_.Sec >= NARROW_LIMIT.Sec)
		|| deadPlayer >= 1)
	{
		//�k���J�n
		isNarrowStage_ = true;
	}

	//�b���ƕ����̒���
	if (timeCnt_.Sec > SEC_LIMIT)
	{
		timeCnt_.Min += timeCnt_.Sec / SEC_2_MIN;
		timeCnt_.Sec = timeCnt_.Sec % SEC_2_MIN;
	}

	//�b���J�E���^
	if (GetNowCount() - secCount_ > ONE_SEC)
	{
		//�b���J�E���g������
		secCount_ = GetNowCount();

		//�^�C�}�[
		timeCnt_.Sec++;
	}
	
	//��{�X�V
	RuleBase::Update();
}

void RuleLife::Draw(void)
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
			//�X�e�[�^�X��
			DrawRotaGraph(statePos_.x + ((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, stateImages_[i], true);

			for (int l = 0; l < sceneGame_->GetPlayer()[i]->GetLife(); l++)
			{
				//�c�胉�C�t
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
			//�X�e�[�^�X��
			DrawRotaGraph(statePos_.x + STATE_DIS_TO_TIME +((STATE_SIZE_X + STATE_DIS) * i), statePos_.y, 1.0, 0.0, stateImages_[i], true);

			for (int l = 0; l < sceneGame_->GetPlayer()[i]->GetLife(); l++)
			{
				//�c�胉�C�t
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

	//�^�C�}�[
	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_X / 2 - timeFontSize_ * TIME_SIZE_PER
		, timeFontSize_ / 2
		, color
		, timeFont_
		, "%.2d:%.2d"
		, timeCnt_.Min, timeCnt_.Sec
	);

	//��{�`��
	RuleBase::Draw();
}

void RuleLife::DeadTimeCompare(Player* _player)
{
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//���g�Ƃ͔�r���Ȃ�
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//�������Q�ƃv���C���[�̕��������������@���@�������Q�ƃv���C���[�̕������ʂ����Ȃ�
			if (_player->GetDeadTime() < player->GetDeadTime()
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

void RuleLife::IsSameDeadTime(Player* _player)
{
	//�v���C���[
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//���g�Ƃ͔�r���Ȃ�
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//�����ƎQ�ƃv���C���[�̂��ꂽ���Ԃ������@���@�������Q�ƃv���C���[�̕������ʂ�����
			if (_player->GetDeadTime() == player->GetDeadTime()
				&& _player->GetRank() > player->GetRank())
			{
				//�����̏��ʂ��Q�ƃv���C���[�ɍ��킹��
				_player->SetRank(player->GetRank());
			}
		}
	}
}
