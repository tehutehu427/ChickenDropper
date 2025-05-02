#include<DxLib.h>
#include"../Application.h"
#include"../Utility/NodyUtility.h"
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
			pos_[tz][tx] = NodyUtility::VECTOR_ZERO;
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
	backImage_ = LoadGraph((Application::PATH_IMAGE + "TitleBack.png").c_str());
	cloudImage_ = LoadGraph((Application::PATH_IMAGE + "cloud.png").c_str());

	//�X�e�[�W�̃��f���ǂݍ���
	model_[static_cast<int>(TILE_TYPE::PAT_1_1)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-1.mv1").c_str());
	model_[static_cast<int>(TILE_TYPE::PAT_1_2)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-2.mv1").c_str());
	model_[static_cast<int>(TILE_TYPE::P1_PAT_1_1)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-1-Blue.mv1").c_str());
	model_[static_cast<int>(TILE_TYPE::P1_PAT_1_2)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-2-Blue.mv1").c_str());
	model_[static_cast<int>(TILE_TYPE::P2_PAT_1_1)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-1-Red.mv1").c_str());
	model_[static_cast<int>(TILE_TYPE::P2_PAT_1_2)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-2-Red.mv1").c_str());
	model_[static_cast<int>(TILE_TYPE::P3_PAT_1_1)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-1-Green.mv1").c_str());
	model_[static_cast<int>(TILE_TYPE::P3_PAT_1_2)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-2-Green.mv1").c_str());
	model_[static_cast<int>(TILE_TYPE::P4_PAT_1_1)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-1-Yellow.mv1").c_str());
	model_[static_cast<int>(TILE_TYPE::P4_PAT_1_2)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-2-Yellow.mv1").c_str());
	model_[static_cast<int>(TILE_TYPE::PAT_1_1_END)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-1-END.mv1").c_str());
	model_[static_cast<int>(TILE_TYPE::PAT_1_2_END)] = MV1LoadModel((Application::PATH_MODEL + "TilePat1-2-END.mv1").c_str());

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
				if (tz % 2 == 0 && tx % 2 == 0 || tz % 2 == 1 && tx % 2 == 1)
				{
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_1)]);
				}
				else
				{
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_2)]);
				}

				break;

			case Stage::TILE_STATE::ATTACK:
				switch (attackPlayer_[tz][tx])
				{
				case CommonData::TYPE::P1:
					//�p�^�[�������݂ɒu��
					if (tz % 2 == 0 && tx % 2 == 0 || tz % 2 == 1 && tx % 2 == 1)
					{
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P1_PAT_1_1)]);
					}
					else
					{
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P1_PAT_1_2)]);
					}
					break;

				case CommonData::TYPE::P2:
					//�p�^�[�������݂ɒu��
					if (tz % 2 == 0 && tx % 2 == 0 || tz % 2 == 1 && tx % 2 == 1)
					{
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P2_PAT_1_1)]);
					}
					else
					{
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P2_PAT_1_2)]);
					}
					break;

				case CommonData::TYPE::P3:
					//�p�^�[�������݂ɒu��
					if (tz % 2 == 0 && tx % 2 == 0 || tz % 2 == 1 && tx % 2 == 1)
					{
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P3_PAT_1_1)]);
					}
					else
					{
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P3_PAT_1_2)]);
					}
					break;

				case CommonData::TYPE::P4:
					//�p�^�[�������݂ɒu��
					if (tz % 2 == 0 && tx % 2 == 0 || tz % 2 == 1 && tx % 2 == 1)
					{
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P4_PAT_1_1)]);
					}
					else
					{
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P4_PAT_1_2)]);
					}
					break;
				}
				break;

			case TILE_STATE::PREEND:
				//�p�^�[�������݂ɒu��
				if (tz % 2 == 0 && tx % 2 == 0 || tz % 2 == 1 && tx % 2 == 1)
				{
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_1_END)]);
				}
				else
				{
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_2_END)]);
				}
				break;
			}
		}
	}
}

//�������
bool Stage::Release(void)
{
	DeleteGraph(backImage_);
	DeleteGraph(cloudImage_);

	for (int m = 0; m < static_cast<int>(TILE_TYPE::MAX); m++)
	{
		for (int tz = 0; tz < TILE_NUM; tz++)
		{
			for (int tx = 0; tx < TILE_NUM; tx++)
			{

				MV1DeleteModel(map_[tz][tx][m]);
			}
		}
	}

	return true;
}

