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

//デフォルトコンストラクタ
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

//デストラクタ
Player::~Player(void)
{

}

//初期化処理
bool Player::Init(SceneGame* parent, const VECTOR _pos, const CommonData::TYPE _charaNum)
{
	//インスタンス取得
	ResourceManager& res = ResourceManager::GetInstance();

	//ペアレント
	sceneGame_ = parent;

	//エフェクト
	effect_ = new EffectManager();
	effect_->EffectInit();

	//サウンド
	//--------------------
	se_ = new SoundManager();
	se_->SEInit();

	//サウンドのロード
	se_->SELoad(SoundManager::SE_TYPE::ATTACK);

	//変数の初期化	
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


	//キャラ番号判断
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

	//開始アニメーション
	ChangeAnim(ANIM_NUM::IDLE);
	speedAnim_ = SPEED_ANIM;


	//モデルの位置、大きさ、角度
	MV1SetPosition(model_, pos_);
	MV1SetScale(model_, scale_);
	MV1SetRotationXYZ(model_, rot_);

	return true;
}

//更新処理
void Player::Update(void)
{
	//やられたら処理を止める
	if (state_ == STATE::DEAD)
	{
		return;
	}

	if (numViewTime_ < NUM_VIEW_TIME)
	{
		//番号の表示カウンタ
		numViewTime_++;
	}

	//無敵時間制御
	invincible_++;
	
	//回転制御
	ProcessRot();

	//キャラごとの更新
	switch (charaJudge_)
	{
	case Player::CHARA_JUDGE::PLAYER:
		UpdatePlayer();
		break;
	
	case Player::CHARA_JUDGE::CPU:
		UpdateCPU();
		break;
	}

	//アニメーションの更新処理
	UpdateAnimation();

	//アニメーション
	Animation();

	//状態処理
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

//更新処理(プレイヤー)
void Player::UpdatePlayer(void)
{
	//移動制御
	MovePlayer();

	//攻撃の制御
	ProcessAttackPlayer();
}

//更新処理(CPU)
void Player::UpdateCPU(void)
{
	//移動制御
	MoveCPU();

	//攻撃の制御
	ProcessAttackCPU();
}

//描画処理
void Player::Draw(void)
{
	if (state_ == STATE::DEAD)
	{
		return;
	}
	if (invincible_ >= INVINCIBLE || static_cast<int>(invincible_) % 3 == 0)
	{
		//プレイヤーの描画
		MV1DrawModel(model_);
	}

	if (numViewTime_ < NUM_VIEW_TIME)
	{
		DrawBillboard3D({ pos_.x,pos_.y + NUM_DIS ,pos_.z }, 0.5f, 0.5f, NUM_SIZE, 0.0f, numImage_, true);
	}
}

//解放処理
bool Player::Release(void)
{
	MV1DeleteModel(model_);

	//攻撃の解放
	for (auto attack : attack_)
	{
		attack->Release();
		delete attack;
		attack = nullptr;
	}

	//エフェクトの解放
	effect_->EffectEnd();
	delete effect_;
	effect_ = nullptr;

	//SEの解放
	se_->SoundRelease();
	delete se_;
	se_ = nullptr;

	return true;
}

//移動制御
void Player::MovePlayer(void)
{
	auto& ins = InputManager::GetInstance();

	//移動処理
	//----------------------------------------
	//歩いていないとき かつ 攻撃中ではない　かつ　落下中ではない
	if (!isWalk_ && state_ != STATE::ATTACK && animNum_ != ANIM_NUM::ATTACK && animNum_ != ANIM_NUM::FALL)
	{
		//移動方向設定
		//-------------------------------------------------
		//方向転換用キー
		auto keepButtonType = PAD_INPUT_6;	//Rキー
		auto keepKeyType = KEY_INPUT_X;		//Xキー

		//キーボードの時の入力合わせ
		bool keyboardKeep = (GetJoypadNum() <= 0 && (charaNum_ == CommonData::TYPE::P1 && ins.IsKeyDown(keepKeyType)));

		//前方向

		//Rキーを押しながらだと方向転換
		if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_UP) && (ins.IsJoypadKeyDown(padNum_, keepButtonType) || keyboardKeep))
		{
			//方向変更のみ
			dir_ = Utility::DIR_3D::FRONT;
		}
		else if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_UP) && !isWalk_)
		{
			dir_ = Utility::DIR_3D::FRONT;

			//移動先のタイルが壊れていない　かつ　移動先にプレイヤーがいない　かつ　端ではない
			if (!sceneGame_->IsNextTileBroken(pos_, dir_) 
				&& !sceneGame_->IsNextTileEnd(pos_, dir_) 
				&& !sceneGame_->HitP2P(pos_, dir_, charaNum_) 
				&& !sceneGame_->IsCollisionStageEnd(pos_, dir_))
			{
				pos_.z += speed_;
				isWalk_ = true;

				//歩きモーション開始
				ChangeAnim(ANIM_NUM::WALK);
			}
		}

		//右方向

		//Rキーを押しながらだと方向転換
		if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_RIGHT) && !isWalk_ && (ins.IsJoypadKeyDown(padNum_, keepButtonType) || keyboardKeep))
		{
			//方向変更のみ
			dir_ = Utility::DIR_3D::RIGHT;
		}
		else if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_RIGHT) && !isWalk_)
		{
			dir_ = Utility::DIR_3D::RIGHT;
			//移動先のタイルが壊れていない　かつ　移動先にプレイヤーがいない　かつ　端ではない
			if (!sceneGame_->IsNextTileBroken(pos_, dir_) 
				&& !sceneGame_->IsNextTileEnd(pos_, dir_)
				&& !sceneGame_->HitP2P(pos_, dir_, charaNum_) 
				&& !sceneGame_->IsCollisionStageEnd(pos_, dir_))
			{
				pos_.x += speed_;
				isWalk_ = true;

				//歩きモーション開始
				ChangeAnim(ANIM_NUM::WALK);
			}
		}

		//後方向
		
		//Rキーを押しながらだと方向転換
		if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_DOWN) && !isWalk_ && (ins.IsJoypadKeyDown(padNum_, keepButtonType) || keyboardKeep))
		{
			dir_ = Utility::DIR_3D::BACK;
		}
		else if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_DOWN) && !isWalk_)
		{
			dir_ = Utility::DIR_3D::BACK;
			//移動先のタイルが壊れていない　かつ　移動先にプレイヤーがいない　かつ　端ではない
			if (!sceneGame_->IsNextTileBroken(pos_, dir_) 
				&& !sceneGame_->IsNextTileEnd(pos_, dir_) 
				&& !sceneGame_->HitP2P(pos_, dir_, charaNum_) 
				&& !sceneGame_->IsCollisionStageEnd(pos_, dir_))
			{
				pos_.z -= speed_;
				isWalk_ = true;

				//歩きモーション開始
				ChangeAnim(ANIM_NUM::WALK);
			}
		}
		
		//左方向

		//Rキーを押しながらだと方向転換
		if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_LEFT) && !isWalk_ && (ins.IsJoypadKeyDown(padNum_, keepButtonType) || keyboardKeep))
		{
			dir_ = Utility::DIR_3D::LEFT;
		}
		else if (ins.IsJoypadKeyDown(padNum_, PAD_INPUT_LEFT) && !isWalk_)
		{
			dir_ = Utility::DIR_3D::LEFT;
			//移動先のタイルが壊れていない　かつ　移動先にプレイヤーがいない　かつ　端ではない
			if (!sceneGame_->IsNextTileBroken(pos_, dir_) 
				&& !sceneGame_->IsNextTileEnd(pos_, dir_) 
				&& !sceneGame_->HitP2P(pos_, dir_, charaNum_) 
				&& !sceneGame_->IsCollisionStageEnd(pos_, dir_))
			{
				pos_.x -= speed_;
				isWalk_ = true;

				//歩きモーション開始
				ChangeAnim(ANIM_NUM::WALK);
			}
		}		
	}
	else						//歩いている
	{
		//タイルの真ん中にいるか
		if (!(static_cast<int>(pos_.x) % static_cast<int>(sceneGame_->GetTileSizeX()) == 0
			&& static_cast<int>(pos_.z) % static_cast<int>(sceneGame_->GetTileSizeZ()) == 0))		//真ん中にいない
		{
			//真ん中にいない間移動する
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
		else	//真ん中にいる
		{
			//止まる
			isWalk_ = false;
		}
	}
}

