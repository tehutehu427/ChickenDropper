#include<DxLib.h>
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Manager/Resource.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"
#include"../Common/CommonData.h"
#include"../Scene/SceneGame.h"
#include"Stage.h"

//�f�t�H���g�R���X�g���N�^
Stage::Stage(void)
{
	sceneGame_ = nullptr;

	for (int tz = 0; tz < TILE_NUM; tz++)
	{
		for (int tx = 0; tx < TILE_NUM; tx++)
		{
			pos_[tz][tx] = Utility::VECTOR_ZERO;
			tileState_[tz][tx] = TILE_STATE::NORMAL;
			tileSwapTime_[tz][tx] = 0;
			attackPlayer_[tz][tx] = CommonData::TYPE::NONE;
		}
	}
}

//�f�X�g���N�^
Stage::~Stage(void)
{

}

//����������
bool Stage::Init(SceneGame* parent)
{
	ResourceManager& res = ResourceManager::GetInstance();

	//�e
	sceneGame_ = parent;

	//SE�̏�����
	se_ = new SoundManager();
	se_->SEInit();

	scale_ = { 1.0f,1.0f,1.0f };
	rot_ = { 0.0f,0.0f,0.0f };

	//�ϐ��̏�����
	if (TILE_NUM % 2 == 1)		//�^�C���������
	{
		for (int tz = -TILE_HNUM; tz < TILE_HNUM + 1; tz++)
		{
			for (int tx = -TILE_HNUM; tx < TILE_HNUM + 1; tx++)
			{
				pos_[tz + TILE_HNUM][tx + TILE_HNUM] =
				{ TILE_SIZE_X * tx
				,-TILE_HSIZE_Y
				,TILE_SIZE_Z * tz };
			}
		}

	}
	else		//�^�C����������
	{
		for (int tz = -TILE_HNUM; tz < TILE_HNUM; tz++)
		{
			for (int tx = -TILE_HNUM; tx < TILE_HNUM; tx++)
			{
				pos_[tz + TILE_HNUM][tx + TILE_HNUM] =
				{ TILE_SIZE_X * tx
				,-TILE_HSIZE_Y
				,TILE_SIZE_Z * tz };
			}
		}
	}	
	
	//����
	for (int tz = 0; tz < TILE_NUM; tz++)
	{
		for (int tx = 0; tx < TILE_NUM; tx++)
		{
			tileState_[tz][tx] = TILE_STATE::NORMAL;
			tileSwapTime_[tz][tx] = 0;
		}
	}

	//�w�i�摜�̓ǂݍ���
	backImage_ = res.Load(ResourceManager::SRC::GAME_BACK).handleId_;
	cloudImage_ = res.Load(ResourceManager::SRC::GAME_CLOUD).handleId_;

	//�X�e�[�W�̃��f���ǂݍ���
	model_[static_cast<int>(TILE_TYPE::PAT_1_1)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_1);
	model_[static_cast<int>(TILE_TYPE::PAT_1_2)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_2);
	model_[static_cast<int>(TILE_TYPE::P1_PAT_1_1)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_1_BLUE);
	model_[static_cast<int>(TILE_TYPE::P1_PAT_1_2)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_2_BLUE);
	model_[static_cast<int>(TILE_TYPE::P2_PAT_1_1)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_1_RED);
	model_[static_cast<int>(TILE_TYPE::P2_PAT_1_2)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_2_RED);
	model_[static_cast<int>(TILE_TYPE::P3_PAT_1_1)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_1_GREEN);
	model_[static_cast<int>(TILE_TYPE::P3_PAT_1_2)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_2_GREEN);
	model_[static_cast<int>(TILE_TYPE::P4_PAT_1_1)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_1_YELLOW);
	model_[static_cast<int>(TILE_TYPE::P4_PAT_1_2)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_2_YELLOW);
	model_[static_cast<int>(TILE_TYPE::PAT_1_1_END)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_1_END);
	model_[static_cast<int>(TILE_TYPE::PAT_1_2_END)] = res.LoadModelDuplicate(ResourceManager::SRC::TILE_PAT_2_END);

	for (int m = 0; m < static_cast<int>(TILE_TYPE::MAX); m++)
	{
		for (int tz = 0; tz < TILE_NUM; tz++)
		{
			for (int tx = 0; tx < TILE_NUM; tx++)
			{
				map_[tz][tx][m] = MV1DuplicateModel(model_[m]);
			}
		}
	}

	for (int m = 0; m < static_cast<int>(TILE_TYPE::MAX); m++)
	{
		for (int tz = 0; tz < TILE_NUM; tz++)
		{
			for (int tx = 0; tx < TILE_NUM; tx++)
			{
				MV1SetPosition(map_[tz][tx][m], pos_[tz][tx]);
				MV1SetScale(map_[tz][tx][m], scale_);
				MV1SetRotationXYZ(map_[tz][tx][m], rot_);
			}
		}
	}

	cloudPos_ = { 0.0f,Application::SCREEN_SIZE_Y / 2 };
	endCnt_ = 0;
	endTileNumX_ = 0;
	endTileNumZ_ = 0;
	endMoveDir_ = STAGE_NARROW_DIR;
	endTileNum_ = 0;
	stageNarrowRange_ = 0;

	return true;
}