//�^�C���̏�ԑJ�ڏ���(ix:X�̃^�C���ԍ�, iz:Z�̃^�C���ԍ�, stateSwap:�ω���������)
void Stage::TileSwap(int ix, int iz, Stage::TILE_STATE _stateSwap)		
{
	//��ԑJ��
	tileState_[iz][ix] = _stateSwap;

	//��ԑJ�ڃJ�E���g�̏�����
	tileSwapTime_[iz][ix] = 0.0f;

	//�U�����ԃJ�E���g�̏�����
	atkCnt_[iz][ix] = 0.0f;
}

void Stage::TileAttackPlayer(int ix, int iz, CommonData::TYPE _charaNum)
{
	attackPlayer_[iz][ix] = _charaNum;
}

CommonData::TYPE Stage::GetTileAttackPlayer(int ix, int iz)
{
	return attackPlayer_[iz][ix];
}

//�U�����Ԃ�Ԃ�(ix:X�̃^�C���ԍ�, iz:Z�̃^�C���ԍ�)
float Stage::GetAtkCnt(int ix, int iz)
{
	return atkCnt_[iz][ix];
}

//�U�����Ԃ�^����
void Stage::SetAtkCnt(int ix, int iz, float _cnt)
{
	//�^�C�����U����Ԃ̂Ƃ��̂�
	if (tileState_[iz][ix] == TILE_STATE::ATTACK)
	{
		atkCnt_[iz][ix] = _cnt;
	}
}

//1���C���̃^�C������Ԃ��֐�
int Stage::GetTileHNum(void)
{
	return TILE_HNUM;
}

//�^�C���̍��W��Ԃ��֐�(ix:X�̃^�C���ԍ�, iz:Z�̃^�C���ԍ�)
VECTOR Stage::GetPos(int ix, int iz)
{
	return pos_[iz][ix];
}

//�^�C���̏�Ԃ�Ԃ��֐�
Stage::TILE_STATE Stage::GetTileState(int ix, int iz)
{
	return tileState_[iz][ix];
}

bool Stage::GetTileAdd(int ix, int iz)
{
	bool tileAdd[TILE_NUM][TILE_NUM];
	
	tileAdd[iz][ix] = true;
	
	return tileAdd[iz][ix];
}

void Stage::NarrowStage(void)
{
	if (endCnt_ < TILE_PREEND_TIME)
	{
		endCnt_++;
		return;
	}
	else
	{
		endCnt_ = 0;
	}

	if (stageNarrowRange_ >= STAGE_NARROW_RANGE)
	{
		return;
	}

	if (tileState_[endTileNumZ_][endTileNumX_] != TILE_STATE::PREEND
		&& tileState_[endTileNumZ_][endTileNumX_] != TILE_STATE::END)
	{
		//�^�C�����I��点��
		TileSwap(endTileNumX_, endTileNumZ_, TILE_STATE::PREEND);
	}

	if (sceneGame_->IsCollisionStageEnd(VGet(sceneGame_->TileAdd2PosX(endTileNumX_), 0.0f, sceneGame_->TileAdd2PosZ(endTileNumZ_)), endMoveDir_)
		|| sceneGame_->IsNextTileEnd(VGet(sceneGame_->TileAdd2PosX(endTileNumX_), 0.0f, sceneGame_->TileAdd2PosZ(endTileNumZ_)), endMoveDir_)
		)
	{
		switch (endMoveDir_)
		{
		case NodyUtility::DIR_3D::FRONT:
			endMoveDir_ = NodyUtility::DIR_3D::LEFT;
			break;

		case NodyUtility::DIR_3D::RIGHT:
			endMoveDir_ = NodyUtility::DIR_3D::FRONT;
			break;

		case NodyUtility::DIR_3D::BACK:
			endMoveDir_ = NodyUtility::DIR_3D::RIGHT;
			break;

		case NodyUtility::DIR_3D::LEFT:
			endMoveDir_ = NodyUtility::DIR_3D::BACK;
			break;
		}

		if (endMoveDir_ == STAGE_NARROW_DIR)
		{
			stageNarrowRange_++;
		}
	}

	switch (endMoveDir_)
	{
	case NodyUtility::DIR_3D::FRONT:
		endTileNumZ_++;
		break;
	
	case NodyUtility::DIR_3D::RIGHT:
		endTileNumX_++;
		break;
	
	case NodyUtility::DIR_3D::BACK:
		endTileNumZ_--;
		break;
	
	case NodyUtility::DIR_3D::LEFT:
		endTileNumX_--;
		break;
	}

}
