#include<DxLib.h>
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Manager/Resource.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"
#include"../Common/CommonData.h"
#include"../Scene/SceneGame.h"
#include"Stage.h"

//デフォルトコンストラクタ
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

//デストラクタ
Stage::~Stage(void)
{

}

//初期化処理
bool Stage::Init(SceneGame* parent)
{
	//リソースマネージャー
	ResourceManager& res = ResourceManager::GetInstance();

	//親
	sceneGame_ = parent;

	//SEの初期化
	se_ = new SoundManager();
	se_->SEInit();

	scale_ = { 1.0f,1.0f,1.0f };
	rot_ = { 0.0f,0.0f,0.0f };

	//変数の初期化
	if (TILE_NUM % 2 == 1)		//タイル数が奇数時
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
	else		//タイルが偶数時
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
	
	//共通
	for (int tz = 0; tz < TILE_NUM; tz++)
	{
		for (int tx = 0; tx < TILE_NUM; tx++)
		{
			tileState_[tz][tx] = TILE_STATE::NORMAL;
			tileSwapTime_[tz][tx] = 0;
		}
	}

	//背景画像の読み込み
	backImage_ = res.Load(ResourceManager::SRC::GAME_BACK).handleId_;
	cloudImage_ = res.Load(ResourceManager::SRC::GAME_CLOUD).handleId_;

	//ステージのモデル読み込み
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

//更新処理
void Stage::Update(void)
{
	//雲の移動(左方向)
	cloudPos_.x--;
	if (cloudPos_.x < -Application::SCREEN_SIZE_X)
	{
		cloudPos_.x = 0.0f;
	}

	//ステージ縮小のフラグ
	if (sceneGame_->IsNarrowStage())
	{
		//ステージ縮小
		NarrowStage();
	}

	//タイルの状態遷移
	for (int tz = 0; tz < TILE_NUM; tz++)
	{
		for (int tx = 0; tx < TILE_NUM; tx++)
		{
			//カウント
			tileSwapTime_[tz][tx] ++;

			switch (tileState_[tz][tx])
			{
			//攻撃状態
			case TILE_STATE::ATTACK:
				//攻撃時間を取得してから
				if (atkCnt_[tz][tx] != 0.0f)
				{
					if (tileSwapTime_[tz][tx] > TILE_BREAK_TIME + atkCnt_[tz][tx])
					{
						//タイルが壊れる
						TileSwap(tx, tz, TILE_STATE::BROKEN);
						
						//TILEBREAKルールなら
						if (CommonData::GetInstance().GetRule() == CommonData::RULE::BREAK_TILE)
						{
							//壊したタイル数を増加
							sceneGame_->IncreaseBrokeTileNum(attackPlayer_[tz][tx]);
						}
					}
				}
				break;

			//破壊状態
			case TILE_STATE::BROKEN:
				if (tileSwapTime_[tz][tx] > TILE_REPAIR_TIME)
				{
					TileSwap(tx, tz, TILE_STATE::NORMAL);
					TileAttackPlayer(tx, tz, CommonData::TYPE::NONE);
				}
				break;

			//終了準備
			case TILE_STATE::PREEND:
				if (tileSwapTime_[tz][tx] > TILE_END_TIME)
				{
					TileSwap(tx, tz, TILE_STATE::END);
					TileAttackPlayer(tx, tz, CommonData::TYPE::NONE);
				}
				break;

			//終了
			case TILE_STATE::END:
				//何もしない
				break;

			default:
				//何も起きていないので初期化しておく
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
				//3Dモデル
				MV1SetScale(map_[tz][tx][m], scale_);
				MV1SetRotationXYZ(map_[tz][tx][m], rot_);
				MV1SetPosition(map_[tz][tx][m], pos_[tz][tx]);
			}
		}
	}
}

//描画処理
void Stage::Draw(void)
{
	//背景画像の描画
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, backImage_, true);
	DrawRotaGraph(cloudPos_.x, cloudPos_.y, 1.0, 0.0, cloudImage_, true);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X, cloudPos_.y, 1.0, 0.0, cloudImage_, true);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X * 2, cloudPos_.y, 1.0, 0.0, cloudImage_, true);

	//タイルの描画
	for (int tz = 0; tz < TILE_NUM; tz++)
	{
		for (int tx = 0; tx < TILE_NUM; tx++)
		{
			switch (tileState_[tz][tx])
			{
			case Stage::TILE_STATE::NORMAL:
				//パターンを交互に置く
				if ((tz + tx) % 2 == 0)
				{
					//パターン1
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_1)]);
				}
				else
				{
					//パターン2
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_2)]);
				}

				break;

			case Stage::TILE_STATE::ATTACK:
				switch (attackPlayer_[tz][tx])
				{
				case CommonData::TYPE::P1:
					//パターンを交互に置く
					if ((tz + tx) % 2 == 0)
					{
						//P1パターン1
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P1_PAT_1_1)]);
					}
					else
					{
						//P1パターン2
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P1_PAT_1_2)]);
					}
					break;

				case CommonData::TYPE::P2:
					//パターンを交互に置く
					if ((tz + tx) % 2 == 0)
					{
						//P2パターン1
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P2_PAT_1_1)]);
					}
					else
					{
						//P2パターン2
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P2_PAT_1_2)]);
					}
					break;

				case CommonData::TYPE::P3:
					//パターンを交互に置く
					if ((tz + tx) % 2 == 0)
					{
						//P3パターン1
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P3_PAT_1_1)]);
					}
					else
					{
						//P3パターン2
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P3_PAT_1_2)]);
					}
					break;

				case CommonData::TYPE::P4:
					//パターンを交互に置く
					if ((tz + tx) % 2 == 0)
					{
						//P4パターン1
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P4_PAT_1_1)]);
					}
					else
					{
						//P4パターン2
						MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::P4_PAT_1_2)]);
					}
					break;
				}
				break;

			case TILE_STATE::PREEND:
				//パターンを交互に置く
				if ((tz + tx) % 2 == 0)
				{
					//ENDパターン1
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_1_END)]);
				}
				else
				{
					//ENDパターン1
					MV1DrawModel(map_[tz][tx][static_cast<int>(TILE_TYPE::PAT_1_2_END)]);
				}
				break;
			}
		}
	}
}

