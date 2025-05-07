#include<DxLib.h>
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Manager/Resource.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/EffectManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/SceneManager.h"
#include"../Scene/SceneGame.h"
#include"Attack.h"
#include"Player.h"

//�f�t�H���g�R���X�g���N�^
Player::Player(void)
{
	sceneGame_ = nullptr;
	effect_ = nullptr;
	se_ = nullptr;
	model_ = 0;
	pos_ = Utility::VECTOR_ZERO;
	rot_ = Utility::VECTOR_ZERO;
	scale_ = Utility::VECTOR_ONE;
	dir_ = Utility::DIR_3D::BACK;
	state_ = STATE::NORMAL;
	speed_ = 0.0f;
	killScore_ = 0;
	life_ = 0;
	deadTime_ = 0;
	brokeTileNum_ = 0;
	rank_ = 0;
	isWalk_ = false;
	atkCoolDown_ = 0.0f;
	attackedPlayer_ = CommonData::TYPE::NONE;
	animNum_ = ANIM_NUM::IDLE;
	animAttachNo_ = 0;
	animTotalTime_ = 0.0f;
	stepAnim_ = 0;
	invincible_ = 0.0f;
	charaJudge_ = CHARA_JUDGE::MAX;
	charaNum_ = CommonData::TYPE::NONE;
	numImage_ = 0;
	numViewTime_ = 0.0f;
	padNum_ = 0;
	CPUMoveInterVal_ = 0.0f;
	randInterval_ = 0.0f;
	isEndMoveInterval_ = false;
	isEndAtkCoolDownCPU_ = false;
	isMoveFront_ = true;
	isMoveRight_ = true;
	isMoveBack_ = true;
	isMoveLeft_ = true;
	difficulty_ = CommonData::DIFFICULTY::EASY;
	speedAnim_ = 0.0f;
}

//�f�X�g���N�^
Player::~Player(void)
{

}

//����������
bool Player::Init(SceneGame* parent, const VECTOR _pos, const CommonData::TYPE _charaNum)
{
	//�C���X�^���X�擾
	ResourceManager& res = ResourceManager::GetInstance();

	//�y�A�����g
	sceneGame_ = parent;

	//�G�t�F�N�g
	effect_ = new EffectManager();
	effect_->EffectInit();

	//�T�E���h
	//--------------------
	se_ = new SoundManager();
	se_->SEInit();

	//�T�E���h�̃��[�h
	se_->SELoad(SoundManager::SE_TYPE::ATTACK);

	//�ϐ��̏�����	
	//------------------------------------------	
	pos_ = _pos;
	scale_ = Utility::VECTOR_ONE;
	rot_ = Utility::VECTOR_ZERO;
	state_ = STATE::NORMAL;
	speed_ = SPEED_NORMAL;
	killScore_ = 0;
	life_ = MAX_LIFE;
	attackedPlayer_ = CommonData::TYPE::NONE;
	deadTime_ = 0;
	brokeTileNum_ = 0;
	rank_ = 0;
	isWalk_ = false;
	atkCoolDown_ = 0.0f;
	invincible_ = INVINCIBLE;
	charaNum_ = _charaNum;
	numViewTime_ = 0.0f;
	CPUMoveInterVal_ = 0.0f;
	randInterval_ = false;
	isMoveFront_ = true;
	isMoveRight_ = true;
	isMoveBack_ = true;
	isMoveLeft_ = true;


	//�L�����ԍ����f
	switch (charaNum_)
	{
	case CommonData::TYPE::P1:
		padNum_ = DX_INPUT_PAD1;

		numImage_ = LoadGraph((Application::PATH_IMAGE + "P1.png").c_str());
		
		model_ = MV1DuplicateModel(MV1LoadModel((Application::PATH_MODEL + "chickenBlue.mv1").c_str()));
		break;

	case CommonData::TYPE::P2:
		if (charaJudge_ == CHARA_JUDGE::PLAYER)
		{
			padNum_ = DX_INPUT_PAD2;
		}

		numImage_ = LoadGraph((Application::PATH_IMAGE + "P2.png").c_str());

		model_ = MV1DuplicateModel(MV1LoadModel((Application::PATH_MODEL + "chickenRed.mv1").c_str()));
		break;

	case CommonData::TYPE::P3:
		if (charaJudge_ == CHARA_JUDGE::PLAYER)
		{
			padNum_ = DX_INPUT_PAD3;
		}

		numImage_ = LoadGraph((Application::PATH_IMAGE + "P3.png").c_str());

		model_ = MV1DuplicateModel(MV1LoadModel((Application::PATH_MODEL + "chickenGreen.mv1").c_str()));
		break;

	case CommonData::TYPE::P4:
		if (charaJudge_ == CHARA_JUDGE::PLAYER)
		{
			padNum_ = DX_INPUT_PAD4;
		}

		numImage_ = LoadGraph((Application::PATH_IMAGE + "P4.png").c_str());

		model_ = MV1DuplicateModel(MV1LoadModel((Application::PATH_MODEL + "chickenYellow.mv1").c_str()));
		break;
	}

	//�J�n�A�j���[�V����
	ChangeAnim(ANIM_NUM::IDLE);
	speedAnim_ = SPEED_ANIM;


	//���f���̈ʒu�A�傫���A�p�x
	MV1SetPosition(model_, pos_);
	MV1SetScale(model_, scale_);
	MV1SetRotationXYZ(model_, rot_);

	return true;
}