//�X�V����
void Stage::Update(void)
{
	//�_�̈ړ�(������)
	cloudPos_.x--;
	if (cloudPos_.x < -Application::SCREEN_SIZE_X)
	{
		cloudPos_.x = 0.0f;
	}

	//�X�e�[�W�k���̃t���O
	if (sceneGame_->IsNarrowStage())
	{
		//�X�e�[�W�k��
		NarrowStage();
	}

	//�^�C���̏�ԑJ��
	for (int tz = 0; tz < TILE_NUM; tz++)
	{
		for (int tx = 0; tx < TILE_NUM; tx++)
		{
			//�J�E���g
			tileSwapTime_[tz][tx] ++;

			switch (tileState_[tz][tx])
			{
			//�U�����
			case TILE_STATE::ATTACK:
				//�U�����Ԃ��擾���Ă���
				if (atkCnt_[tz][tx] != 0.0f)
				{
					if (tileSwapTime_[tz][tx] > TILE_BREAK_TIME + atkCnt_[tz][tx])
					{
						//�^�C��������
						TileSwap(tx, tz, TILE_STATE::BROKEN);
						
						//TILEBREAK���[���Ȃ�
						if (CommonData::GetInstance().GetRule() == CommonData::RULE::BREAK_TILE)
						{
							//�󂵂��^�C�����𑝉�
							sceneGame_->IncreaseBrokeTileNum(attackPlayer_[tz][tx]);
						}
					}
				}
				break;

			//�j����
			case TILE_STATE::BROKEN:
				if (tileSwapTime_[tz][tx] > TILE_REPAIR_TIME)
				{
					TileSwap(tx, tz, TILE_STATE::NORMAL);
					TileAttackPlayer(tx, tz, CommonData::TYPE::NONE);
				}
				break;

			//�I������
			case TILE_STATE::PREEND:
				if (tileSwapTime_[tz][tx] > TILE_END_TIME)
				{
					TileSwap(tx, tz, TILE_STATE::END);
					TileAttackPlayer(tx, tz, CommonData::TYPE::NONE);
				}
				break;

			//�I��
			case TILE_STATE::END:
				//�������Ȃ�
				break;

			default:
				//�����N���Ă��Ȃ��̂ŏ��������Ă���
				TileSwap(tx, tz, TILE_STATE::NORMAL);
				break;
			}
		}
	}

	for (int m = 0; m < static_cast<int>(TILE_TYPE::MAX); m++)
	{
		for (int tz = 0; tz < TILE_NUM; tz++)
		{
			for (int tx = 0; tx < TILE_NUM; tx++)
			{
				//3D���f��
				MV1SetScale(map_[tz][tx][m], scale_);
				MV1SetRotationXYZ(map_[tz][tx][m], rot_);
				MV1SetPosition(map_[tz][tx][m], pos_[tz][tx]);
			}
		}
	}
}

