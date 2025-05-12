#include<DxLib.h>
#include"../Application.h"
#include"../Manager/InputManager.h"
#include"../Manager/EffectManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/SceneManager.h"
#include"../Common/CommonData.h"
#include"../Object/Grid.h"
#include"../Object/Stage.h"
#include"../Rule/RuleTime.h"
#include"../Rule/RuleLife.h"
#include"../Rule/RuleBreakTile.h"
#include"../Object/Player.h"
#include"../Object/Attack.h"
#include"../Object/Item/ItemBase.h"
#include"../Object/Item/ItemHammer.h"
#include"../Object/Item/ItemBoots.h"
#include"SceneGame.h"

//����������
bool SceneGame::Init(void)
{
	//�C���X�^���X����
	//------------------------------------------

	//�G�t�F�N�g�̃C���X�^���X����
	effect_ = new EffectManager();
	effect_->EffectInit();

	//�T�E���h�̃C���X�^���X����
	sound_ = new SoundManager();
	sound_->SoundInit();

	//�T�E���h�̃��[�h
	sound_->BGMLoad(SoundManager::BGM_TYPE::BATTLE_FIRST_HALF);
	sound_->BGMLoad(SoundManager::BGM_TYPE::BATTLE_SECOND_HALF);

	//�o�g��BGM
	sound_->PlayBGM(SoundManager::BGM_TYPE::BATTLE_FIRST_HALF, DX_PLAYTYPE_LOOP, BATTLE_FIRST_BGM_VOLUME, false);

	//�X�e�[�W�̃C���X�^���X����
	stage_ = new Stage;
	if (stage_->Init(this) == false)
	{
		OutputDebugString("Stage�̏��������s\n");
		return false;
	}

	//�Q�[���̏�Ԃ̏�����
	ChangeGameState(GAME_STATE::PRE_START);

	//���[������
	SetRule();

	//�A�C�e���̏o���Ԋu
	itemInterval_ = 0;

	//��x�������s
	Update();

	return true;
}

void SceneGame::Update(void)
{
	SceneManager& scnMng = SceneManager::GetInstance();

	switch (gameState_)
	{
	case SceneGame::GAME_STATE::PRE_START:
		//�Q�[���J�n�̏���
		ChangeGameState(GAME_STATE::START);
		break;

	case SceneGame::GAME_STATE::START:
		//�Q�[���J�n
		ChangeGameState(GAME_STATE::GAME);

		for (auto player : player_)
		{
			effect_->EffectPlay(EffectManager::EFF_TYPE::START
				, VSub(player->GetPos(), { 0.0f,Player::RADIUS,0.0f })
				, START_EFFECT_SCALE
				, Utility::VECTOR_ZERO);
		}

		break;

	case SceneGame::GAME_STATE::END:
		//�Q�[���̏������~�߂�
		scnMng.ChangeScene(SceneManager::SCENE_ID::RESULT, true);
		return;
	}

	//BGM
	//�͈͏k�����Ă���Ȃ�
	if (rule_->GetIsNarrowStage())
	{
		//�O����BGM���~�߂�
		sound_->StopBGM(SoundManager::BGM_TYPE::BATTLE_FIRST_HALF);

		//�㔼��BGM�𗬂��n�߂�
		sound_->PlayBGM(SoundManager::BGM_TYPE::BATTLE_SECOND_HALF, DX_PLAYTYPE_LOOP, BATTLE_SECOND_BGM_VOLUME,false);
	}

	//�X�e�[�W�̍X�V
	stage_->Update();

	//���[�����Ƃ̏���
	rule_->Update();

	//�v���C���[�̏����ݒ�ƍX�V
	ProcessPlayer();

	//�A�C�e���̐���
	AppearItem();

	//�v���C���[���Ƃ̏���
	for (auto player : player_)
	{
		for (int tz = 0; tz < Stage::TILE_NUM; tz++)
		{
			for (int tx = 0; tx < Stage::TILE_NUM; tx++)
			{
				//�v���C���[�Ə��̓����蔻��
				//---------------------------------------
				//�v���C���[���������Ă���	���@�_���[�W��Ԃł͂Ȃ��@���@�����A�j���[�V�������ł͂Ȃ�
				if (player->GetState() != Player::STATE::DEAD && player->GetInvincible() > Player::INVINCIBLE && player->GetAnimNum() != Player::ANIM_NUM::FALL)
				{
					//�v���C���[���^�C���̒����ɂ���@���@�^�C���������
					if ((player->GetPos().x) == (stage_->GetPos(tx, tz).x)
						&& (player->GetPos().z) == (stage_->GetPos(tx, tz).z)
						&& (stage_->GetTileState(tx, tz) == Stage::TILE_STATE::BROKEN
							|| stage_->GetTileState(tx, tz) == Stage::TILE_STATE::END)
							)
					{
						//�U�����Ă����v���C���[��ۑ�
						player->PreDamage(stage_->GetTileAttackPlayer(tx, tz));
					}
				}

				for (auto attack : player->GetAttack())
				{
					//�U���Ə��̓����蔻��
					//---------------------------------------

					//�U�����@���@�����ʏ��Ԃ̎�	
					if (attack->GetIsAlive() && stage_->GetTileState(tx, tz) == Stage::TILE_STATE::NORMAL)
					{
						//�U�����^�C���Əd�Ȃ���
						if ((attack->GetPos().x) <= (stage_->GetPos(tx, tz).x + Stage::TILE_HSIZE_X)
							&& (attack->GetPos().x) >= (stage_->GetPos(tx, tz).x - Stage::TILE_HSIZE_X)
							&& (attack->GetPos().z) <= (stage_->GetPos(tx, tz).z + Stage::TILE_HSIZE_Z)
							&& (attack->GetPos().z) >= (stage_->GetPos(tx, tz).z - Stage::TILE_HSIZE_Z))
						{
							//�^�C�����U����Ԃɂ���
							stage_->TileSwap(tx, tz, Stage::TILE_STATE::ATTACK);

							//�U�������v���C���[��ۑ�
							stage_->TileAttackPlayer(tx, tz, attack->GetCharaNum());
						}
					}
					//���łɍU����Ԃ������ꍇ
					else if (attack->GetIsAlive() && stage_->GetTileState(tx, tz) == Stage::TILE_STATE::ATTACK)
					{
						//�U�����^�C���Əd�Ȃ���
						if ((attack->GetPos().x) <= (stage_->GetPos(tx, tz).x + Stage::TILE_HSIZE_X)
							&& (attack->GetPos().x) >= (stage_->GetPos(tx, tz).x - Stage::TILE_HSIZE_X)
							&& (attack->GetPos().z) <= (stage_->GetPos(tx, tz).z + Stage::TILE_HSIZE_Z)
							&& (attack->GetPos().z) >= (stage_->GetPos(tx, tz).z - Stage::TILE_HSIZE_Z))
						{
							//�U�������v���C���[���̂ݏ㏑��
							stage_->TileAttackPlayer(tx, tz, attack->GetCharaNum());
						}
					}
				}

				//�I��点�����ƃA�C�e���̔���
				for (auto item : item_)
				{
					//�A�C�e�����擾����Ă��Ȃ���ԂŏI���������Ɠ�������
					if (item->GetIsAlive()
						&& (item->GetPos().x == stage_->GetPos(tx, tz).x)
						&& (item->GetPos().z == stage_->GetPos(tx, tz).z)
						&& (stage_->GetTileState(tx,tz) == Stage::TILE_STATE::END
						|| stage_->GetTileState(tx,tz) == Stage::TILE_STATE::BROKEN)
						)
					{
						//�A�C�e��������
						item->DeleteItem();
					}
				}
			}
		}

		for (auto attack : player->GetAttack())
		{
			//�U���̍X�V
			attack->Update();
		}

		for (auto item : item_)
		{
			if (
				player->GetPos().x == item->GetPos().x
				&& player->GetPos().z == item->GetPos().z
				&& item->GetIsAlive()
				)
			{
				//�A�C�e�����폜����
				item->KillItem();

				//�v���C���[�̃A�C�e���擾����
				PlayerGotItem(player, item);
			}
		}
	}
}