//�X�V����
void Player::Update(void)
{
	//���ꂽ�珈�����~�߂�
	if (state_ == STATE::DEAD)
	{
		return;
	}

	if (numViewTime_ < NUM_VIEW_TIME)
	{
		//�ԍ��̕\���J�E���^
		numViewTime_++;
	}

	//���G���Ԑ���
	invincible_++;
	
	//��]����
	ProcessRot();

	//�L�������Ƃ̍X�V
	switch (charaJudge_)
	{
	case Player::CHARA_JUDGE::PLAYER:
		UpdatePlayer();
		break;
	
	case Player::CHARA_JUDGE::CPU:
		UpdateCPU();
		break;
	}

	//�A�j���[�V�����̍X�V����
	UpdateAnimation();

	//�A�j���[�V����
	Animation();

	//��ԏ���
	UpdateState();
}

void Player::UpdateState(void)
{
	switch (state_)
	{
	case Player::STATE::NORMAL:
		break;

	case Player::STATE::ATTACK:
		if (stepAnim_ >= animTotalTime_)
		{
			ChangeState(STATE::NORMAL);
		}
		break;

	case Player::STATE::DEAD:
		break;
	}
}

//�X�V����(�v���C���[)
void Player::UpdatePlayer(void)
{
	//�ړ�����
	MovePlayer();

	//�U���̐���
	ProcessAttackPlayer();
}

//�X�V����(CPU)
void Player::UpdateCPU(void)
{
	//�ړ�����
	MoveCPU();

	//�U���̐���
	ProcessAttackCPU();
}

//�`�揈��
void Player::Draw(void)
{
	if (state_ == STATE::DEAD)
	{
		return;
	}
	if (invincible_ >= INVINCIBLE || static_cast<int>(invincible_) % 3 == 0)
	{
		//�v���C���[�̕`��
		MV1DrawModel(model_);
	}

	if (numViewTime_ < NUM_VIEW_TIME)
	{
		DrawBillboard3D({ pos_.x,pos_.y + NUM_DIS ,pos_.z }, 0.5f, 0.5f, NUM_SIZE, 0.0f, numImage_, true);
	}
}

//�������
bool Player::Release(void)
{
	MV1DeleteModel(model_);

	//�U���̉��
	for (auto attack : attack_)
	{
		attack->Release();
		delete attack;
		attack = nullptr;
	}

	//�G�t�F�N�g�̉��
	effect_->EffectEnd();
	delete effect_;
	effect_ = nullptr;

	//SE�̉��
	se_->SoundRelease();
	delete se_;
	se_ = nullptr;

	return true;
}

