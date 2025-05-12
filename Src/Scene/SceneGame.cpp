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

//初期化処理
bool SceneGame::Init(void)
{
	//インスタンス生成
	//------------------------------------------

	//エフェクトのインスタンス生成
	effect_ = new EffectManager();
	effect_->EffectInit();

	//サウンドのインスタンス生成
	sound_ = new SoundManager();
	sound_->SoundInit();

	//サウンドのロード
	sound_->BGMLoad(SoundManager::BGM_TYPE::BATTLE_FIRST_HALF);
	sound_->BGMLoad(SoundManager::BGM_TYPE::BATTLE_SECOND_HALF);

	//バトルBGM
	sound_->PlayBGM(SoundManager::BGM_TYPE::BATTLE_FIRST_HALF, DX_PLAYTYPE_LOOP, BATTLE_FIRST_BGM_VOLUME, false);

	//ステージのインスタンス生成
	stage_ = new Stage;
	if (stage_->Init(this) == false)
	{
		OutputDebugString("Stageの初期化失敗\n");
		return false;
	}

	//ゲームの状態の初期化
	ChangeGameState(GAME_STATE::PRE_START);

	//ルール決め
	SetRule();

	//アイテムの出現間隔
	itemInterval_ = 0;

	//一度だけ実行
	Update();

	return true;
}

void SceneGame::Update(void)
{
	SceneManager& scnMng = SceneManager::GetInstance();

	switch (gameState_)
	{
	case SceneGame::GAME_STATE::PRE_START:
		//ゲーム開始の準備
		ChangeGameState(GAME_STATE::START);
		break;

	case SceneGame::GAME_STATE::START:
		//ゲーム開始
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
		//ゲームの処理を止める
		scnMng.ChangeScene(SceneManager::SCENE_ID::RESULT, true);
		return;
	}

	//BGM
	//範囲縮小しているなら
	if (rule_->GetIsNarrowStage())
	{
		//前半戦BGMを止めて
		sound_->StopBGM(SoundManager::BGM_TYPE::BATTLE_FIRST_HALF);

		//後半戦BGMを流し始める
		sound_->PlayBGM(SoundManager::BGM_TYPE::BATTLE_SECOND_HALF, DX_PLAYTYPE_LOOP, BATTLE_SECOND_BGM_VOLUME,false);
	}

	//ステージの更新
	stage_->Update();

	//ルールごとの処理
	rule_->Update();

	//プレイヤーの初期設定と更新
	ProcessPlayer();

	//アイテムの生成
	AppearItem();

	//プレイヤーごとの処理
	for (auto player : player_)
	{
		for (int tz = 0; tz < Stage::TILE_NUM; tz++)
		{
			for (int tx = 0; tx < Stage::TILE_NUM; tx++)
			{
				//プレイヤーと床の当たり判定
				//---------------------------------------
				//プレイヤーが生存している	かつ　ダメージ状態ではない　かつ　落下アニメーション中ではない
				if (player->GetState() != Player::STATE::DEAD && player->GetInvincible() > Player::INVINCIBLE && player->GetAnimNum() != Player::ANIM_NUM::FALL)
				{
					//プレイヤーがタイルの中央にいる　かつ　タイルが壊れ状態
					if ((player->GetPos().x) == (stage_->GetPos(tx, tz).x)
						&& (player->GetPos().z) == (stage_->GetPos(tx, tz).z)
						&& (stage_->GetTileState(tx, tz) == Stage::TILE_STATE::BROKEN
							|| stage_->GetTileState(tx, tz) == Stage::TILE_STATE::END)
							)
					{
						//攻撃してきたプレイヤーを保存
						player->PreDamage(stage_->GetTileAttackPlayer(tx, tz));
					}
				}

				for (auto attack : player->GetAttack())
				{
					//攻撃と床の当たり判定
					//---------------------------------------

					//攻撃中　かつ　床が通常状態の時	
					if (attack->GetIsAlive() && stage_->GetTileState(tx, tz) == Stage::TILE_STATE::NORMAL)
					{
						//攻撃がタイルと重なった
						if ((attack->GetPos().x) <= (stage_->GetPos(tx, tz).x + Stage::TILE_HSIZE_X)
							&& (attack->GetPos().x) >= (stage_->GetPos(tx, tz).x - Stage::TILE_HSIZE_X)
							&& (attack->GetPos().z) <= (stage_->GetPos(tx, tz).z + Stage::TILE_HSIZE_Z)
							&& (attack->GetPos().z) >= (stage_->GetPos(tx, tz).z - Stage::TILE_HSIZE_Z))
						{
							//タイルを攻撃状態にする
							stage_->TileSwap(tx, tz, Stage::TILE_STATE::ATTACK);

							//攻撃したプレイヤーを保存
							stage_->TileAttackPlayer(tx, tz, attack->GetCharaNum());
						}
					}
					//すでに攻撃状態だった場合
					else if (attack->GetIsAlive() && stage_->GetTileState(tx, tz) == Stage::TILE_STATE::ATTACK)
					{
						//攻撃がタイルと重なった
						if ((attack->GetPos().x) <= (stage_->GetPos(tx, tz).x + Stage::TILE_HSIZE_X)
							&& (attack->GetPos().x) >= (stage_->GetPos(tx, tz).x - Stage::TILE_HSIZE_X)
							&& (attack->GetPos().z) <= (stage_->GetPos(tx, tz).z + Stage::TILE_HSIZE_Z)
							&& (attack->GetPos().z) >= (stage_->GetPos(tx, tz).z - Stage::TILE_HSIZE_Z))
						{
							//攻撃したプレイヤー情報のみ上書き
							stage_->TileAttackPlayer(tx, tz, attack->GetCharaNum());
						}
					}
				}

				//終わらせた床とアイテムの判定
				for (auto item : item_)
				{
					//アイテムが取得されていない状態で終了した床と当たった
					if (item->GetIsAlive()
						&& (item->GetPos().x == stage_->GetPos(tx, tz).x)
						&& (item->GetPos().z == stage_->GetPos(tx, tz).z)
						&& (stage_->GetTileState(tx,tz) == Stage::TILE_STATE::END
						|| stage_->GetTileState(tx,tz) == Stage::TILE_STATE::BROKEN)
						)
					{
						//アイテムを消す
						item->DeleteItem();
					}
				}
			}
		}

		for (auto attack : player->GetAttack())
		{
			//攻撃の更新
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
				//アイテムを削除する
				item->KillItem();

				//プレイヤーのアイテム取得処理
				PlayerGotItem(player, item);
			}
		}
	}
}