void SceneGame::Draw(void)
{
	//�X�e�[�W�̕`��
	stage_->Draw();

	//���[�����Ƃ̕`��
	rule_->Draw();

	//�Q�[���J�n�������@���́@�Q�[���I����Ȃ�
	if (gameState_ == GAME_STATE::PRE_START || gameState_ == GAME_STATE::END)
	{
		return;
	}

	//�A�C�e���̕`��
	for (auto item : item_)
	{
		//�A�C�e���̕`��
		item->Draw();
	}

	for (auto player : player_)
	{
		//�v���C���[�̕`��
		player->Draw();
	
		for (auto attack : player->GetAttack())
		{
			//�U���̕`��
			attack->Draw();
		}
	}
}

bool SceneGame::Release(void)
{
	//�A�C�e���̐������
	for (auto item : item_)
	{
		//�A�C�e���̉��
		item->Release();
		delete item;
		item = nullptr;
	}

	//�v���C���[�̐����擾
	for (auto player : player_)
	{
		//�v���C���[�̉��
		player->Release();
		delete player;
		player = nullptr;
	}

	//���[���̉��
	rule_->Release();
	delete rule_;
	rule_ = nullptr;

	//�X�e�[�W�̉��
	delete stage_;
	stage_ = nullptr;

	//�T�E���h�̉��
	sound_->SoundRelease();
	delete sound_;
	sound_ = nullptr;

	//�G�t�F�N�g�̉��
	effect_->EffectEnd();
	delete effect_;
	effect_ = nullptr;

	return true;
}

const int SceneGame::GetTileNum(void)
{
	return Stage::TILE_NUM;
}

const int SceneGame::GetTileHNum(void)
{
	return stage_->GetTileHNum();
}

const float SceneGame::GetTileSizeX(void)
{
	return Stage::TILE_SIZE_X;
}

const float SceneGame::GetTileSizeZ(void)
{
	return Stage::TILE_SIZE_Z;
}

const float SceneGame::GetStageHSizeX(void)
{
	return Stage::STAGE_HSIZE_X;
}

const float SceneGame::GetStageHSizeZ(void)
{
	return Stage::STAGE_HSIZE_Z;
}

const SceneGame::GAME_STATE SceneGame::GetGameState(void)
{
	return gameState_;
}

void SceneGame::ChangeGameState(const GAME_STATE _state)
{
	gameState_ = _state;
}

const float SceneGame::GetAtkCnt(const int ix, const int iz)
{
	return stage_->GetAtkCnt(ix, iz);
}

void SceneGame::SetAtkCnt(const int ix, const int iz, const float _cnt)
{
	stage_->SetAtkCnt(ix, iz, _cnt);
}


const bool SceneGame::IsCollisionStageEnd(const VECTOR _pos, const Utility::DIR_3D _dir)
{
	switch (_dir)
	{
	case Utility::DIR_3D::FRONT:
		//�O���̒[�̈ړ�����
		if (_pos.z >= Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z)
		{
			return true;
		}
		break;

	case Utility::DIR_3D::RIGHT:
		//�E�[�̈ړ�����
		if (_pos.x >= Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X)
		{
			return true;
		}
		break;

	case Utility::DIR_3D::BACK:
		//��둤�̒[�̈ړ�����
		if (_pos.z <= -Stage::STAGE_HSIZE_Z + Stage::TILE_HSIZE_Z)
		{
			return true;
		}
		break;

	case Utility::DIR_3D::LEFT:
		//���[�̈ړ�����
		if (_pos.x <= -Stage::STAGE_HSIZE_X + Stage::TILE_HSIZE_X)
		{
			return true;
		}
		break;
	}

	return false;
}