//�ړ�����
void Player::MovePlayer(void)
{
	auto& ins = InputManager::GetInstance();

	//�ړ�����
	//----------------------------------------
	//�����Ă��Ȃ��Ƃ� ���� �U�����ł͂Ȃ��@���@�������ł͂Ȃ�
	if (!isWalk_ && state_ != STATE::ATTACK && animNum_ != ANIM_NUM::ATTACK && animNum_ != ANIM_NUM::FALL)
	{
		//�ړ������ݒ�
		//-------------------------------------------------
		//�����]���p�L�[
		auto keepButtonType = PAD_INPUT_6;	//R�L�[
		auto keepKeyType = KEY_INPUT_X;		//X�L�[

		//�L�[�{�[�h�̎��̓��͍��킹
		bool keyboardKeep = (GetJoypadNum() <= 0 && (charaNum_ == CommonData::TYPE::P1 && ins.IsKeyDown(keepKeyType)));

		//�O����

		//R�L�[�������Ȃ��炾�ƕ����]��
		if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_UP) && (ins.IsJoypadKeyDown(padNum_, keepButtonType) || keyboardKeep))
		{
			//�����ύX�̂�
			dir_ = Utility::DIR_3D::FRONT;
		}
		else if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_UP) && !isWalk_)
		{
			dir_ = Utility::DIR_3D::FRONT;

			//�ړ���̃^�C�������Ă��Ȃ��@���@�ړ���Ƀv���C���[�����Ȃ��@���@�[�ł͂Ȃ�
			if (!sceneGame_->IsNextTileBroken(pos_, dir_) 
				&& !sceneGame_->IsNextTileEnd(pos_, dir_) 
				&& !sceneGame_->HitP2P(pos_, dir_, charaNum_) 
				&& !sceneGame_->IsCollisionStageEnd(pos_, dir_))
			{
				pos_.z += speed_;
				isWalk_ = true;

				//�������[�V�����J�n
				ChangeAnim(ANIM_NUM::WALK);
			}
		}

		//�E����

		//R�L�[�������Ȃ��炾�ƕ����]��
		if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_RIGHT) && !isWalk_ && (ins.IsJoypadKeyDown(padNum_, keepButtonType) || keyboardKeep))
		{
			//�����ύX�̂�
			dir_ = Utility::DIR_3D::RIGHT;
		}
		else if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_RIGHT) && !isWalk_)
		{
			dir_ = Utility::DIR_3D::RIGHT;
			//�ړ���̃^�C�������Ă��Ȃ��@���@�ړ���Ƀv���C���[�����Ȃ��@���@�[�ł͂Ȃ�
			if (!sceneGame_->IsNextTileBroken(pos_, dir_) 
				&& !sceneGame_->IsNextTileEnd(pos_, dir_)
				&& !sceneGame_->HitP2P(pos_, dir_, charaNum_) 
				&& !sceneGame_->IsCollisionStageEnd(pos_, dir_))
			{
				pos_.x += speed_;
				isWalk_ = true;

				//�������[�V�����J�n
				ChangeAnim(ANIM_NUM::WALK);
			}
		}

		//�����
		
		//R�L�[�������Ȃ��炾�ƕ����]��
		if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_DOWN) && !isWalk_ && (ins.IsJoypadKeyDown(padNum_, keepButtonType) || keyboardKeep))
		{
			dir_ = Utility::DIR_3D::BACK;
		}
		else if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_DOWN) && !isWalk_)
		{
			dir_ = Utility::DIR_3D::BACK;
			//�ړ���̃^�C�������Ă��Ȃ��@���@�ړ���Ƀv���C���[�����Ȃ��@���@�[�ł͂Ȃ�
			if (!sceneGame_->IsNextTileBroken(pos_, dir_) 
				&& !sceneGame_->IsNextTileEnd(pos_, dir_) 
				&& !sceneGame_->HitP2P(pos_, dir_, charaNum_) 
				&& !sceneGame_->IsCollisionStageEnd(pos_, dir_))
			{
				pos_.z -= speed_;
				isWalk_ = true;

				//�������[�V�����J�n
				ChangeAnim(ANIM_NUM::WALK);
			}
		}
		
		//������

		//R�L�[�������Ȃ��炾�ƕ����]��
		if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_LEFT) && !isWalk_ && (ins.IsJoypadKeyDown(padNum_, keepButtonType) || keyboardKeep))
		{
			dir_ = Utility::DIR_3D::LEFT;
		}
		else if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_LEFT) && !isWalk_)
		{
			dir_ = Utility::DIR_3D::LEFT;
			//�ړ���̃^�C�������Ă��Ȃ��@���@�ړ���Ƀv���C���[�����Ȃ��@���@�[�ł͂Ȃ�
			if (!sceneGame_->IsNextTileBroken(pos_, dir_) 
				&& !sceneGame_->IsNextTileEnd(pos_, dir_) 
				&& !sceneGame_->HitP2P(pos_, dir_, charaNum_) 
				&& !sceneGame_->IsCollisionStageEnd(pos_, dir_))
			{
				pos_.x -= speed_;
				isWalk_ = true;

				//�������[�V�����J�n
				ChangeAnim(ANIM_NUM::WALK);
			}
		}		
	}
	else						//�����Ă���
	{
		//�^�C���̐^�񒆂ɂ��邩
		if (!(static_cast<int>(pos_.x) % static_cast<int>(sceneGame_->GetTileSizeX()) == 0
			&& static_cast<int>(pos_.z) % static_cast<int>(sceneGame_->GetTileSizeZ()) == 0))		//�^�񒆂ɂ��Ȃ�
		{
			//�^�񒆂ɂ��Ȃ��Ԉړ�����
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
		}
		else	//�^�񒆂ɂ���
		{
			//�~�܂�
			isWalk_ = false;
		}
	}
}

