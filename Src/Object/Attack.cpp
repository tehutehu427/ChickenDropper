#include<DxLib.h>
#include"../Manager/InputManager.h"
#include"../Utility/NodyUtility.h"
#include"../Scene/SceneGame.h"
#include"Player.h"
#include"Attack.h"

//デフォルトコンストラクタ
Attack::Attack(void)
{
	sceneGame_ = nullptr;
	player_ = nullptr;
	images_ = 0;
	pos_ = NodyUtility::VECTOR_ZERO;
	dir_ = NodyUtility::DIR_3D::FRONT;
	isAlive_ = false;
	speed_ = 0.0f;
	atkCnt_ = 0.0f;
}

//デストラクタ
Attack::~Attack(void)
{

}

//初期化処理
bool Attack::Init(SceneGame* parent, Player* parPlayer, VECTOR _pos, NodyUtility::DIR_3D _dir, CommonData::TYPE _charaNum)
{
	sceneGame_ = parent;
	player_ = parPlayer;

	//変数の初期化
	pos_ = _pos;
	dir_ = _dir;
	charaNum_ = _charaNum;
	isAlive_ = true;
	speed_ = ATK_SPEED;
	atkCnt_ = 0.0f;

	return true;
}

//更新処理
void Attack::Update(void)
{	
	//攻撃が存在しないときは処理を止める
	if (!isAlive_)
	{
		atkCnt_ = 0.0f;
		return;
	}

	//攻撃最中の経過時間カウント
	atkCnt_++;

	//攻撃の移動処理
	//---------------------------------------
	switch (dir_)
	{
	case NodyUtility::DIR_3D::FRONT:
		pos_.z += speed_;
		break;

	case NodyUtility::DIR_3D::RIGHT:
		pos_.x += speed_;
		break;

	case NodyUtility::DIR_3D::BACK:
		pos_.z -= speed_;
		break;

	case NodyUtility::DIR_3D::LEFT:
		pos_.x -= speed_;
		break;

	default:
		break;
	}

	VECTOR loaclPos;
	switch (dir_)
	{
	case NodyUtility::DIR_3D::FRONT:
		loaclPos = VSub(pos_, { 0.0f,0.0f,sceneGame_->GetTileSizeZ() / 2 });
		break;

	case NodyUtility::DIR_3D::RIGHT:
		loaclPos = VSub(pos_, { sceneGame_->GetTileSizeX() / 2,0.0f,0.0f });
		break;

	case NodyUtility::DIR_3D::BACK:
		loaclPos = VAdd(pos_, { 0.0f,0.0f,sceneGame_->GetTileSizeZ() / 2 });
		break;

	case NodyUtility::DIR_3D::LEFT:
		loaclPos = VAdd(pos_, { sceneGame_->GetTileSizeX() / 2,0.0f,0.0f });
		break;
	}

	//攻撃の移動制限
	//---------------------------------------
	if (sceneGame_->IsCollisionStageEnd(pos_,dir_)
		|| sceneGame_->IsNextTileEnd(loaclPos, dir_)
		)
	{
		for (int tz = 0; tz < sceneGame_->GetTileNum(); tz++)
		{
			for (int tx = 0; tx < sceneGame_->GetTileNum(); tx++)
			{
				//攻撃が終わって
				//攻撃状態になったタイルに攻撃時間がセットされてなかったら
				if (sceneGame_->GetAtkCnt(tx, tz) == 0.0f)
				{
					//攻撃状態になったタイルに攻撃時間をセットする
					sceneGame_->SetAtkCnt(tx, tz, atkCnt_);
				}
			}
		}

		//攻撃を消す
		Destroy();
	}
}

//描画処理
void Attack::Draw(void)
{
	unsigned int color;

	//攻撃の描画
	if (isAlive_)
	{
		switch (charaNum_)
		{
		case CommonData::TYPE::P1:
			color = NodyUtility::COLOR_BLUE;
			break;

		case CommonData::TYPE::P2:
			color = NodyUtility::COLOR_RED;
			break;

		case CommonData::TYPE::P3:
			color = NodyUtility::COLOR_GREEN;
			break;

		case CommonData::TYPE::P4:
			color = NodyUtility::COLOR_YELLOW;
			break;
		}
	}
}

//解放処理
bool Attack::Release(void)
{
	return true;
}

//攻撃の現在座標を返す
VECTOR Attack::GetPos(void)
{
	return pos_;
}

//攻撃の座標を決める
void Attack::SetPos(VECTOR _setPos)
{
	pos_.x = _setPos.x;
	pos_.z = _setPos.z;
}

//攻撃の生存判定を返す
bool Attack::GetIsAlive(void)
{
	return isAlive_;
}

//攻撃の生存判定を設定
void Attack::SetIsAlive(bool _isAlive)
{
	isAlive_ = _isAlive;
}

//攻撃の方向を返す
NodyUtility::DIR_3D Attack::GetAttackDir(void)
{
	return dir_;
}

//攻撃の方向設定
void Attack::SetAttackDir(NodyUtility::DIR_3D _setDir)
{
	dir_ = _setDir;
}

CommonData::TYPE Attack::GetCharaNum(void)
{
	return charaNum_;
}

//攻撃最中の経過時間を返す
float Attack::GetAtkCnt(void)
{
	return atkCnt_;
}

void Attack::Destroy(void)
{
	pos_ = NodyUtility::VECTOR_ZERO;
	isAlive_ = false;
}