const bool SceneGame::IsNextTileBroken(const VECTOR _pos, const Utility::DIR_3D _dir)
{
	VECTOR pos = _pos;
	Utility::DIR_3D dir = _dir;

	//�v���C���[�̃^�C���ɂ��ړ�����
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//�ׂ̃^�C���������(�X�e�[�W���O�͔��肵�Ȃ�)
			//--------------------------------
			if (!IsCollisionStageEnd(_pos, _dir))
			{
				switch (_dir)
				{
				case Utility::DIR_3D::FRONT:
					//�O
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) + 1)
						== Stage::TILE_STATE::BROKEN)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::RIGHT:
					//�E
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) + 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::BROKEN)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::BACK:
					//���
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) - 1)
						== Stage::TILE_STATE::BROKEN)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::LEFT:
					//��
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) - 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::BROKEN)
					{
						return true;
					}
					break;
				}
			}
		}
	}
	return false;
}

const bool SceneGame::IsNextTileAttack(const VECTOR _pos, const Utility::DIR_3D _dir)
{
	VECTOR pos = _pos;
	Utility::DIR_3D dir = _dir;

	//�v���C���[�̃^�C���ɂ��ړ�����
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//�ׂ̃^�C�����U�����(�X�e�[�W���O�͔��肵�Ȃ�)
			//--------------------------------
			if (!IsCollisionStageEnd(_pos, _dir))
			{
				switch (_dir)
				{
				case Utility::DIR_3D::FRONT:
					//�O
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) + 1)
						== Stage::TILE_STATE::ATTACK)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::RIGHT:
					//�E
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) + 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::ATTACK)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::BACK:
					//���
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) - 1)
						== Stage::TILE_STATE::ATTACK)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::LEFT:
					//��
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) - 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::ATTACK)
					{
						return true;
					}
					break;
				}
			}
		}
	}
	return false;
}

const bool SceneGame::IsThisTileAttack(const VECTOR _pos)
{
	VECTOR pos = _pos;

	//�v���C���[�̃^�C���ɂ��ړ�����
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//�����̗����Ă���^�C�����U�����
			if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z))
				== Stage::TILE_STATE::ATTACK)
			{
				return true;
			}
		}
	}
	return false;
}

const bool SceneGame::IsNextTilePreEnd(const VECTOR _pos, const Utility::DIR_3D _dir)
{
	VECTOR pos = _pos;
	Utility::DIR_3D dir = _dir;

	//�v���C���[�̃^�C���ɂ��ړ�����
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//�ׂ̃^�C�����U�����(�X�e�[�W���O�͔��肵�Ȃ�)
			//--------------------------------
			if (!IsCollisionStageEnd(_pos, _dir))
			{
				switch (_dir)
				{
				case Utility::DIR_3D::FRONT:
					//�O
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) + 1)
						== Stage::TILE_STATE::PREEND)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::RIGHT:
					//�E
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) + 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::PREEND)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::BACK:
					//���
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) - 1)
						== Stage::TILE_STATE::PREEND)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::LEFT:
					//��
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) - 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::PREEND)
					{
						return true;
					}
					break;
				}			
			}
		}
	}
	return false;
}

const bool SceneGame::IsThisTilePreEnd(const VECTOR _pos)
{
	VECTOR pos = _pos;

	//�v���C���[�̃^�C���ɂ��ړ�����
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//�����̗����Ă���^�C�����U�����
			if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z))
				== Stage::TILE_STATE::PREEND)
			{
				return true;
			}
		}
	}
	return false;
}

const bool SceneGame::IsNextTileEnd(const VECTOR _pos, const Utility::DIR_3D _dir)
{
	VECTOR pos = _pos;
	Utility::DIR_3D dir = _dir;

	//�v���C���[�̃^�C���ɂ��ړ�����
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//�ׂ̃^�C�����U�����(�X�e�[�W���O�͔��肵�Ȃ�)
			//--------------------------------
			if (!IsCollisionStageEnd(_pos, _dir))
			{
				switch (_dir)
				{
				case Utility::DIR_3D::FRONT:
					//�O
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) + 1)
						== Stage::TILE_STATE::END)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::RIGHT:
					//�E
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) + 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::END)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::BACK:
					//���
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) - 1)
						== Stage::TILE_STATE::END)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::LEFT:
					//��
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) - 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::END)
					{
						return true;
					}
					break;
				}
			}
		}
	}
	return false;
}

std::vector<Player*> SceneGame::GetPlayer(void)
{
	return player_;
}

const int SceneGame::GetPlayerScore(Player* _player)
{
	return _player->GetScore();
}

void SceneGame::TakeScore(const CommonData::TYPE _charaNum)
{
	//�U�����Ă������肪���Ȃ��Ȃ�
	if (_charaNum == CommonData::TYPE::NONE)
	{
		//�������Ȃ�
		return;
	}

	for (auto player : player_)
	{
		if (player->GetCharaNum() == _charaNum)
		{
			player->TakeScore(1);
		}
	}
}

const int SceneGame::GetLife(Player* _player)
{
	return _player->GetLife();
}

const int SceneGame::GetBrokeTileNum(Player* _player)
{
	return _player->GetBrokeTileNum();
}

void SceneGame::IncreaseBrokeTileNum(const CommonData::TYPE _charaNum)
{
	//�v���C���[�ԍ��ɑΉ������z��ԍ�
	int playerNum = static_cast<int>(_charaNum) - 1;

	player_[playerNum]->IncreaseBrokeTileNum();
}

const CommonData::TYPE SceneGame::GetCharaNum(Player* _player)
{
	return _player->GetCharaNum();
}