void Player::MoveCPU(void)
{
	//�����Ă��Ȃ��Ƃ� ���� �U�����ł͂Ȃ��@���@�������ł͂Ȃ�
	if (!isWalk_ && state_ != STATE::ATTACK && animNum_ != ANIM_NUM::ATTACK && animNum_ != ANIM_NUM::FALL)
	{
		//�����̗����Ă���^�C�����U����� ���� �I���������
		if (sceneGame_->IsThisTileAttack(pos_) || sceneGame_->IsThisTilePreEnd(pos_))
		{			
			//�����ɓ���
			RunMoveInterval();
		}
		else
		{
			//�Ԋu�����炷��ړ��ɂ����
			if (!randInterval_)
			{
				randInterval_ = true;

				switch (difficulty_)
				{
				case CommonData::DIFFICULTY::EASY:
					CPUMoveInterVal_ += CPU_MOVE_INTERVAL_RAND_EASY - GetRand(CPU_MOVE_INTERVAL_RAND_EASY * 2);
					break;

				case CommonData::DIFFICULTY::NORMAL:
					CPUMoveInterVal_ += CPU_MOVE_INTERVAL_RAND_NORMAL - GetRand(CPU_MOVE_INTERVAL_RAND_NORMAL * 2);
					break;

				case CommonData::DIFFICULTY::HARD:
					CPUMoveInterVal_ += CPU_MOVE_INTERVAL_RAND_HARD - GetRand(CPU_MOVE_INTERVAL_RAND_HARD * 2);
					break;
				}
			}
		}

		//�G�̈ړ��Ԋu�J�E���^
		CPUMoveInterVal_++;

		if (difficulty_ == CommonData::DIFFICULTY::EASY && CPUMoveInterVal_ > CPU_MOVE_INTERVAL_EASY
		|| difficulty_ == CommonData::DIFFICULTY::NORMAL && CPUMoveInterVal_ > CPU_MOVE_INTERVAL_NORMAL
		|| difficulty_ == CommonData::DIFFICULTY::HARD && CPUMoveInterVal_ > CPU_MOVE_INTERVAL_HARD
			)
		{
			//����v���C���[�Ƃ̏Փ˔���Ǝ����̗����Ă���^�C�����U����Ԃ��̔���
			if (sceneGame_->HitP2P(pos_, sceneGame_->AimPlayer(pos_, charaNum_), charaNum_) 
				&& !sceneGame_->IsThisTileAttack(pos_)
				&& !sceneGame_->IsThisTilePreEnd(pos_))
			{
				//�����ύX�̂�
				dir_ = sceneGame_->AimPlayer(pos_, charaNum_);
			}
			else
			{
				//CPU�̈ړ����f(��ԋ߂��v���C���[��_��)
				dir_ = CPUMoveChack(sceneGame_->AimPlayer(pos_, charaNum_));
				
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
				}

				//�������Ƃ��\�Ȃ�
				if (dir_ != Utility::DIR_3D::MAX)
				{
					//����������
					isWalk_ = true;

					//�������[�V�����J�n
					ChangeAnim(ANIM_NUM::WALK);
				}
			}

			//�����_���ŊԊu���J����
			randInterval_ = false;

			//�U���������
			isEndMoveInterval_ = false;

			//�ړ��Ԋu�J�E���^������
			CPUMoveInterVal_ = 0.0f;
		}
	}
	else						//�����Ă���
	{
		//�^�C���̐^�񒆂ɂ��邩
		if (!(static_cast<int>(pos_.x) % static_cast<int>(sceneGame_->GetTileSizeX()) == 0
			&& static_cast<int>(pos_.z) % static_cast<int>(sceneGame_->GetTileSizeZ()) == 0))		//�^�񒆂ɂ��Ȃ�
		{
			//�^�񒆂ɂ��Ȃ��Ԉړ�����
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
		}
		else	//�^�񒆂ɂ���
		{
			//�~�܂�
			isWalk_ = false;
		}
	}
}

void Player::ProcessRot(void)
{
	switch (dir_)
	{
	case Utility::DIR_3D::FRONT:
		rot_.y = DX_PI_F / 180.0f * (0.0f + 180.0f);
		break;

	case Utility::DIR_3D::RIGHT:
		rot_.y = DX_PI_F / 180.0f * (90.0f + 180.0f);
		break;

	case Utility::DIR_3D::BACK:
		rot_.y = DX_PI_F / 180.0f * (180.0f + 180.0f);
		break;

	case Utility::DIR_3D::LEFT:
		rot_.y = DX_PI_F / 180.0f * (270.0f + 180.0f);
		break;
	}
}

void Player::PreDamage(const CommonData::TYPE _charaNum)
{
	//�U�����Ă����v���C���[��ۑ�
	attackedPlayer_ = _charaNum;

	//�����A�j���[�V��������
	ChangeAnim(ANIM_NUM::FALL);
}