//タイルの状態遷移処理(ix:Xのタイル番号, iz:Zのタイル番号, stateSwap:変化させる状態)
void Stage::TileSwap(const int ix, const int iz, const Stage::TILE_STATE _stateSwap)
{
	//状態遷移
	tileState_[iz][ix] = _stateSwap;

	//状態遷移カウントの初期化
	tileSwapTime_[iz][ix] = 0.0f;

	//攻撃時間カウントの初期化
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

//攻撃時間を返す(ix:Xのタイル番号, iz:Zのタイル番号)
const float Stage::GetAtkCnt(const int ix, const int iz)
{
	return atkCnt_[iz][ix];
}

//攻撃時間を与える
void Stage::SetAtkCnt(const int ix, const int iz, const float _cnt)
{
	//タイルが攻撃状態のときのみ
	if (tileState_[iz][ix] == TILE_STATE::ATTACK)
	{
		atkCnt_[iz][ix] = _cnt;
	}
}

//1ラインのタイル数を返す関数
const int Stage::GetTileHNum(void)
{
	return TILE_HNUM;
}

//タイルの座標を返す関数(ix:Xのタイル番号, iz:Zのタイル番号)
const VECTOR Stage::GetPos(const int ix, const int iz)
{
	return pos_[iz][ix];
}

//タイルの状態を返す関数
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
	//タイルが消えるまでの時間
	if (endCnt_ < TILE_PREEND_TIME)
	{
		endCnt_++;
		return;
	}
	else
	{
		endCnt_ = 0;
	}

	//ステージ縮小限界
	if (stageNarrowRange_ >= STAGE_NARROW_RANGE)
	{
		return;
	}

	//タイルの終了処理
	if (tileState_[endTileNumZ_][endTileNumX_] != TILE_STATE::PREEND
		&& tileState_[endTileNumZ_][endTileNumX_] != TILE_STATE::END)
	{
		//タイルを終わらせる
		TileSwap(endTileNumX_, endTileNumZ_, TILE_STATE::PREEND);
	}

	//ステージの縮小方向
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

		//縮小範囲
		if (endMoveDir_ == STAGE_NARROW_DIR)
		{
			stageNarrowRange_++;
		}
	}

	//縮小方向への処理
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