void SceneGame::SetRule(void)
{
	CommonData::RULE rule = CommonData::GetInstance().GetRule();

	switch (rule)
	{
	case CommonData::RULE::TIME:
		rule_ = new RuleTime(this);
		break;

	case CommonData::RULE::LIFE:
		rule_ = new RuleLife(this);
		break;
	
	case CommonData::RULE::BREAK_TILE:
		rule_ = new RuleBreakTile(this);
		break;
	}

	rule_->Init();
}

void SceneGame::ProcessPlayer(void)
{
	for (auto player : player_)
	{
		//�v���C���[�̍X�V
		player->Update();
	}

	//�C���X�^���X�����p
	Player* playerIns;

	auto& common = CommonData::GetInstance();

	//�ΐ�l���i�[�p
	CommonData::BATTLE_PATTERN bPat_;
	bPat_ = common.GetBattlePattern();

	//�ΐ�l���p
	int bNum = 0;

	//�L�����̐l�����擾
	size_t size = player_.size();

	//�l�����킹
	switch (bPat_)
	{
		//2�l
	case CommonData::BATTLE_PATTERN::P1C1:
	case CommonData::BATTLE_PATTERN::P2:
		bNum = static_cast<int>(CommonData::TYPE::P2);
		break;

		//3�l
	case CommonData::BATTLE_PATTERN::P1C2:
	case CommonData::BATTLE_PATTERN::P2C1:
	case CommonData::BATTLE_PATTERN::P3:
		bNum = static_cast<int>(CommonData::TYPE::P3);
		break;

		//4�l
	case CommonData::BATTLE_PATTERN::P1C3:
	case CommonData::BATTLE_PATTERN::P2C2:
	case CommonData::BATTLE_PATTERN::P3C1:
	case CommonData::BATTLE_PATTERN::P4:
		bNum = static_cast<int>(CommonData::TYPE::P4);
		break;
	}

	//�ΐ�l�����̔z��𐶐�
	while (size < bNum)
	{
		//�v���C���[�̃C���X�^���X����
		playerIns = new Player();

		//�v���C���[�̐l��
		int pNum = 1;

		//�L�������ƂɃv���C���[��CPU���̑�����^����
		//------------------------------
		//���������l������΂�
		pNum += static_cast<int>(size);		
		if (pNum <= static_cast<int>(common.GetPlayerNum()))
		{
			playerIns->SetCharaJudge(Player::CHARA_JUDGE::PLAYER);
		}
		else
		{
			playerIns->SetCharaJudge(Player::CHARA_JUDGE::CPU);
		}

		//����������
		//�L�����ԍ��ŏ����ʒu�����߂�
		if (size == 0)
		{
			//����ɐ���
			playerIns->Init(this, { stage_->GetPos(0,Stage::TILE_NUM - 1).x
									,Player::RADIUS
									,stage_->GetPos(0,Stage::TILE_NUM - 1).z } 
			, CommonData::TYPE::P1);
		}
		else if (size == 1)
		{
			//�E��ɐ���
			playerIns->Init(this, { stage_->GetPos(Stage::TILE_NUM - 1,Stage::TILE_NUM - 1).x
									,Player::RADIUS
									,stage_->GetPos(Stage::TILE_NUM - 1,Stage::TILE_NUM - 1).z }
			, CommonData::TYPE::P2);
		}
		else if (size == 2)
		{
			//�����ɐ���
			playerIns->Init(this, { stage_->GetPos(0,0).x
									,Player::RADIUS
									,stage_->GetPos(0,0).z }
			, CommonData::TYPE::P3);
		}
		else if (size == 3)
		{
			//�E���ɐ���
			playerIns->Init(this, { stage_->GetPos(Stage::TILE_NUM - 1, 0).x
									, Player::RADIUS
									, stage_->GetPos(Stage::TILE_NUM - 1, 0).z }
			, CommonData::TYPE::P4);
		}

		//�z��̒ǉ�
		player_.push_back(playerIns);

		//�L�����̐l���擾
		size = player_.size();
	}

	//��Փx�z�񂪑��݂���Ȃ���
	if (common.GetDifficultySize() > 0)
	{
		for (int p = size - 1; p > 0; p--)
		{
			//��Փx�ݒ�
			player_[p]->SetDifficulty(CommonData::GetInstance().PopDifficulty());
		}
	}
}