void Player::Damage(const int _damage)
{
	//���[��
	auto rule = CommonData::GetInstance().GetRule();

	switch (rule)
	{
	case CommonData::RULE::TIME:
		//���_����
		killScore_ -= _damage;

		//���_��0�����ɂȂ�Ȃ��悤�ɂ���
		if (killScore_ <= 0)
		{
			killScore_ = 0;
		}
		break;
	
	case CommonData::RULE::LIFE:
		//�_���[�W��^����
		life_ -= _damage;

		//���C�t���O�ȉ��ɂȂ����琶�������false�ɂ���
		if (life_ <= 0)
		{
			//���ꂽ���Ԃ��Ƃ��Ă���
			deadTime_ = GetNowCount();

			ChangeState(STATE::DEAD);
			return;
		}
		break;
	
	case CommonData::RULE::BREAK_TILE:
		//�^�C���̔j�󐔂̒l�������ŉ�����
		brokeTileNum_ -= brokeTileNum_ / _damage;
		break;
	}

	//�����G�t�F�N�g
	effect_->EffectPlay(EffectManager::EFF_TYPE::REVIVAL, pos_, { 8.0f,8.0f,8.0f }, Utility::VECTOR_ZERO);

	//�v���C���[�ԍ��̍ĕ\��
	numViewTime_ = 0.0f;
}

void Player::TakeScore(const int _score)
{
	killScore_ += _score;
}

const int Player::GetScore(void)
{
	return killScore_;
}

//�U���̐���(�v���C���[)
void Player::ProcessAttackPlayer(void)
{
	//�v���C���[�̍U������
	//-------------------------------
	
	//�U���A�j���[�V�������I�������
	if (animTotalTime_ <= stepAnim_ && animNum_ == ANIM_NUM::ATTACK)
	{
		//�U���̐���
		GenerateAttack();

		StartJoypadVibration(padNum_, 100, 250);
	}

	//�v���C���[�������Ă���@���@�v���C���[�������Ă��Ȃ��@���@�U�����ł͂Ȃ�
	if (state_ != STATE::DEAD 
		&& !isWalk_ 
		&& state_ != STATE::ATTACK
		&& animNum_ != ANIM_NUM::ATTACK
		&& animNum_ != ANIM_NUM::FALL
		)
	{
		if (InputManager::GetInstance().IsJoypadKeyPush(padNum_, PAD_INPUT_A) == true)
		{
			//�U�����[�V�����J�n
			ChangeAnim(ANIM_NUM::ATTACK);

			//�U����Ԃɂ���
			ChangeState(STATE::ATTACK);
		}
	}
}

//�U���̐���(CPU)
void Player::ProcessAttackCPU(void)
{
	//�N�[���^�C���J�E���^
	atkCoolDown_++;

	//�U���A�j���[�V�������I�������
	if (animTotalTime_ <= stepAnim_ && animNum_ == ANIM_NUM::ATTACK)
	{
		//�U���̐���
		GenerateAttack();

		//�N�[���^�C�����Z�b�g
		atkCoolDown_ = 0;
	}

	//�U������
	//-------------------------------
	/*�v���C���[�������Ă���@���@
	�v���C���[�������Ă��Ȃ��@���@
	�U�����ł͂Ȃ�(�A�j���[�V����)�@���@
	�Ə�������ɍ����Ă���@���@
	�U���̃N�[���^�C�����I����Ă���@����
	�����A�j���[�V�������ł͂Ȃ�*/
	if (state_ != STATE::DEAD 
		&& !isWalk_ 
		&& state_ != STATE::ATTACK 
		&& animNum_ != ANIM_NUM::ATTACK 
		&& sceneGame_->AimPlayerAttack(pos_, charaNum_, dir_)
		&& ((difficulty_ == CommonData::DIFFICULTY::EASY && atkCoolDown_ > ATK_COOLDOWN_CPU_EASY)
		|| (difficulty_ == CommonData::DIFFICULTY::NORMAL && atkCoolDown_ > ATK_COOLDOWN_CPU_NORMAL)
		|| (difficulty_ == CommonData::DIFFICULTY::HARD && atkCoolDown_ > ATK_COOLDOWN_CPU_HARD))
		&& animNum_ != ANIM_NUM::FALL)
	{
		//�U�����[�V�����J�n
		ChangeAnim(ANIM_NUM::ATTACK);

		//�U����Ԃɂ���
		ChangeState(STATE::ATTACK);

		//�N�[���_�E���̏�����
		atkCoolDown_ = 0;
	}
}