void SceneGame::Draw(void)
{
	//ステージの描画
	stage_->Draw();

	//ルールごとの描画
	rule_->Draw();

	//ゲーム開始準備中　又は　ゲーム終了後なら
	if (gameState_ == GAME_STATE::PRE_START || gameState_ == GAME_STATE::END)
	{
		return;
	}

	//アイテムの描画
	for (auto item : item_)
	{
		//アイテムの描画
		item->Draw();
	}

	for (auto player : player_)
	{
		//プレイヤーの描画
		player->Draw();
	
		for (auto attack : player->GetAttack())
		{
			//攻撃の描画
			attack->Draw();
		}
	}
}

bool SceneGame::Release(void)
{
	//アイテムの数分解放
	for (auto item : item_)
	{
		//アイテムの解放
		item->Release();
		delete item;
		item = nullptr;
	}

	//プレイヤーの数を取得
	for (auto player : player_)
	{
		//プレイヤーの解放
		player->Release();
		delete player;
		player = nullptr;
	}

	//ルールの解放
	rule_->Release();
	delete rule_;
	rule_ = nullptr;

	//ステージの解放
	delete stage_;
	stage_ = nullptr;

	//サウンドの解放
	sound_->SoundRelease();
	delete sound_;
	sound_ = nullptr;

	//エフェクトの解放
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
		//前側の端の移動制限
		if (_pos.z >= Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z)
		{
			return true;
		}
		break;

	case Utility::DIR_3D::RIGHT:
		//右端の移動制限
		if (_pos.x >= Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X)
		{
			return true;
		}
		break;

	case Utility::DIR_3D::BACK:
		//後ろ側の端の移動制限
		if (_pos.z <= -Stage::STAGE_HSIZE_Z + Stage::TILE_HSIZE_Z)
		{
			return true;
		}
		break;

	case Utility::DIR_3D::LEFT:
		//左端の移動制限
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

	//プレイヤーのタイルによる移動制限
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//隣のタイルが壊れ状態(ステージより外は判定しない)
			//--------------------------------
			if (!IsCollisionStageEnd(_pos, _dir))
			{
				switch (_dir)
				{
				case Utility::DIR_3D::FRONT:
					//前
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) + 1)
						== Stage::TILE_STATE::BROKEN)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::RIGHT:
					//右
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) + 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::BROKEN)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::BACK:
					//後ろ
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) - 1)
						== Stage::TILE_STATE::BROKEN)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::LEFT:
					//左
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

	//プレイヤーのタイルによる移動制限
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//隣のタイルが攻撃状態(ステージより外は判定しない)
			//--------------------------------
			if (!IsCollisionStageEnd(_pos, _dir))
			{
				switch (_dir)
				{
				case Utility::DIR_3D::FRONT:
					//前
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) + 1)
						== Stage::TILE_STATE::ATTACK)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::RIGHT:
					//右
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) + 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::ATTACK)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::BACK:
					//後ろ
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) - 1)
						== Stage::TILE_STATE::ATTACK)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::LEFT:
					//左
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

	//プレイヤーのタイルによる移動制限
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//自分の立っているタイルが攻撃状態
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

	//プレイヤーのタイルによる移動制限
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//隣のタイルが攻撃状態(ステージより外は判定しない)
			//--------------------------------
			if (!IsCollisionStageEnd(_pos, _dir))
			{
				switch (_dir)
				{
				case Utility::DIR_3D::FRONT:
					//前
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) + 1)
						== Stage::TILE_STATE::PREEND)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::RIGHT:
					//右
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) + 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::PREEND)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::BACK:
					//後ろ
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) - 1)
						== Stage::TILE_STATE::PREEND)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::LEFT:
					//左
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

	//プレイヤーのタイルによる移動制限
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//自分の立っているタイルが攻撃状態
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

	//プレイヤーのタイルによる移動制限
	//-------------------------------------------------------------
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//隣のタイルが攻撃状態(ステージより外は判定しない)
			//--------------------------------
			if (!IsCollisionStageEnd(_pos, _dir))
			{
				switch (_dir)
				{
				case Utility::DIR_3D::FRONT:
					//前
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) + 1)
						== Stage::TILE_STATE::END)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::RIGHT:
					//右
					if (stage_->GetTileState(Pos2TileAddX(_pos.x) + 1, Pos2TileAddZ(_pos.z))
						== Stage::TILE_STATE::END)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::BACK:
					//後ろ
					if (stage_->GetTileState(Pos2TileAddX(_pos.x), Pos2TileAddZ(_pos.z) - 1)
						== Stage::TILE_STATE::END)
					{
						return true;
					}
					break;

				case Utility::DIR_3D::LEFT:
					//左
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
	//攻撃してきた相手がいないなら
	if (_charaNum == CommonData::TYPE::NONE)
	{
		//何もしない
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
	//プレイヤー番号に対応した配列番号
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
		//プレイヤーの更新
		player->Update();
	}

	//インスタンス生成用
	Player* playerIns;

	auto& common = CommonData::GetInstance();

	//対戦人数格納用
	CommonData::BATTLE_PATTERN bPat_;
	bPat_ = common.GetBattlePattern();

	//対戦人数用
	int bNum = 0;

	//キャラの人数を取得
	size_t size = player_.size();

	//人数合わせ
	switch (bPat_)
	{
		//2人
	case CommonData::BATTLE_PATTERN::P1C1:
	case CommonData::BATTLE_PATTERN::P2:
		bNum = static_cast<int>(CommonData::TYPE::P2);
		break;

		//3人
	case CommonData::BATTLE_PATTERN::P1C2:
	case CommonData::BATTLE_PATTERN::P2C1:
	case CommonData::BATTLE_PATTERN::P3:
		bNum = static_cast<int>(CommonData::TYPE::P3);
		break;

		//4人
	case CommonData::BATTLE_PATTERN::P1C3:
	case CommonData::BATTLE_PATTERN::P2C2:
	case CommonData::BATTLE_PATTERN::P3C1:
	case CommonData::BATTLE_PATTERN::P4:
		bNum = static_cast<int>(CommonData::TYPE::P4);
		break;
	}

	//対戦人数分の配列を生成
	while (size < bNum)
	{
		//プレイヤーのインスタンス生成
		playerIns = new Player();

		//プレイヤーの人数
		int pNum = 1;

		//キャラごとにプレイヤーかCPUかの属性を与える
		//------------------------------
		//生成した人数分飛ばす
		pNum += static_cast<int>(size);		
		if (pNum <= static_cast<int>(common.GetPlayerNum()))
		{
			playerIns->SetCharaJudge(Player::CHARA_JUDGE::PLAYER);
		}
		else
		{
			playerIns->SetCharaJudge(Player::CHARA_JUDGE::CPU);
		}

		//初期化処理
		//キャラ番号で初期位置も決める
		if (size == 0)
		{
			//左上に生成
			playerIns->Init(this, { stage_->GetPos(0,Stage::TILE_NUM - 1).x
									,Player::RADIUS
									,stage_->GetPos(0,Stage::TILE_NUM - 1).z } 
			, CommonData::TYPE::P1);
		}
		else if (size == 1)
		{
			//右上に生成
			playerIns->Init(this, { stage_->GetPos(Stage::TILE_NUM - 1,Stage::TILE_NUM - 1).x
									,Player::RADIUS
									,stage_->GetPos(Stage::TILE_NUM - 1,Stage::TILE_NUM - 1).z }
			, CommonData::TYPE::P2);
		}
		else if (size == 2)
		{
			//左下に生成
			playerIns->Init(this, { stage_->GetPos(0,0).x
									,Player::RADIUS
									,stage_->GetPos(0,0).z }
			, CommonData::TYPE::P3);
		}
		else if (size == 3)
		{
			//右下に生成
			playerIns->Init(this, { stage_->GetPos(Stage::TILE_NUM - 1, 0).x
									, Player::RADIUS
									, stage_->GetPos(Stage::TILE_NUM - 1, 0).z }
			, CommonData::TYPE::P4);
		}

		//配列の追加
		player_.push_back(playerIns);

		//キャラの人数取得
		size = player_.size();
	}

	//難易度配列が存在するなら代入
	if (common.GetDifficultySize() > 0)
	{
		for (int p = size - 1; p > 0; p--)
		{
			//難易度設定
			player_[p]->SetDifficulty(CommonData::GetInstance().PopDifficulty());
		}
	}
}