void SceneGame::ReviavalPlayer(Player* _player)
{
	//��
	bool tileNum[Stage::TILE_NUM][Stage::TILE_NUM];

	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//�����Ȃ��ꏊ�͕����s��
			if (stage_->GetTileState(tx, tz) != Stage::TILE_STATE::NORMAL)
			{
				tileNum[tz][tx] = false;
			}
			else
			{
				tileNum[tz][tx] = true;
			}
		}
	}

	//�e�v���C���[������ꏊ�ɂ͕������ł��Ȃ��悤�ɂ���
	for (auto player : player_)
	{
		//�����Ƃ͎Q�Ƃ��Ȃ�
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//�����Ă���Ȃ���̍��W���Q��
			if (player->GetIsWalk())
			{
				//�[����͎Q�Ƃ��Ȃ�
				if (!IsCollisionStageEnd(player->GetPos(), player->GetDir()))
				{
					switch (player->GetDir())
					{
					case Utility::DIR_3D::FRONT:
						//�ړ��ォ�ǂ���
						if (static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z) + 1 >= Stage::TILE_NUM)
						{
							//�����Ă���ʒu���ړ��s��
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						else
						{
							//�ړ�����o���s��
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z) + 1]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						break;

					case Utility::DIR_3D::RIGHT:
						//�ړ��ォ�ǂ���
						if (static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X) + 1 >= Stage::TILE_NUM)
						{
							//�����Ă���ʒu���ړ��s��
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						else
						{
							//�ړ�����o���s��
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X) + 1] = false;
						}
						break;

					case Utility::DIR_3D::BACK:
						//�ړ��ォ�ǂ���
						if (static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z) - 1 < 0)
						{
							//�����Ă���ʒu���ړ��s��
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						else
						{
							//�ړ�����o���s��
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z) - 1]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						break;

					case Utility::DIR_3D::LEFT:
						//�ړ��ォ�ǂ���
						if (static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X) - 1 < 0)
						{
							//�����Ă���ʒu���ړ��s��
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						else
						{
							//�ړ�����o���s��
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X) - 1] = false;
						}
					
						break;
					}
				}
				//�[�Ȃ猻�ݍ��W��false�ɂ���
				else
				{
					//�����Ă���ʒu���ړ��s��
					tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
						[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
				}
			}
			else
			{
				//�����Ă���ʒu���ړ��s��
				tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
					[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
			}
		}
	}

	//�����_���̏ꏊ
	int randZ = GetRand(Stage::TILE_NUM - 1);
	int randX = GetRand(Stage::TILE_NUM - 1);

	//�o���\��
	if (tileNum[randZ][randX])
	{
		//���W�Z�b�g
		_player->SetPos(
			{ Stage::TILE_SIZE_X * (randX - Stage::TILE_HNUM)
				,Player::RADIUS
			 ,Stage::TILE_SIZE_Z * (randZ - Stage::TILE_HNUM) }
		);
	}
	else
	{
		//��蒼��
		ReviavalPlayer(_player);
	}
}