void Player::MoveCPU(void)
{
	//歩いていないとき かつ 攻撃中ではない　かつ　落下中ではない
	if (!isWalk_ && state_ != STATE::ATTACK && animNum_ != ANIM_NUM::ATTACK && animNum_ != ANIM_NUM::FALL)
	{
		//自分の立っているタイルが攻撃状態 又は 終了準備状態
		if (sceneGame_->IsThisTileAttack(pos_) || sceneGame_->IsThisTilePreEnd(pos_))
		{			
			//すぐに動く
			RunMoveInterval();
		}
		else
		{
			//間隔をずらす一移動につき一回
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

		//敵の移動間隔カウンタ
		CPUMoveInterVal_++;

		if (difficulty_ == CommonData::DIFFICULTY::EASY && CPUMoveInterVal_ > CPU_MOVE_INTERVAL_EASY
		|| difficulty_ == CommonData::DIFFICULTY::NORMAL && CPUMoveInterVal_ > CPU_MOVE_INTERVAL_NORMAL
		|| difficulty_ == CommonData::DIFFICULTY::HARD && CPUMoveInterVal_ > CPU_MOVE_INTERVAL_HARD
			)
		{
			//相手プレイヤーとの衝突判定と自分の立っているタイルが攻撃状態かの判定
			if (sceneGame_->HitP2P(pos_, sceneGame_->AimPlayer(pos_, charaNum_), charaNum_) 
				&& !sceneGame_->IsThisTileAttack(pos_)
				&& !sceneGame_->IsThisTilePreEnd(pos_))
			{
				//方向変更のみ
				dir_ = sceneGame_->AimPlayer(pos_, charaNum_);
			}
			else
			{
				//CPUの移動判断(一番近いプレイヤーを狙う)
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

				//動くことが可能なら
				if (dir_ != Utility::DIR_3D::MAX)
				{
					//歩いた判定
					isWalk_ = true;

					//歩きモーション開始
					ChangeAnim(ANIM_NUM::WALK);
				}
			}

			//ランダムで間隔を開ける
			randInterval_ = false;

			//攻撃を避ける
			isEndMoveInterval_ = false;

			//移動間隔カウンタ初期化
			CPUMoveInterVal_ = 0.0f;
		}
	}
	else						//歩いている
	{
		//タイルの真ん中にいるか
		if (!(static_cast<int>(pos_.x) % static_cast<int>(sceneGame_->GetTileSizeX()) == 0
			&& static_cast<int>(pos_.z) % static_cast<int>(sceneGame_->GetTileSizeZ()) == 0))		//真ん中にいない
		{
			//真ん中にいない間移動する
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
		else	//真ん中にいる
		{
			//止まる
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
	//攻撃してきたプレイヤーを保存
	attackedPlayer_ = _charaNum;

	//落下アニメーション準備
	ChangeAnim(ANIM_NUM::FALL);
}

void Player::Damage(const int _damage)
{
	//ルール
	auto rule = CommonData::GetInstance().GetRule();

	switch (rule)
	{
	case CommonData::RULE::TIME:
		//減点処理
		killScore_ -= _damage;

		//得点が0未満にならないようにする
		if (killScore_ <= 0)
		{
			killScore_ = 0;
		}
		break;
	
	case CommonData::RULE::LIFE:
		//ダメージを与える
		life_ -= _damage;

		//ライフが０以下になったら生存判定をfalseにする
		if (life_ <= 0)
		{
			//やられた時間をとっておく
			deadTime_ = GetNowCount();

			ChangeState(STATE::DEAD);
			return;
		}
		break;
	
	case CommonData::RULE::BREAK_TILE:
		//タイルの破壊数の値を割合で下げる
		brokeTileNum_ -= brokeTileNum_ / _damage;
		break;
	}

	//復活エフェクト
	effect_->EffectPlay(EffectManager::EFF_TYPE::REVIVAL, pos_, { 8.0f,8.0f,8.0f }, Utility::VECTOR_ZERO);

	//プレイヤー番号の再表示
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

//攻撃の制御(プレイヤー)
void Player::ProcessAttackPlayer(void)
{
	//プレイヤーの攻撃処理
	//-------------------------------
	
	//攻撃アニメーションが終わったら
	if (animTotalTime_ <= stepAnim_ && animNum_ == ANIM_NUM::ATTACK)
	{
		//攻撃の生成
		GenerateAttack();

		StartJoypadVibration(padNum_, 100, 250);
	}

	//プレイヤーが生きている　かつ　プレイヤーが歩いていない　かつ　攻撃中ではない
	if (state_ != STATE::DEAD 
		&& !isWalk_ 
		&& state_ != STATE::ATTACK
		&& animNum_ != ANIM_NUM::ATTACK
		&& animNum_ != ANIM_NUM::FALL
		)
	{
		if (InputManager::GetInstance().IsJoypadKeyPush(padNum_, PAD_INPUT_A) == true)
		{
			//攻撃モーション開始
			ChangeAnim(ANIM_NUM::ATTACK);

			//攻撃状態にする
			ChangeState(STATE::ATTACK);
		}
	}
}

//攻撃の制御(CPU)
void Player::ProcessAttackCPU(void)
{
	//クールタイムカウンタ
	atkCoolDown_++;

	//攻撃アニメーションが終わったら
	if (animTotalTime_ <= stepAnim_ && animNum_ == ANIM_NUM::ATTACK)
	{
		//攻撃の生成
		GenerateAttack();

		//クールタイムリセット
		atkCoolDown_ = 0;
	}

	//攻撃処理
	//-------------------------------
	/*プレイヤーが生きている　かつ　
	プレイヤーが歩いていない　かつ　
	攻撃中ではない(アニメーション)　かつ　
	照準が相手に合っている　かつ　
	攻撃のクールタイムが終わっている　かつ
	落下アニメーション中ではない*/
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
		//攻撃モーション開始
		ChangeAnim(ANIM_NUM::ATTACK);

		//攻撃状態にする
		ChangeState(STATE::ATTACK);

		//クールダウンの初期化
		atkCoolDown_ = 0;
	}
}

void Player::GenerateAttack(void)
{
	//有効な攻撃を取得
	Attack* attack = GetValidAttack();

	//ステージより外では攻撃できないようにする
	if (attack->GetPos().z > -sceneGame_->GetStageHSizeZ() - (sceneGame_->GetTileSizeZ() / 2)		//前端の移動制限
		&& attack->GetPos().x < sceneGame_->GetStageHSizeX() + (sceneGame_->GetTileSizeX() / 2)		//右端の移動制限
		&& attack->GetPos().z < sceneGame_->GetStageHSizeZ() + (sceneGame_->GetTileSizeZ() / 2)		//後ろ端の移動制限
		&& attack->GetPos().x > -sceneGame_->GetStageHSizeX() - (sceneGame_->GetTileSizeX() / 2)	//左端の移動制限
		)
	{
		//攻撃を始める
		switch (dir_)
		{
		case Utility::DIR_3D::FRONT:
			//一マス先に攻撃を生成する
			attack->Init(sceneGame_, this, VAdd(pos_, VGet(0.0f, 0.0f, sceneGame_->GetTileSizeZ())), dir_, charaNum_);
			break;

		case Utility::DIR_3D::RIGHT:
			//一マス先に攻撃を生成する
			attack->Init(sceneGame_, this, VAdd(pos_, VGet(sceneGame_->GetTileSizeX(), 0.0f, 0.0f)), dir_, charaNum_);
			break;

		case Utility::DIR_3D::BACK:
			//一マス先に攻撃を生成する
			attack->Init(sceneGame_, this, VSub(pos_, VGet(0.0f, 0.0f, sceneGame_->GetTileSizeZ())), dir_, charaNum_);
			break;

		case Utility::DIR_3D::LEFT:
			//一マス先に攻撃を生成する
			attack->Init(sceneGame_, this, VSub(pos_, VGet(sceneGame_->GetTileSizeX(), 0.0f, 0.0f)), dir_, charaNum_);
			break;
		}

		//攻撃SE
		se_->PlaySE(SoundManager::SE_TYPE::ATTACK, DX_PLAYTYPE_BACK, 70);
	}
}

//有効な攻撃を取得
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

	//新しい攻撃用に配列を追加
	Attack* attack = new Attack();
	attack_.push_back(attack);

	return attack;

}

//プレイヤーの状態を返す
const Player::STATE Player::GetState(void)
{
	return state_;
}

//プレイヤーの状態変更(_state:変更したい状態)
void Player::ChangeState(const STATE _state)
{
	state_ = _state;
}

//プレイヤーの現在座標を返す
const VECTOR Player::GetPos(void)
{
	return pos_;
}

//プレイヤーの座標を与える
void Player::SetPos(const VECTOR _setPos)
{
	pos_ = _setPos;
}

//プレイヤーの方向を返す
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

	//現在のアニメーションをデタッチする
	MV1DetachAnim(model_,animAttachNo_);

	//新しく持ってきたアニメーションをアタッチする
	animAttachNo_ = MV1AttachAnim(model_,static_cast<int>(animNum_));

	//アニメーション時間の初期化
	stepAnim_ = 0.0f;
	animTotalTime_ = MV1GetAttachAnimTotalTime(model_, animAttachNo_);

	//アニメーションの速度
	switch (animNum_)
	{
	case Player::ANIM_NUM::IDLE:
		speedAnim_ = SPEED_ANIM;
		break;

	case Player::ANIM_NUM::WALK:
		//次のタイルに移動するまでに一巡する
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

//プレイヤーの無敵時間を返す
const float Player::GetInvincible(void)
{
	return invincible_;
}

const Player::CHARA_JUDGE Player::GetCharaJudge(void)
{
	return charaJudge_;
}

//プレイヤーか敵かを与える(_type:プレイヤーか敵かの判断)
void Player::SetCharaJudge(const CHARA_JUDGE _type)
{
	charaJudge_ = _type;
}

const CommonData::TYPE Player::GetCharaNum(void)
{
	return charaNum_;
}

//攻撃のポインタを返す
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
	//移動間隔減少の処理をしていないなら
	if (!isEndMoveInterval_)
	{
		//難易度
		switch (difficulty_)
		{
		case CommonData::DIFFICULTY::EASY:
			//少し間隔を狭める
			CPUMoveInterVal_ = CPU_MOVE_INTERVAL_EASY / 2;

			break;

		case CommonData::DIFFICULTY::NORMAL:
			//間隔を狭める
			CPUMoveInterVal_ = CPU_MOVE_INTERVAL_NORMAL / 1.5f;

			break;

		case CommonData::DIFFICULTY::HARD:
			//すぐに動く
			CPUMoveInterVal_ = CPU_MOVE_INTERVAL_HARD;

			break;
		}
	}

	//移動間隔を狭めた
	isEndMoveInterval_ = true;
}

//CPUの移動判断
const Utility::DIR_3D Player::CPUMoveChack(const Utility::DIR_3D _dir)
{
	//ランダムで方向を決める
	Utility::DIR_3D dir = _dir;

	//動ける方向がなかったら
	if (!isMoveFront_ && !isMoveRight_ && !isMoveBack_ && !isMoveLeft_)
	{
		//隣のタイルが攻撃状態ならそっちに動く
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

		//相手プレイヤーとの衝突判定
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

		//端との衝突判定
		if (sceneGame_->IsCollisionStageEnd(pos_, dir))
		{
			//端の移動制限
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
			//初期化
			isMoveFront_ = true;
			isMoveRight_ = true;
			isMoveBack_ = true;
			isMoveLeft_ = true;

			//動かない
			return Utility::DIR_3D::MAX;
		}
		else
		{
			//方向の決めなおし
			while (!isMoveFront_ && dir == Utility::DIR_3D::FRONT
				|| !isMoveRight_ && dir == Utility::DIR_3D::RIGHT
				|| !isMoveBack_ && dir == Utility::DIR_3D::BACK
				|| !isMoveLeft_ && dir == Utility::DIR_3D::LEFT)
			{
				//空いている方向で決めなおす
				dir = static_cast<Utility::DIR_3D>(GetRand(static_cast<int>(Utility::DIR_3D::LEFT)));
			}

			//方向の確定
			return dir;
		}
	}


	//隣のタイルが壊れている、攻撃状態、終了準備状態、終了状態のどれか　又は　端の移動制限に当たった　又は　相手プレイヤーと衝突した
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

	//初期化
	isMoveFront_ = true;
	isMoveRight_ = true;
	isMoveBack_ = true;
	isMoveLeft_ = true;

	//方向の確定
	return dir;
}

void Player::UpdateAnimation(void)
{
	//各アニメーションが終わったら
	if (animNum_ == ANIM_NUM::WALK && !isWalk_
		|| animNum_ == ANIM_NUM::ATTACK && state_ != STATE::ATTACK)
	{
		//立ち状態に戻る
		ChangeAnim(ANIM_NUM::IDLE);
	}
	else if (animNum_ == ANIM_NUM::FALL)
	{
		//落下アニメーション
		FallAnimation();
	}
}

void Player::FallAnimation(void)
{
	//落下中で　かつ　アニメーションが終わってないなら
	if (animNum_ == ANIM_NUM::FALL && animTotalTime_ > stepAnim_)
	{
		//少しづつ落下していく
		pos_.y -= FALL_SPEED;
	}
	//落下アニメーションが終わったら
	else if (animNum_ == ANIM_NUM::FALL && animTotalTime_ <= stepAnim_)
	{
		//無敵時間をリセット
		invincible_ = 0.0f;

		//復活処理
		sceneGame_->ReviavalPlayer(this);

		//ダメージ
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

				//攻撃してきたプレイヤーに得点
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

		//アニメーション終了
		ChangeAnim(ANIM_NUM::IDLE);
	}
}

void Player::Animation(void)
{
	//大きさ、角度、座標の3D制御
	//---------------------------------
	//大きさの設定
	MV1SetPosition(model_, pos_);

	//角度の設定
	MV1SetScale(model_, scale_);

	//位置の設定
	MV1SetRotationXYZ(model_, rot_);

	//アニメーション再生
	//-------------------------------------------

	//アニメーション時間の進行
	stepAnim_ += (speedAnim_ * Utility::DELTA_TIME);
	if (stepAnim_ > animTotalTime_)
	{
		//ループ再生
		stepAnim_ = 0.0f;
	}

	//再生するアニメーション時間の設定
	MV1SetAttachAnimTime(model_, animAttachNo_, stepAnim_);
}