void SceneGame::ReviavalPlayer(Player* _player)
{
	//床
	bool tileNum[Stage::TILE_NUM][Stage::TILE_NUM];

	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//床がない場所は復活不可
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

	//各プレイヤーがいる場所には復活ができないようにする
	for (auto player : player_)
	{
		//自分とは参照しない
		if (player->GetCharaNum() != _player->GetCharaNum())
		{
			//歩いているなら一つ先の座標を参照
			if (player->GetIsWalk())
			{
				//端より先は参照しない
				if (!IsCollisionStageEnd(player->GetPos(), player->GetDir()))
				{
					switch (player->GetDir())
					{
					case Utility::DIR_3D::FRONT:
						//移動後かどうか
						if (static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z) + 1 >= Stage::TILE_NUM)
						{
							//立っている位置を移動不可に
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						else
						{
							//移動先を出現不可に
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z) + 1]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						break;

					case Utility::DIR_3D::RIGHT:
						//移動後かどうか
						if (static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X) + 1 >= Stage::TILE_NUM)
						{
							//立っている位置を移動不可に
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						else
						{
							//移動先を出現不可に
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X) + 1] = false;
						}
						break;

					case Utility::DIR_3D::BACK:
						//移動後かどうか
						if (static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z) - 1 < 0)
						{
							//立っている位置を移動不可に
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						else
						{
							//移動先を出現不可に
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z) - 1]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						break;

					case Utility::DIR_3D::LEFT:
						//移動後かどうか
						if (static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X) - 1 < 0)
						{
							//立っている位置を移動不可に
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
						}
						else
						{
							//移動先を出現不可に
							tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
								[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X) - 1] = false;
						}
					
						break;
					}
				}
				//端なら現在座標をfalseにする
				else
				{
					//立っている位置を移動不可に
					tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
						[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
				}
			}
			else
			{
				//立っている位置を移動不可に
				tileNum[static_cast<int>((player->GetPos().z + Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
					[static_cast<int>((player->GetPos().x + Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X) / Stage::TILE_SIZE_X)] = false;
			}
		}
	}

	//ランダムの場所
	int randZ = GetRand(Stage::TILE_NUM - 1);
	int randX = GetRand(Stage::TILE_NUM - 1);

	//出現可能か
	if (tileNum[randZ][randX])
	{
		//座標セット
		_player->SetPos(
			{ Stage::TILE_SIZE_X * (randX - Stage::TILE_HNUM)
				,Player::RADIUS
			 ,Stage::TILE_SIZE_Z * (randZ - Stage::TILE_HNUM) }
		);
	}
	else
	{
		//やり直し
		ReviavalPlayer(_player);
	}
}

