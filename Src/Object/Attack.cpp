#include<DxLib.h>
#include"../Manager/InputManager.h"
#include"../Utility/Utility.h"
#include"../Scene/SceneGame.h"
#include"Player.h"
#include"Attack.h"

//�f�t�H���g�R���X�g���N�^
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

//�f�X�g���N�^
Attack::~Attack(void)
{

}

//����������
bool Attack::Init(SceneGame* parent, Player* parPlayer, const VECTOR _pos, const Utility::DIR_3D _dir, const CommonData::TYPE _charaNum)
{
	sceneGame_ = parent;
	player_ = parPlayer;

	//�ϐ��̏�����
	pos_ = _pos;
	dir_ = _dir;
	charaNum_ = _charaNum;
	isAlive_ = true;
	speed_ = ATK_SPEED;
	atkCnt_ = 0.0f;

	return true;
}

//�X�V����
void Attack::Update(void)
{	
	//�U�������݂��Ȃ��Ƃ��͏������~�߂�
	if (!isAlive_)
	{
		atkCnt_ = 0.0f;
		return;
	}

	//�U���Œ��̌o�ߎ��ԃJ�E���g
	atkCnt_++;

	//�U���̈ړ�����
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

	//�U���̈ړ�����
	//---------------------------------------
	if (sceneGame_->IsCollisionStageEnd(pos_,dir_)
		|| sceneGame_->IsNextTileEnd(loaclPos, dir_)
		)
	{
		for (int tz = 0; tz < sceneGame_->GetTileNum(); tz++)
		{
			for (int tx = 0; tx < sceneGame_->GetTileNum(); tx++)
			{
				//�U�����I�����
				//�U����ԂɂȂ����^�C���ɍU�����Ԃ��Z�b�g����ĂȂ�������
				if (sceneGame_->GetAtkCnt(tx, tz) == 0.0f)
				{
					//�U����ԂɂȂ����^�C���ɍU�����Ԃ��Z�b�g����
					sceneGame_->SetAtkCnt(tx, tz, atkCnt_);
				}
			}
		}

		//�U��������
		Destroy();
	}
}

//�`�揈��
void Attack::Draw(void)
{
	unsigned int color;

	//�U���̕`��
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

//�������
bool Attack::Release(void)
{
	return true;
}

//�U���̌��ݍ��W��Ԃ�
const VECTOR Attack::GetPos(void)
{
	return pos_;
}

//�U���̍��W�����߂�
void Attack::SetPos(const VECTOR _setPos)
{
	pos_.x = _setPos.x;
	pos_.z = _setPos.z;
}

//�U���̐��������Ԃ�
const bool Attack::GetIsAlive(void)
{
	return isAlive_;
}

//�U���̐��������ݒ�
void Attack::SetIsAlive(const bool _isAlive)
{
	isAlive_ = _isAlive;
}

//�U���̕�����Ԃ�
const Utility::DIR_3D Attack::GetAttackDir(void)
{
	return dir_;
}

//�U���̕����ݒ�
void Attack::SetAttackDir(const Utility::DIR_3D _setDir)
{
	dir_ = _setDir;
}

const CommonData::TYPE Attack::GetCharaNum(void)
{
	return charaNum_;
}

//�U���Œ��̌o�ߎ��Ԃ�Ԃ�
const float Attack::GetAtkCnt(void)
{
	return atkCnt_;
}

void Attack::Destroy(void)
{
	pos_ = Utility::VECTOR_ZERO;
	isAlive_ = false;
}