//�`�揈��
void Stage::Draw(void)
{
	//�w�i�摜�̕`��
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, backImage_, true);
	DrawRotaGraph(cloudPos_.x, cloudPos_.y, 1.0, 0.0, cloudImage_, true);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X, cloudPos_.y, 1.0, 0.0, cloudImage_, true);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X * 2, cloudPos_.y, 1.0, 0.0, cloudImage_, true);

	//�^�C���̕`��
	for (int tz = 0; tz < TILE_NUM; tz++)
	{
		for (int tx = 0; tx < TILE_NUM; tx++)
		{
			switch (tileState_[tz][tx])
			{
			case Stage::TILE_STATE::NORMAL:
				//�p�^�[�������݂ɒu��
				if ((tz + tx) % 2 == 0)
				{
					//�p�^�[��1
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_1)]);
				}
				else
				{
					//�p�^�[��2
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_2)]);
				}

				break;

			case Stage::TILE_STATE::ATTACK:
				switch (attackPlayer_[tz][tx])
				{
				case CommonData::TYPE::P1:
					//�p�^�[�������݂ɒu��
					if ((tz + tx) % 2 == 0)
					{
						//P1�p�^�[��1
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P1_PAT_1_1)]);
					}
					else
					{
						//P1�p�^�[��2
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P1_PAT_1_2)]);
					}
					break;

				case CommonData::TYPE::P2:
					//�p�^�[�������݂ɒu��
					if ((tz + tx) % 2 == 0)
					{
						//P2�p�^�[��1
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P2_PAT_1_1)]);
					}
					else
					{
						//P2�p�^�[��2
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P2_PAT_1_2)]);
					}
					break;

				case CommonData::TYPE::P3:
					//�p�^�[�������݂ɒu��
					if ((tz + tx) % 2 == 0)
					{
						//P3�p�^�[��1
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P3_PAT_1_1)]);
					}
					else
					{
						//P3�p�^�[��2
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P3_PAT_1_2)]);
					}
					break;

				case CommonData::TYPE::P4:
					//�p�^�[�������݂ɒu��
					if ((tz + tx) % 2 == 0)
					{
						//P4�p�^�[��1
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P4_PAT_1_1)]);
					}
					else
					{
						//P4�p�^�[��2
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P4_PAT_1_2)]);
					}
					break;
				}
				break;

			case TILE_STATE::PREEND:
				//�p�^�[�������݂ɒu��
				if ((tz + tx) % 2 == 0)
				{
					//END�p�^�[��1
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_1_END)]);
				}
				else
				{
					//END�p�^�[��1
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_2_END)]);
				}
				break;
			}
		}
	}
}

//�^�C���̏�ԑJ�ڏ���(ix:X�̃^�C���ԍ�, iz:Z�̃^�C���ԍ�, stateSwap:�ω���������)
void Stage::TileSwap(const int ix, const int iz, const Stage::TILE_STATE _stateSwap)
{
	//��ԑJ��
	tileState_[iz][ix] = _stateSwap;

	//��ԑJ�ڃJ�E���g�̏�����
	tileSwapTime_[iz][ix] = 0.0f;

	//�U�����ԃJ�E���g�̏�����
	atkCnt_[iz][ix] = 0.0f;
}

void Stage::TileAttackPlayer(const int ix, const int iz, const CommonData::TYPE _charaNum)
{
	attackPlayer_[iz][ix] = _charaNum;
}

const CommonData::TYPE Stage::GetTileAttackPlayer(const int ix, const int iz)
{
	return attackPlayer_[iz][ix];
}