void Player::GenerateAttack(void)
{
	//�L���ȍU�����擾
	Attack* attack = GetValidAttack();

	//�X�e�[�W���O�ł͍U���ł��Ȃ��悤�ɂ���
	if (attack->GetPos().z > -sceneGame_->GetStageHSizeZ() - (sceneGame_->GetTileSizeZ() / 2)		//�O�[�̈ړ�����
		&& attack->GetPos().x < sceneGame_->GetStageHSizeX() + (sceneGame_->GetTileSizeX() / 2)		//�E�[�̈ړ�����
		&& attack->GetPos().z < sceneGame_->GetStageHSizeZ() + (sceneGame_->GetTileSizeZ() / 2)		//���[�̈ړ�����
		&& attack->GetPos().x > -sceneGame_->GetStageHSizeX() - (sceneGame_->GetTileSizeX() / 2)	//���[�̈ړ�����
		)
	{
		//�U�����n�߂�
		switch (dir_)
		{
		case Utility::DIR_3D::FRONT:
			//��}�X��ɍU���𐶐�����
			attack->Init(sceneGame_, this, VAdd(pos_, VGet(0.0f, 0.0f, sceneGame_->GetTileSizeZ())), dir_, charaNum_);
			break;

		case Utility::DIR_3D::RIGHT:
			//��}�X��ɍU���𐶐�����
			attack->Init(sceneGame_, this, VAdd(pos_, VGet(sceneGame_->GetTileSizeX(), 0.0f, 0.0f)), dir_, charaNum_);
			break;

		case Utility::DIR_3D::BACK:
			//��}�X��ɍU���𐶐�����
			attack->Init(sceneGame_, this, VSub(pos_, VGet(0.0f, 0.0f, sceneGame_->GetTileSizeZ())), dir_, charaNum_);
			break;

		case Utility::DIR_3D::LEFT:
			//��}�X��ɍU���𐶐�����
			attack->Init(sceneGame_, this, VSub(pos_, VGet(sceneGame_->GetTileSizeX(), 0.0f, 0.0f)), dir_, charaNum_);
			break;
		}

		//�U��SE
		se_->PlaySE(SoundManager::SE_TYPE::ATTACK, DX_PLAYTYPE_BACK, 70);
	}
}

//�L���ȍU�����擾
Attack* Player::GetValidAttack(void)
{
	size_t size = attack_.size();
	for (int i = 0; i < size; i++)
	{
		if (!attack_[i]->GetIsAlive())
		{
			return attack_[i];
		}
	}

	//�V�����U���p�ɔz���ǉ�
	Attack* attack = new Attack();
	attack_.push_back(attack);

	return attack;

}

//�v���C���[�̏�Ԃ�Ԃ�
const Player::STATE Player::GetState(void)
{
	return state_;
}

//�v���C���[�̏�ԕύX(_state:�ύX���������)
void Player::ChangeState(const STATE _state)
{
	state_ = _state;
}

//�v���C���[�̌��ݍ��W��Ԃ�
const VECTOR Player::GetPos(void)
{
	return pos_;
}

//�v���C���[�̍��W��^����
void Player::SetPos(const VECTOR _setPos)
{
	pos_ = _setPos;
}

//�v���C���[�̕�����Ԃ�
const Utility::DIR_3D Player::GetDir(void)
{
	return dir_;
}

const bool Player::GetIsWalk(void)
{
	return isWalk_;
}

const int Player::GetLife(void)
{
	return life_;
}

const int Player::GetDeadTime(void)
{
	return deadTime_;
}

void Player::SetDeadTime(void)
{
	deadTime_ = GetNowCount();
}

const int Player::GetBrokeTileNum(void)
{
	return brokeTileNum_;
}

void Player::IncreaseBrokeTileNum(void)
{
	brokeTileNum_++;
}

const int Player::GetRank(void)
{
	return rank_;
}

void Player::SetRank(const int _rank)
{
	rank_ = _rank;
}

const Player::ANIM_NUM Player::GetAnimNum(void)
{
	return animNum_;
}

void Player::ChangeAnim(const ANIM_NUM _animNo)
{
	animNum_ = _animNo;

	//���݂̃A�j���[�V�������f�^�b�`����
	MV1DetachAnim(model_,animAttachNo_);

	//�V���������Ă����A�j���[�V�������A�^�b�`����
	animAttachNo_ = MV1AttachAnim(model_,static_cast<int>(animNum_));

	//�A�j���[�V�������Ԃ̏�����
	stepAnim_ = 0.0f;
	animTotalTime_ = MV1GetAttachAnimTotalTime(model_, animAttachNo_);

	//�A�j���[�V�����̑��x
	switch (animNum_)
	{
	case Player::ANIM_NUM::IDLE:
		speedAnim_ = SPEED_ANIM;
		break;

	case Player::ANIM_NUM::WALK:
		//���̃^�C���Ɉړ�����܂łɈꏄ����
		speedAnim_ = SPEED_ANIM * (animTotalTime_ / (sceneGame_->GetTileSizeX() / speed_));
		break;

	case Player::ANIM_NUM::ATTACK:
		speedAnim_ = SPEED_ANIM;
		break;

	case Player::ANIM_NUM::FALL:
		speedAnim_ = SPEED_ANIM;
		break;
	}
}