const bool SceneGame::HitP2P(const VECTOR _pos, const Utility::DIR_3D _dir, const CommonData::TYPE _num)
{
	//�Q�ƃv���C���[�̈ʒu
	auto myPos = _pos;
	//�Q�ƃv���C���[�̌���
	auto myDir = _dir;
	//�Q�ƃv���C���[�̃v���C���[�ԍ�
	auto myNum = _num;
	//�Q�ƃv���C���[�̓��������^�C��
	bool myTileNum[Stage::TILE_NUM][Stage::TILE_NUM];

	//����v���C���[���Ƃ̃^�C������
	bool oppTileNum[Stage::TILE_NUM][Stage::TILE_NUM];

	//������
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			myTileNum[tz][tx] = false;
			oppTileNum[tz][tx] = false;
		}
	}

	for (auto player : player_)
	{
		//�����Ƃ��ꂽ�v���C���[�Ƃ͔�r���Ȃ�
		if (myNum != player->GetCharaNum() && player->GetState() != Player::STATE::DEAD)
		{
			//����v���C���[�������Ă���Ȃ�ړ�����Q��
			if (player->GetIsWalk())
			{
				switch (player->GetDir())
				{
				case Utility::DIR_3D::FRONT:
					//�[����͎Q�Ƃ��Ȃ�
					if (player->GetPos().z + Stage::TILE_SIZE_Z < Stage::STAGE_HSIZE_Z + Stage::TILE_HSIZE_Z)
					{
						//��}�X�@�{�@�������̏ꏊ���Ƃ�
						oppTileNum
							[static_cast<int>(((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z - Player::SPEED_NORMAL) / Stage::TILE_SIZE_Z) + 1.0f)]
						[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X) / Stage::TILE_SIZE_X)]
						= true;
						oppTileNum
							[static_cast<int>(((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z - 1.0f) / Stage::TILE_SIZE_Z) + 1.0f)]
						[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X) / Stage::TILE_SIZE_X)]
						= true;
					}
					break;

				case Utility::DIR_3D::RIGHT:
					//�[����͎Q�Ƃ��Ȃ�
					if (player->GetPos().x + Stage::TILE_SIZE_X < Stage::STAGE_HSIZE_X + Stage::TILE_HSIZE_X)
					{
						//��}�X�@�{�@�������̏ꏊ���Ƃ�
						oppTileNum
							[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
						[static_cast<int>(((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X - Player::SPEED_NORMAL) / Stage::TILE_SIZE_X) + 1.0f)]
						= true;
						oppTileNum
							[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
						[static_cast<int>(((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X - 1.0f) / Stage::TILE_SIZE_X) + 1.0f)]
						= true;
					}
					break;

				case Utility::DIR_3D::BACK:
					//�[����͎Q�Ƃ��Ȃ�
					if (player->GetPos().z - Stage::TILE_SIZE_Z > -Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z)
					{
						//��}�X�@�{�@�������̏ꏊ���Ƃ�
						oppTileNum
							[static_cast<int>(((player->GetPos().z + Stage::STAGE_HSIZE_Z + Stage::TILE_HSIZE_Z + Player::SPEED_NORMAL) / Stage::TILE_SIZE_Z) - 1.0f)]
						[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X) / Stage::TILE_SIZE_X)]
						= true;
						oppTileNum
							[static_cast<int>(((player->GetPos().z + Stage::STAGE_HSIZE_Z + Stage::TILE_HSIZE_Z + 1.0f) / Stage::TILE_SIZE_Z) - 1.0f)]
						[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X) / Stage::TILE_SIZE_X)]
						= true;
					}
					break;

				case Utility::DIR_3D::LEFT:
					//�[����͎Q�Ƃ��Ȃ�
					if (player->GetPos().x - Stage::TILE_SIZE_X > -Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X)
					{
						//��}�X�@�{�@�������̏ꏊ���Ƃ�
						oppTileNum
							[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
						[static_cast<int>(((player->GetPos().x + Stage::STAGE_HSIZE_X + Stage::TILE_HSIZE_X + Player::SPEED_NORMAL) / Stage::TILE_SIZE_X) - 1.0f)]
						= true;
						oppTileNum
							[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
						[static_cast<int>(((player->GetPos().x + Stage::STAGE_HSIZE_X + Stage::TILE_HSIZE_X + 1.0f) / Stage::TILE_SIZE_X) - 1.0f)]
						= true;
					}
					break;
				}
			}

			for (int tz = 0; tz < Stage::TILE_NUM; tz++)
			{
				for (int tx = 0; tx < Stage::TILE_NUM; tx++)
				{
					//����v���C���[�������Ă��Ȃ��@���́@�[�ɂ���Ȃ痧���Ă���ꏊ���Q��
					if (!player->GetIsWalk()
						|| player->GetPos().z + Stage::TILE_SIZE_Z >= Stage::STAGE_HSIZE_Z + Stage::TILE_HSIZE_Z
						|| player->GetPos().x + Stage::TILE_SIZE_X >= Stage::STAGE_HSIZE_X + Stage::TILE_HSIZE_X
						|| player->GetPos().z - Stage::TILE_SIZE_Z <= -Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z
						|| player->GetPos().x - Stage::TILE_SIZE_X <= -Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X)
					{
						if (player->GetPos().x == Stage::TILE_SIZE_X * (tx - Stage::TILE_HNUM)
							&& player->GetPos().z == Stage::TILE_SIZE_Z * (tz - Stage::TILE_HNUM))
						{
							//����v���C���[�̂���ʒu�����߂�
							oppTileNum[tz][tx] = true;
						}
					}
				}
			}
		}
	}

	//�[����͎Q�Ƃ��Ȃ�
	if (!IsCollisionStageEnd(myPos, myDir))
	{
		//�Q�ƃv���C���[�̓��������ʒu�����߂�
		switch (myDir)
		{
		case Utility::DIR_3D::FRONT:
			//�ړ��悪��������
			myTileNum[static_cast<int>(((myPos.z + Stage::STAGE_HSIZE_Z) / Stage::TILE_SIZE_Z) + 1.0f)]
				[static_cast<int>((myPos.x + Stage::STAGE_HSIZE_X) / Stage::TILE_SIZE_X)]
			= true;

			break;

		case Utility::DIR_3D::RIGHT:
			//�ړ��悪��������
			myTileNum[static_cast<int>((myPos.z + Stage::STAGE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
				[static_cast<int>(((myPos.x + Stage::STAGE_HSIZE_X) / Stage::TILE_SIZE_X) + 1.0f)]
			= true;

			break;

		case Utility::DIR_3D::BACK:
			//�ړ��悪��������
			myTileNum[static_cast<int>(((myPos.z + Stage::STAGE_HSIZE_Z) / Stage::TILE_SIZE_Z) - 1.0f)]
				[static_cast<int>((myPos.x + Stage::STAGE_HSIZE_X) / Stage::TILE_SIZE_X)]
			= true;

			break;

		case Utility::DIR_3D::LEFT:
			//�ړ��悪��������
			myTileNum[static_cast<int>((myPos.z + Stage::STAGE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
				[static_cast<int>(((myPos.x + Stage::STAGE_HSIZE_X) / Stage::TILE_SIZE_X) - 1.0f)]
			= true;

			break;
		}
	}

	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			for (auto player : player_)
			{
				//�����Ƃ��ꂽ�v���C���[�Ƃ͔�r���Ȃ�
				if (myNum != player->GetCharaNum() && player->GetState() != Player::STATE::DEAD)
				{
					if (myTileNum[tz][tx] && oppTileNum[tz][tx])
					{
						//�Փ˂���
						return true;
					}
				}
			}
		}
	}

	//�Փ˂��Ȃ�����
	return false;
}

const Utility::DIR_3D SceneGame::AimPlayer(const VECTOR _pos, const CommonData::TYPE _num)
{
	//�Ԃ�����
	Utility::DIR_3D dir;

	//�����̈ʒu
	VECTOR myPos = _pos;
	//�����̃v���C���[�ԍ�
	CommonData::TYPE num = _num;

	//�Ώۃv���C���[�ւ̕����x�N�g��
	VECTOR dirVector = Utility::VECTOR_ZERO;

	for (auto player : player_)
	{
		//�����Ƃ��ꂽ�v���C���[�Ƃ͔�r���Ȃ�
		if (num != player->GetCharaNum() && player->GetState() != Player::STATE::DEAD)
		{
			//�ŏ��̈��͕K�����
			if (dirVector.x == 0.0f && dirVector.z == 0.0f)
			{
				dirVector.x = player->GetPos().x - myPos.x;
				dirVector.z = player->GetPos().z - myPos.z;
			}

			//���̃v���C���[�Ɣ�r���Ă��߂������ق����Ƃ�(�����Ȃ��ɂƂ��Ă������D��)
			if (fabsf(dirVector.x + dirVector.z) >
				fabsf((player->GetPos().x - myPos.x) + (player->GetPos().z - myPos.z)))
			{
				dirVector.x = player->GetPos().x - myPos.x;
				dirVector.z = player->GetPos().z - myPos.z;
			}
		}
	}

	//������ɂ���Ƃ��͑��葤�������čU��
	if (dirVector.x == 0 || dirVector.z == 0)
	{
		//�O��ƍ��E�̂ǂ����ɋ߂���(�����Ȃ獶�E�D��)
		if (fabsf(dirVector.x) >= fabsf(dirVector.z))
		{
			//���E�̕����ɋ߂�
			if (dirVector.x > 0)
			{
				//�E
				dir = Utility::DIR_3D::RIGHT;
			}
			else
			{
				//��
				dir = Utility::DIR_3D::LEFT;
			}
		}
		else
		{
			//�O��̕����ɋ߂�
			if (dirVector.z > 0)
			{
				//�O
				dir = Utility::DIR_3D::FRONT;
			}
			else
			{
				//��
				dir = Utility::DIR_3D::BACK;
			}
		}
	}
	else
	{
		//�O��ƍ��E�̂ǂ����ɋ߂���(�����Ȃ獶�E�D��)
		if (fabsf(dirVector.x) <= fabsf(dirVector.z))
		{
			//���E�̕����ɋ߂�
			if (dirVector.x > 0)
			{
				//�E
				dir = Utility::DIR_3D::RIGHT;
			}
			else
			{
				//��
				dir = Utility::DIR_3D::LEFT;
			}

			//�s�悪���Ă���Ȃ�㉺��������߂Â�
			if (IsNextTileBroken(myPos, dir))
			{
				if (dirVector.z > 0)
				{
					//�O
					dir = Utility::DIR_3D::FRONT;
				}
				else
				{
					//��
					dir = Utility::DIR_3D::BACK;
				}
			}
		}
		else
		{
			//�O��̕����ɋ߂�
			if (dirVector.z > 0)
			{
				//�O
				dir = Utility::DIR_3D::FRONT;
			}
			else
			{
				//��
				dir = Utility::DIR_3D::BACK;
			}

			//�s�悪���Ă���Ȃ獶�E��������߂Â�
			if (IsNextTileBroken(myPos, dir))
			{
				if (dirVector.x > 0)
				{
					//�E
					dir = Utility::DIR_3D::RIGHT;
				}
				else
				{
					//��
					dir = Utility::DIR_3D::LEFT;
				}
			}
		}
	}

	return dir;
}

const bool SceneGame::AimPlayerAttack(const VECTOR _pos, const CommonData::TYPE _num, const Utility::DIR_3D _dir)
{
	//�����̈ʒu
	auto myPos = _pos;
	//�����̃v���C���[�ԍ�
	auto num = _num;
	//�����̕���
	auto dir = _dir;

	//�Ώۃv���C���[�ւ̕����x�N�g��
	VECTOR dirVector = Utility::VECTOR_ZERO;

	for (auto player : player_)
	{
		//�����Ƃ��ꂽ�v���C���[�Ƃ͔�r���Ȃ�
		if (num != player->GetCharaNum() && player->GetState() != Player::STATE::DEAD)
		{
			//�ŏ��̈��͕K�����
			if (dirVector.x == 0.0f && dirVector.z == 0.0f)
			{
				dirVector.x = player->GetPos().x - myPos.x;
				dirVector.z = player->GetPos().z - myPos.z;
			}

			//���̃v���C���[�Ɣ�r���Ă��߂������ق����Ƃ�(�����Ȃ��ɂƂ��Ă������D��)
			if (fabsf(dirVector.x + dirVector.z) >
				fabsf((player->GetPos().x - myPos.x) + (player->GetPos().z - myPos.z)))
			{
				dirVector.x = player->GetPos().x - myPos.x;
				dirVector.z = player->GetPos().z - myPos.z;
			}
		}
	}

	//������ɂ���Ƃ��͑��葤�������čU��
	if (dirVector.x == 0 || dirVector.z == 0)
	{
		//�O��ƍ��E�̂ǂ����ɋ߂���(�����Ȃ獶�E�D��)
		if (fabsf(dirVector.x) >= fabsf(dirVector.z))
		{
			//���E�̕����ɋ߂�
			if (dirVector.x > 0)
			{
				//�E
				if (dir == Utility::DIR_3D::RIGHT)
				{
					return true;
				}
			}
			else
			{
				//��
				if (dir == Utility::DIR_3D::LEFT)
				{
					return true;
				}
			}
		}
		else
		{
			//�O��̕����ɋ߂�
			if (dirVector.z > 0)
			{
				//�O
				if (dir == Utility::DIR_3D::FRONT)
				{
					return true;
				}
			}
			else
			{
				//��
				if (dir == Utility::DIR_3D::BACK)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void SceneGame::TileNowBreak(const CommonData::TYPE _charaNum)
{
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			if (stage_->GetTileAttackPlayer(tx, tz) == _charaNum)
			{
				//�U����̑҂����Ԃ�0.1f�ɂ���	(0���Ɣ��������Ă��Ȃ�����)
				stage_->SetAtkCnt(tx, tz, 0.00001f);
			}
		}
	}
}

const bool SceneGame::IsNarrowStage(void)
{
	return rule_->GetIsNarrowStage();
}

const int SceneGame::Pos2TileAddX(const float _posX)
{
	return static_cast<int>((Stage::STAGE_HSIZE_X + _posX) / Stage::TILE_SIZE_X);
}

const int SceneGame::Pos2TileAddZ(const float _posZ)
{
	return static_cast<int>((Stage::STAGE_HSIZE_Z + _posZ) / Stage::TILE_SIZE_Z);
}

const float SceneGame::TileAdd2PosX(const int _tileAddX)
{
	return static_cast<float>((_tileAddX * Stage::TILE_SIZE_X + Stage::TILE_HSIZE_X) - Stage::STAGE_HSIZE_X);
}

const float SceneGame::TileAdd2PosZ(const int _tileAddZ)
{
	return static_cast<float>((_tileAddZ * Stage::TILE_SIZE_Z + Stage::TILE_HSIZE_Z) - Stage::STAGE_HSIZE_Z);
}

void SceneGame::EndGame(void)
{
	//����
	int rank = 0;

	for (auto player : player_)
	{
		//�v���C���[���Ƃ̏��ʂ��Q��
		rank = player->GetRank();

		//��ʂ����܂��Ă�Ȃ�
		if (rank == 1)
		{
			//�I��
			break;
		}
	}
	
	//1�ʂ����܂����Ȃ�I��
	if (rank == 1)
	{
		for (auto p : player_)
		{
			//���ʂ𑗂�
			CommonData::GetInstance().SetRank(p->GetRank());
		}

		//�Q�[���̏�Ԃ�END�ɂ���
		ChangeGameState(GAME_STATE::END);
	}
}

void SceneGame::AppearItem(void)
{
	//�X�V����
	for (auto item : item_)
	{
		item->Update();
	}

	//�L���ȃA�C�e���̐�
	int aliveItem = 0;

	//�L���ȃA�C�e���̐��𐔂���
	for (auto i : item_)
	{
		if (i->GetIsAlive())
		{
			aliveItem++;
		}
	}

	//�L���ȃA�C�e���̐����ő吔�ȏ�ɂȂ�����
	if (aliveItem >= ItemBase::MAX_NUM)
	{
		//�o���Ԋu�J�E���^�̏�����
		itemInterval_ = 0;

		//����ȏ�o�������Ȃ�
		return;
	}

	//�A�C�e���̏o���J�E���g���I���܂�
	if (itemInterval_ < ItemBase::APPEAR_INTERVAL)
	{
		//�A�C�e���̏o���Ԋu�J�E���g��i�߂�
		itemInterval_++;

		//�������I����
		return;
	}

	//�L���ȃA�C�e���z��ԍ����擾
	ItemBase* item = GetValidItem();

	//�A�C�e���̐���
	GenerateItem(item);

	//�A�C�e���̏o���Ԋu�J�E���^�̏�����
	itemInterval_ = 0;
}

ItemBase* SceneGame::GetValidItem(void)
{
	//�����_���Ɏ�ނ����߂�
	ItemBase::ITEM_TYPE itemType = static_cast<ItemBase::ITEM_TYPE>(GetRand(static_cast<int>(ItemBase::ITEM_TYPE::MAX) - 1));

	size_t size = item_.size();
	for (int i = 0; i < size; i++)
	{
		//�����g���Ă��Ȃ��z�񂪂���Ȃ�
		if (!item_[i]->GetIsAlive() && !item_[i]->GetIsEffective())
		{
			//������Ă���
			item_[i]->Release();
			delete item_[i];
			item_[i] = nullptr;

			//�㏑�����čė��p
			switch (itemType)
			{
				//�n���}�[
			case ItemBase::ITEM_TYPE::HAMMER:
				item_[i] = new ItemHammer(this);
				break;

				//�u�[�c
			case ItemBase::ITEM_TYPE::BOOTS:
				item_[i] = new ItemBoots(this);
				break;
			}

			return item_[i];
		}
	}

	//�V�����U���p�ɔz���ǉ�
	ItemBase* item = nullptr;

	switch (itemType)
	{
		//�n���}�[
	case ItemBase::ITEM_TYPE::HAMMER:
		item = new ItemHammer(this);
		break;

		//�u�[�c
	case ItemBase::ITEM_TYPE::BOOTS:
		item = new ItemBoots(this);
		break;
	}

	item_.push_back(item);

	return item;
}

void SceneGame::GenerateItem(ItemBase* _item)
{
	//�^�C���̏ꏊ
	bool tileNum[Stage::TILE_NUM][Stage::TILE_NUM];
	
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//�v���C���[�̗����Ă���Ƃ���ɂ͏o�����Ȃ��悤�ɂ���
			for (auto player : player_)
			{
				//�Q�ƃv���C���[�̗����Ă���ꏊ
				if (player->GetPos().x == Stage::TILE_SIZE_X * (tx - Stage::TILE_HNUM)
					&& player->GetPos().z == Stage::TILE_SIZE_Z * (tz - Stage::TILE_HNUM))
				{
					//�����Ă���Ȃ���̍��W���Q��
					if (player->GetIsWalk())
					{
						//�[����͎Q�Ƃ��Ȃ�
						if (!IsCollisionStageEnd(player->GetPos(), player->GetDir()))
						{
							switch (player->GetDir())
							{
							case Utility::DIR_3D::FRONT:

								tileNum[tz + 1][tx] = false;

								break;

							case Utility::DIR_3D::RIGHT:

								tileNum[tz][tx + 1] = false;

								break;

							case Utility::DIR_3D::BACK:

								tileNum[tz - 1][tx] = false;

								break;

							case Utility::DIR_3D::LEFT:

								tileNum[tz][tx - 1] = false;

								break;
							}
						}
						//�[�Ȃ猻�ݍ��W��false�ɂ���
						else
						{
							tileNum[tz][tx] = false;
						}
					}
					else
					{
						tileNum[tz][tx] = false;
					}
				}
			}

			if (stage_->GetTileState(tx, tz) == Stage::TILE_STATE::PREEND
				|| stage_->GetTileState(tx, tz) == Stage::TILE_STATE::END
				)
			{
				tileNum[tz][tx] = false;
			}
		}
	}

	//�����_���ňʒu����
	int randZ = GetRand(Stage::TILE_NUM - 1);
	int randX = GetRand(Stage::TILE_NUM - 1);

	//�����_���ɑI�΂ꂽ�ꏊ�������\�ȏꏊ��
	if (tileNum[randZ][randX])
	{
		//�I�΂ꂽ�ꏊ�Ő�������
		_item->Init(
			{
				(randX * Stage::TILE_SIZE_X) + Stage::TILE_HSIZE_X - Stage::STAGE_HSIZE_X
				,ItemBase::RADIUS
				,(randZ * Stage::TILE_SIZE_Z) + Stage::TILE_HSIZE_Z - Stage::STAGE_HSIZE_Z
			}
		);
	}
	else
	{
		//������x��蒼��
		GenerateItem(_item);
	}
}

void SceneGame::PlayerGotItem(Player* _player, ItemBase* _item)
{
	for (auto item : item_)
	{
		if (item->GetFollow() == _player)
		{
			item->EffectEnd();
		}
	}

	//�A�C�e���̎��
	auto itemType = _item->GetItemType();

	//�A�C�e���̌��ʏ����J�n
	_item->SetIsEffective(true);

	//�A�C�e�����Ƀv���C���[�̏���n��
	_item->SetFollow(_player);
}
