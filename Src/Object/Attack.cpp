#include<DxLib.h>
#include"../Manager/InputManager.h"
#include"../Utility/Utility.h"
#include"../Scene/SceneGame.h"
#include"Player.h"
#include"Attack.h"

//デフォルトコンストラクタ
Attack::Attack(void)
{
	sceneGame_ = nullptr;
	player_ = nullptr;
	images_ = 0;
	pos_ = Utility::VECTOR_ZERO;
	dir_ = Utility::DIR_3D::FRONT;
	isAlive_ = false;
	speed_ = 0.0f;
	atkCnt_ = 0.0f;
}

//デストラクタ
Attack::~Attack(void)
{

}

//初期化処理
bool Attack::Init(SceneGame* parent, Player* parPlayer, const VECTOR _pos, const Utility::DIR_3D _dir, const CommonData::TYPE _charaNum)
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
	case Utility::DIR_3D::FRONT:
		pos_.z += speed_;
		break;

	case Utility::DIR_3D::RIGHT:
		pos_.x += speed_;
		break;

	case Utility::DIR_3D::BACK:
		pos_.z -= speed_;
		break;

	case Utility::DIR_3D::LEFT:
		pos_.x -= speed_;
		break;

	default:
		break;
	}

	VECTOR loaclPos;
	switch (dir_)
	{
	case Utility::DIR_3D::FRONT:
		loaclPos = VSub(pos_, { 0.0f,0.0f,sceneGame_->GetTileSizeZ() / 2 });
		break;

	case Utility::DIR_3D::RIGHT:
		loaclPos = VSub(pos_, { sceneGame_->GetTileSizeX() / 2,0.0f,0.0f });
		break;

	case Utility::DIR_3D::BACK:
		loaclPos = VAdd(pos_, { 0.0f,0.0f,sceneGame_->GetTileSizeZ() / 2 });
		break;

	case Utility::DIR_3D::LEFT:
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
			color = Utility::COLOR_BLUE;
			break;

		case CommonData::TYPE::P2:
			color = Utility::COLOR_RED;
			break;

		case CommonData::TYPE::P3:
			color = Utility::COLOR_GREEN;
			break;

		case CommonData::TYPE::P4:
			color = Utility::COLOR_YELLOW;
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
const VECTOR Attack::GetPos(void)
{
	return pos_;
}

//攻撃の座標を決める
void Attack::SetPos(const VECTOR _setPos)
{
	pos_.x = _setPos.x;
	pos_.z = _setPos.z;
}

//攻撃の生存判定を返す
const bool Attack::GetIsAlive(void)
{
	return isAlive_;
}

//攻撃の生存判定を設定
void Attack::SetIsAlive(const bool _isAlive)
{
	isAlive_ = _isAlive;
}

//攻撃の方向を返す
const Utility::DIR_3D Attack::GetAttackDir(void)
{
	return dir_;
}

//攻撃の方向設定
void Attack::SetAttackDir(const Utility::DIR_3D _setDir)
{
	dir_ = _setDir;
}

const CommonData::TYPE Attack::GetCharaNum(void)
{
	return charaNum_;
}

//攻撃最中の経過時間を返す
const float Attack::GetAtkCnt(void)
{
	return atkCnt_;
}

void Attack::Destroy(void)
{
	pos_ = Utility::VECTOR_ZERO;
	isAlive_ = false;
}