//�U�����Ԃ�Ԃ�(ix:X�̃^�C���ԍ�, iz:Z�̃^�C���ԍ�)
const float Stage::GetAtkCnt(const int ix, const int iz)
{
	return atkCnt_[iz][ix];
}

//�U�����Ԃ�^����
void Stage::SetAtkCnt(const int ix, const int iz, const float _cnt)
{
	//�^�C�����U����Ԃ̂Ƃ��̂�
	if (tileState_[iz][ix] == TILE_STATE::ATTACK)
	{
		atkCnt_[iz][ix] = _cnt;
	}
}

//1���C���̃^�C������Ԃ��֐�
const int Stage::GetTileHNum(void)
{
	return TILE_HNUM;
}

//�^�C���̍��W��Ԃ��֐�(ix:X�̃^�C���ԍ�, iz:Z�̃^�C���ԍ�)
const VECTOR Stage::GetPos(const int ix, const int iz)
{
	return pos_[iz][ix];
}

//�^�C���̏�Ԃ�Ԃ��֐�
const Stage::TILE_STATE Stage::GetTileState(const int ix, const int iz)
{
	return tileState_[iz][ix];
}

const bool Stage::GetTileAdd(const int ix, const int iz)
{
	bool tileAdd[TILE_NUM][TILE_NUM];
	
	tileAdd[iz][ix] = true;
	
	return tileAdd[iz][ix];
}

void Stage::NarrowStage(void)
{
	//�^�C����������܂ł̎���
	if (endCnt_ < TILE_PREEND_TIME)
	{
		endCnt_++;
		return;
	}
	else
	{
		endCnt_ = 0;
	}

	//�X�e�[�W�k�����E
	if (stageNarrowRange_ >= STAGE_NARROW_RANGE)
	{
		return;
	}

	//�^�C���̏I������
	if (tileState_[endTileNumZ_][endTileNumX_] != TILE_STATE::PREEND
		&& tileState_[endTileNumZ_][endTileNumX_] != TILE_STATE::END)
	{
		//�^�C�����I��点��
		TileSwap(endTileNumX_, endTileNumZ_, TILE_STATE::PREEND);
	}

	//�X�e�[�W�̏k������
	if (sceneGame_->IsCollisionStageEnd(VGet(sceneGame_->TileAdd2PosX(endTileNumX_), 0.0f, sceneGame_->TileAdd2PosZ(endTileNumZ_)), endMoveDir_)
		|| sceneGame_->IsNextTileEnd(VGet(sceneGame_->TileAdd2PosX(endTileNumX_), 0.0f, sceneGame_->TileAdd2PosZ(endTileNumZ_)), endMoveDir_)
		)
	{
		switch (endMoveDir_)
		{
		case Utility::DIR_3D::FRONT:
			endMoveDir_ = Utility::DIR_3D::LEFT;
			break;

		case Utility::DIR_3D::RIGHT:
			endMoveDir_ = Utility::DIR_3D::FRONT;
			break;

		case Utility::DIR_3D::BACK:
			endMoveDir_ = Utility::DIR_3D::RIGHT;
			break;

		case Utility::DIR_3D::LEFT:
			endMoveDir_ = Utility::DIR_3D::BACK;
			break;
		}

		//�k���͈�
		if (endMoveDir_ == STAGE_NARROW_DIR)
		{
			stageNarrowRange_++;
		}
	}

	//�k�������ւ̏���
	switch (endMoveDir_)
	{
	case Utility::DIR_3D::FRONT:
		endTileNumZ_++;
		break;
	
	case Utility::DIR_3D::RIGHT:
		endTileNumX_++;
		break;
	
	case Utility::DIR_3D::BACK:
		endTileNumZ_--;
		break;
	
	case Utility::DIR_3D::LEFT:
		endTileNumX_--;
		break;
	}

}