//�v���C���[�̖��G���Ԃ�Ԃ�
const float Player::GetInvincible(void)
{
	return invincible_;
}

const Player::CHARA_JUDGE Player::GetCharaJudge(void)
{
	return charaJudge_;
}

//�v���C���[���G����^����(_type:�v���C���[���G���̔��f)
void Player::SetCharaJudge(const CHARA_JUDGE _type)
{
	charaJudge_ = _type;
}

const CommonData::TYPE Player::GetCharaNum(void)
{
	return charaNum_;
}

//�U���̃|�C���^��Ԃ�
std::vector<Attack*> Player::GetAttack(void)
{
	return attack_;
}

void Player::SetDifficulty(const CommonData::DIFFICULTY _diff)
{
	if (charaJudge_ == CHARA_JUDGE::CPU)
	{
		difficulty_ = _diff;
	}
}

void Player::SetSpeed(const float _speed)
{
	speed_ = _speed;
}

void Player::RunMoveInterval(void)
{
	//�ړ��Ԋu�����̏��������Ă��Ȃ��Ȃ�
	if (!isEndMoveInterval_)
	{
		//��Փx
		switch (difficulty_)
		{
		case CommonData::DIFFICULTY::EASY:
			//�����Ԋu�����߂�
			CPUMoveInterVal_ = CPU_MOVE_INTERVAL_EASY / 2;

			break;

		case CommonData::DIFFICULTY::NORMAL:
			//�Ԋu�����߂�
			CPUMoveInterVal_ = CPU_MOVE_INTERVAL_NORMAL / 1.5f;

			break;

		case CommonData::DIFFICULTY::HARD:
			//�����ɓ���
			CPUMoveInterVal_ = CPU_MOVE_INTERVAL_HARD;

			break;
		}
	}

	//�ړ��Ԋu�����߂�
	isEndMoveInterval_ = true;
}

//CPU�̈ړ����f
const Utility::DIR_3D Player::CPUMoveChack(const Utility::DIR_3D _dir)
{
	//�����_���ŕ��������߂�
	Utility::DIR_3D dir = _dir;

	//������������Ȃ�������
	if (!isMoveFront_ && !isMoveRight_ && !isMoveBack_ && !isMoveLeft_)
	{
		//�ׂ̃^�C�����U����ԂȂ炻�����ɓ���
		if (sceneGame_->IsNextTileAttack(pos_, dir))
		{
			switch (dir)
			{
			case Utility::DIR_3D::FRONT:
				isMoveFront_ = true;
				break;

			case Utility::DIR_3D::RIGHT:
				isMoveRight_ = true;
				break;
			
			case Utility::DIR_3D::BACK:
				isMoveBack_ = true;
				break;
			
			case Utility::DIR_3D::LEFT:
				isMoveLeft_ = true;
				break;
			}
		}

		//����v���C���[�Ƃ̏Փ˔���
		if (sceneGame_->HitP2P(pos_, dir, charaNum_))
		{
			switch (dir)
			{
			case Utility::DIR_3D::FRONT:
				isMoveFront_ = false;
				break;

			case Utility::DIR_3D::RIGHT:
				isMoveRight_ = false;
				break;

			case Utility::DIR_3D::BACK:
				isMoveBack_ = false;
				break;

			case Utility::DIR_3D::LEFT:
				isMoveLeft_ = false;
				break;
			}
		}

		//�[�Ƃ̏Փ˔���
		if (sceneGame_->IsCollisionStageEnd(pos_, dir))
		{
			//�[�̈ړ�����
			switch (dir)
			{
			case Utility::DIR_3D::FRONT:
				isMoveFront_ = false;

				break;
			case Utility::DIR_3D::RIGHT:
				isMoveRight_ = false;

				break;
			case Utility::DIR_3D::BACK:
				isMoveBack_ = false;

				break;
			case Utility::DIR_3D::LEFT:
				isMoveLeft_ = false;

				break;
			}
		}

		if (!isMoveFront_ && !isMoveRight_ && !isMoveBack_ && !isMoveLeft_)
		{
			//������
			isMoveFront_ = true;
			isMoveRight_ = true;
			isMoveBack_ = true;
			isMoveLeft_ = true;

			//�����Ȃ�
			return Utility::DIR_3D::MAX;
		}
		else
		{
			//�����̌��߂Ȃ���
			while (!isMoveFront_ && dir == Utility::DIR_3D::FRONT
				|| !isMoveRight_ && dir == Utility::DIR_3D::RIGHT
				|| !isMoveBack_ && dir == Utility::DIR_3D::BACK
				|| !isMoveLeft_ && dir == Utility::DIR_3D::LEFT)
			{
				//�󂢂Ă�������Ō��߂Ȃ���
				dir = static_cast<Utility::DIR_3D>(GetRand(static_cast<int>(Utility::DIR_3D::LEFT)));
			}

			//�����̊m��
			return dir;
		}
	}


	//�ׂ̃^�C�������Ă���A�U����ԁA�I��������ԁA�I����Ԃ̂ǂꂩ�@���́@�[�̈ړ������ɓ��������@���́@����v���C���[�ƏՓ˂���
	if (sceneGame_->IsNextTileBroken(pos_, dir) 
		|| sceneGame_->IsNextTileAttack(pos_, dir) 
		|| sceneGame_->IsNextTilePreEnd(pos_, dir) 
		|| sceneGame_->IsNextTileEnd(pos_, dir) 
		|| sceneGame_->IsCollisionStageEnd(pos_, dir) 
		|| sceneGame_->HitP2P(pos_, dir, charaNum_))
	{
		switch (dir)
		{
		case Utility::DIR_3D::FRONT:
			isMoveFront_ = false;
			return CPUMoveChack(static_cast<Utility::DIR_3D>(GetRand(static_cast<int>(Utility::DIR_3D::LEFT))));

			break;

		case Utility::DIR_3D::RIGHT:
			isMoveRight_ = false;
			return CPUMoveChack(static_cast<Utility::DIR_3D>(GetRand(static_cast<int>(Utility::DIR_3D::LEFT))));

			break;

		case Utility::DIR_3D::BACK:
			isMoveBack_ = false;
			return CPUMoveChack(static_cast<Utility::DIR_3D>(GetRand(static_cast<int>(Utility::DIR_3D::LEFT))));

			break;

		case Utility::DIR_3D::LEFT:
			isMoveLeft_ = false;
			return CPUMoveChack(static_cast<Utility::DIR_3D>(GetRand(static_cast<int>(Utility::DIR_3D::LEFT))));

			break;
		}
	}

	//������
	isMoveFront_ = true;
	isMoveRight_ = true;
	isMoveBack_ = true;
	isMoveLeft_ = true;

	//�����̊m��
	return dir;
}