const bool SceneGame::HitP2P(const VECTOR _pos, const Utility::DIR_3D _dir, const CommonData::TYPE _num)
{
	//参照プレイヤーの位置
	auto myPos = _pos;
	//参照プレイヤーの向き
	auto myDir = _dir;
	//参照プレイヤーのプレイヤー番号
	auto myNum = _num;
	//参照プレイヤーの動きたいタイル
	bool myTileNum[Stage::TILE_NUM][Stage::TILE_NUM];

	//相手プレイヤーごとのタイル判定
	bool oppTileNum[Stage::TILE_NUM][Stage::TILE_NUM];

	//初期化
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
		//自分とやられたプレイヤーとは比較しない
		if (myNum != player->GetCharaNum() && player->GetState() != Player::STATE::DEAD)
		{
			//相手プレイヤーが歩いているなら移動先を参照
			if (player->GetIsWalk())
			{
				switch (player->GetDir())
				{
				case Utility::DIR_3D::FRONT:
					//端より先は参照しない
					if (player->GetPos().z + Stage::TILE_SIZE_Z < Stage::STAGE_HSIZE_Z + Stage::TILE_HSIZE_Z)
					{
						//一マス　＋　歩幅分の場所をとる
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
					//端より先は参照しない
					if (player->GetPos().x + Stage::TILE_SIZE_X < Stage::STAGE_HSIZE_X + Stage::TILE_HSIZE_X)
					{
						//一マス　＋　歩幅分の場所をとる
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
					//端より先は参照しない
					if (player->GetPos().z - Stage::TILE_SIZE_Z > -Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z)
					{
						//一マス　＋　歩幅分の場所をとる
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
					//端より先は参照しない
					if (player->GetPos().x - Stage::TILE_SIZE_X > -Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X)
					{
						//一マス　＋　歩幅分の場所をとる
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
					//相手プレイヤーが歩いていない　又は　端にいるなら立っている場所を参照
					if (!player->GetIsWalk()
						|| player->GetPos().z + Stage::TILE_SIZE_Z >= Stage::STAGE_HSIZE_Z + Stage::TILE_HSIZE_Z
						|| player->GetPos().x + Stage::TILE_SIZE_X >= Stage::STAGE_HSIZE_X + Stage::TILE_HSIZE_X
						|| player->GetPos().z - Stage::TILE_SIZE_Z <= -Stage::STAGE_HSIZE_Z - Stage::TILE_HSIZE_Z
						|| player->GetPos().x - Stage::TILE_SIZE_X <= -Stage::STAGE_HSIZE_X - Stage::TILE_HSIZE_X)
					{
						if (player->GetPos().x == Stage::TILE_SIZE_X * (tx - Stage::TILE_HNUM)
							&& player->GetPos().z == Stage::TILE_SIZE_Z * (tz - Stage::TILE_HNUM))
						{
							//相手プレイヤーのいる位置を求める
							oppTileNum[tz][tx] = true;
						}
					}
				}
			}
		}
	}

	//端より先は参照しない
	if (!IsCollisionStageEnd(myPos, myDir))
	{
		//参照プレイヤーの動きたい位置を求める
		switch (myDir)
		{
		case Utility::DIR_3D::FRONT:
			//移動先が当たった
			myTileNum[static_cast<int>(((myPos.z + Stage::STAGE_HSIZE_Z) / Stage::TILE_SIZE_Z) + 1.0f)]
				[static_cast<int>((myPos.x + Stage::STAGE_HSIZE_X) / Stage::TILE_SIZE_X)]
			= true;

			break;

		case Utility::DIR_3D::RIGHT:
			//移動先が当たった
			myTileNum[static_cast<int>((myPos.z + Stage::STAGE_HSIZE_Z) / Stage::TILE_SIZE_Z)]
				[static_cast<int>(((myPos.x + Stage::STAGE_HSIZE_X) / Stage::TILE_SIZE_X) + 1.0f)]
			= true;

			break;

		case Utility::DIR_3D::BACK:
			//移動先が当たった
			myTileNum[static_cast<int>(((myPos.z + Stage::STAGE_HSIZE_Z) / Stage::TILE_SIZE_Z) - 1.0f)]
				[static_cast<int>((myPos.x + Stage::STAGE_HSIZE_X) / Stage::TILE_SIZE_X)]
			= true;

			break;

		case Utility::DIR_3D::LEFT:
			//移動先が当たった
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
				//自分とやられたプレイヤーとは比較しない
				if (myNum != player->GetCharaNum() && player->GetState() != Player::STATE::DEAD)
				{
					if (myTileNum[tz][tx] && oppTileNum[tz][tx])
					{
						//衝突した
						return true;
					}
				}
			}
		}
	}

	//衝突しなかった
	return false;
}

const Utility::DIR_3D SceneGame::AimPlayer(const VECTOR _pos, const CommonData::TYPE _num)
{
	//返す方向
	Utility::DIR_3D dir;

	//自分の位置
	VECTOR myPos = _pos;
	//自分のプレイヤー番号
	CommonData::TYPE num = _num;

	//対象プレイヤーへの方向ベクトル
	VECTOR dirVector = Utility::VECTOR_ZERO;

	for (auto player : player_)
	{
		//自分とやられたプレイヤーとは比較しない
		if (num != player->GetCharaNum() && player->GetState() != Player::STATE::DEAD)
		{
			//最初の一回は必ずやる
			if (dirVector.x == 0.0f && dirVector.z == 0.0f)
			{
				dirVector.x = player->GetPos().x - myPos.x;
				dirVector.z = player->GetPos().z - myPos.z;
			}

			//他のプレイヤーと比較してより近かったほうをとる(同じなら先にとっていた方優先)
			if (fabsf(dirVector.x + dirVector.z) >
				fabsf((player->GetPos().x - myPos.x) + (player->GetPos().z - myPos.z)))
			{
				dirVector.x = player->GetPos().x - myPos.x;
				dirVector.z = player->GetPos().z - myPos.z;
			}
		}
	}

	//直線状にいるときは相手側を向いて攻撃
	if (dirVector.x == 0 || dirVector.z == 0)
	{
		//前後と左右のどっちに近いか(同じなら左右優先)
		if (fabsf(dirVector.x) >= fabsf(dirVector.z))
		{
			//左右の方向に近い
			if (dirVector.x > 0)
			{
				//右
				dir = Utility::DIR_3D::RIGHT;
			}
			else
			{
				//左
				dir = Utility::DIR_3D::LEFT;
			}
		}
		else
		{
			//前後の方向に近い
			if (dirVector.z > 0)
			{
				//前
				dir = Utility::DIR_3D::FRONT;
			}
			else
			{
				//後
				dir = Utility::DIR_3D::BACK;
			}
		}
	}
	else
	{
		//前後と左右のどっちに近いか(同じなら左右優先)
		if (fabsf(dirVector.x) <= fabsf(dirVector.z))
		{
			//左右の方向に近い
			if (dirVector.x > 0)
			{
				//右
				dir = Utility::DIR_3D::RIGHT;
			}
			else
			{
				//左
				dir = Utility::DIR_3D::LEFT;
			}

			//行先が壊れているなら上下方向から近づく
			if (IsNextTileBroken(myPos, dir))
			{
				if (dirVector.z > 0)
				{
					//前
					dir = Utility::DIR_3D::FRONT;
				}
				else
				{
					//後
					dir = Utility::DIR_3D::BACK;
				}
			}
		}
		else
		{
			//前後の方向に近い
			if (dirVector.z > 0)
			{
				//前
				dir = Utility::DIR_3D::FRONT;
			}
			else
			{
				//後
				dir = Utility::DIR_3D::BACK;
			}

			//行先が壊れているなら左右方向から近づく
			if (IsNextTileBroken(myPos, dir))
			{
				if (dirVector.x > 0)
				{
					//右
					dir = Utility::DIR_3D::RIGHT;
				}
				else
				{
					//左
					dir = Utility::DIR_3D::LEFT;
				}
			}
		}
	}

	return dir;
}

const bool SceneGame::AimPlayerAttack(const VECTOR _pos, const CommonData::TYPE _num, const Utility::DIR_3D _dir)
{
	//自分の位置
	auto myPos = _pos;
	//自分のプレイヤー番号
	auto num = _num;
	//自分の方向
	auto dir = _dir;

	//対象プレイヤーへの方向ベクトル
	VECTOR dirVector = Utility::VECTOR_ZERO;

	for (auto player : player_)
	{
		//自分とやられたプレイヤーとは比較しない
		if (num != player->GetCharaNum() && player->GetState() != Player::STATE::DEAD)
		{
			//最初の一回は必ずやる
			if (dirVector.x == 0.0f && dirVector.z == 0.0f)
			{
				dirVector.x = player->GetPos().x - myPos.x;
				dirVector.z = player->GetPos().z - myPos.z;
			}

			//他のプレイヤーと比較してより近かったほうをとる(同じなら先にとっていた方優先)
			if (fabsf(dirVector.x + dirVector.z) >
				fabsf((player->GetPos().x - myPos.x) + (player->GetPos().z - myPos.z)))
			{
				dirVector.x = player->GetPos().x - myPos.x;
				dirVector.z = player->GetPos().z - myPos.z;
			}
		}
	}

	//直線状にいるときは相手側を向いて攻撃
	if (dirVector.x == 0 || dirVector.z == 0)
	{
		//前後と左右のどっちに近いか(同じなら左右優先)
		if (fabsf(dirVector.x) >= fabsf(dirVector.z))
		{
			//左右の方向に近い
			if (dirVector.x > 0)
			{
				//右
				if (dir == Utility::DIR_3D::RIGHT)
				{
					return true;
				}
			}
			else
			{
				//左
				if (dir == Utility::DIR_3D::LEFT)
				{
					return true;
				}
			}
		}
		else
		{
			//前後の方向に近い
			if (dirVector.z > 0)
			{
				//前
				if (dir == Utility::DIR_3D::FRONT)
				{
					return true;
				}
			}
			else
			{
				//後
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
				//攻撃後の待ち時間を0.1fにする	(0だと反応させていないため)
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
	//順位
	int rank = 0;

	for (auto player : player_)
	{
		//プレイヤーごとの順位を参照
		rank = player->GetRank();

		//一位が決まってるなら
		if (rank == 1)
		{
			//終了
			break;
		}
	}
	
	//1位が決まったなら終了
	if (rank == 1)
	{
		for (auto p : player_)
		{
			//順位を送る
			CommonData::GetInstance().SetRank(p->GetRank());
		}

		//ゲームの状態をENDにする
		ChangeGameState(GAME_STATE::END);
	}
}

void SceneGame::AppearItem(void)
{
	//更新処理
	for (auto item : item_)
	{
		item->Update();
	}

	//有効なアイテムの数
	int aliveItem = 0;

	//有効なアイテムの数を数える
	for (auto i : item_)
	{
		if (i->GetIsAlive())
		{
			aliveItem++;
		}
	}

	//有効なアイテムの数が最大数以上になったら
	if (aliveItem >= ItemBase::MAX_NUM)
	{
		//出現間隔カウンタの初期化
		itemInterval_ = 0;

		//それ以上出現させない
		return;
	}

	//アイテムの出現カウントが終わるまで
	if (itemInterval_ < ItemBase::APPEAR_INTERVAL)
	{
		//アイテムの出現間隔カウントを進める
		itemInterval_++;

		//処理を終える
		return;
	}

	//有効なアイテム配列番号を取得
	ItemBase* item = GetValidItem();

	//アイテムの生成
	GenerateItem(item);

	//アイテムの出現間隔カウンタの初期化
	itemInterval_ = 0;
}

ItemBase* SceneGame::GetValidItem(void)
{
	//ランダムに種類を決める
	ItemBase::ITEM_TYPE itemType = static_cast<ItemBase::ITEM_TYPE>(GetRand(static_cast<int>(ItemBase::ITEM_TYPE::MAX) - 1));

	size_t size = item_.size();
	for (int i = 0; i < size; i++)
	{
		//もう使っていない配列があるなら
		if (!item_[i]->GetIsAlive() && !item_[i]->GetIsEffective())
		{
			//解放してから
			item_[i]->Release();
			delete item_[i];
			item_[i] = nullptr;

			//上書きして再利用
			switch (itemType)
			{
				//ハンマー
			case ItemBase::ITEM_TYPE::HAMMER:
				item_[i] = new ItemHammer(this);
				break;

				//ブーツ
			case ItemBase::ITEM_TYPE::BOOTS:
				item_[i] = new ItemBoots(this);
				break;
			}

			return item_[i];
		}
	}

	//新しい攻撃用に配列を追加
	ItemBase* item = nullptr;

	switch (itemType)
	{
		//ハンマー
	case ItemBase::ITEM_TYPE::HAMMER:
		item = new ItemHammer(this);
		break;

		//ブーツ
	case ItemBase::ITEM_TYPE::BOOTS:
		item = new ItemBoots(this);
		break;
	}

	item_.push_back(item);

	return item;
}

void SceneGame::GenerateItem(ItemBase* _item)
{
	//タイルの場所
	bool tileNum[Stage::TILE_NUM][Stage::TILE_NUM];
	
	for (int tz = 0; tz < Stage::TILE_NUM; tz++)
	{
		for (int tx = 0; tx < Stage::TILE_NUM; tx++)
		{
			//プレイヤーの立っているところには出現しないようにする
			for (auto player : player_)
			{
				//参照プレイヤーの立っている場所
				if (player->GetPos().x == Stage::TILE_SIZE_X * (tx - Stage::TILE_HNUM)
					&& player->GetPos().z == Stage::TILE_SIZE_Z * (tz - Stage::TILE_HNUM))
				{
					//歩いているなら一つ先の座標を参照
					if (player->GetIsWalk())
					{
						//端より先は参照しない
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
						//端なら現在座標をfalseにする
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

	//ランダムで位置決め
	int randZ = GetRand(Stage::TILE_NUM - 1);
	int randX = GetRand(Stage::TILE_NUM - 1);

	//ランダムに選ばれた場所が生成可能な場所か
	if (tileNum[randZ][randX])
	{
		//選ばれた場所で生成する
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
		//もう一度やり直し
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

	//アイテムの種類
	auto itemType = _item->GetItemType();

	//アイテムの効果処理開始
	_item->SetIsEffective(true);

	//アイテム側にプレイヤーの情報を渡す
	_item->SetFollow(_player);
}