void Player::UpdateAnimation(void)
{
	//�e�A�j���[�V�������I�������
	if (animNum_ == ANIM_NUM::WALK && !isWalk_
		|| animNum_ == ANIM_NUM::ATTACK && state_ != STATE::ATTACK)
	{
		//������Ԃɖ߂�
		ChangeAnim(ANIM_NUM::IDLE);
	}
	else if (animNum_ == ANIM_NUM::FALL)
	{
		//�����A�j���[�V����
		FallAnimation();
	}
}

void Player::FallAnimation(void)
{
	//�������Ł@���@�A�j���[�V�������I����ĂȂ��Ȃ�
	if (animNum_ == ANIM_NUM::FALL && animTotalTime_ > stepAnim_)
	{
		//�����Â������Ă���
		pos_.y -= FALL_SPEED;
	}
	//�����A�j���[�V�������I�������
	else if (animNum_ == ANIM_NUM::FALL && animTotalTime_ <= stepAnim_)
	{
		//���G���Ԃ����Z�b�g
		invincible_ = 0.0f;

		//��������
		sceneGame_->ReviavalPlayer(this);

		//�_���[�W
		switch (CommonData::GetInstance().GetRule())
		{
		case CommonData::RULE::TIME:
			if (charaNum_ == attackedPlayer_)
			{
				Damage(SELF_SUB_SCORE);
			}
			else
			{
				Damage(DAMAGE);

				//�U�����Ă����v���C���[�ɓ��_
				sceneGame_->TakeScore(attackedPlayer_);
			}
			break;
		
		case CommonData::RULE::LIFE:
			Damage(DAMAGE);
			break;
		
		case CommonData::RULE::BREAK_TILE:
			Damage(SUB_TILE_BROKE_SCORE);
			break;
		}

		//�A�j���[�V�����I��
		ChangeAnim(ANIM_NUM::IDLE);
	}
}

void Player::Animation(void)
{
	//�傫���A�p�x�A���W��3D����
	//---------------------------------
	//�傫���̐ݒ�
	MV1SetPosition(model_, pos_);

	//�p�x�̐ݒ�
	MV1SetScale(model_, scale_);

	//�ʒu�̐ݒ�
	MV1SetRotationXYZ(model_, rot_);

	//�A�j���[�V�����Đ�
	//-------------------------------------------

	//�A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (speedAnim_ * Utility::DELTA_TIME);
	if (stepAnim_ > animTotalTime_)
	{
		//���[�v�Đ�
		stepAnim_ = 0.0f;
	}

	//�Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(model_, animAttachNo_, stepAnim_);
}
