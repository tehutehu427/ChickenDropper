#include"../../Utility/Utility.h"
#include"../Player.h"
#include "ItemBase.h"

ItemBase::ItemBase(SceneGame* parent)
{
	sceneGame_ = parent;

	model_ = -1;
	pos_ = Utility::VECTOR_ZERO;
	rot_ = Utility::VECTOR_ZERO;
	scale_ = Utility::VECTOR_ONE;
	isAlive_ = false;
	isEffective_ = false;
	effectTime_ = 0.0f;
	itemType_ = ITEM_TYPE::HAMMER;
	follow_ = nullptr;
	color_ = Utility::COLOR_WHITE;
}

ItemBase::~ItemBase()
{
}

void ItemBase::Init(VECTOR _pos)
{
	//変数初期化
	pos_ = _pos;
	rot_ = Utility::VECTOR_ZERO;
	scale_ = Utility::VECTOR_ONE;
	isAlive_ = true;
	isEffective_ = false;
	effectTime_ = 0.0f;

	//アイテム別の初期化
	SetParam();
}

void ItemBase::Update(void)
{
	if (!isEffective_ && follow_ == nullptr)
	{
		return;
	}

	//効果時間が終わったら
	if (effectTime_ > EFFECT_TIME || follow_->GetState() == Player::STATE::DEAD)
	{
		//元に戻す
		EffectEnd();
	}
	else if (follow_ != nullptr && isEffective_)
	{
		//効果時間カウンタ
		effectTime_++;

		//プレイヤーに追従
		pos_ = follow_->GetPos();
				
		//効果発動
		EffectUpdate();
	}
}

void ItemBase::Draw(void)
{
	//存在していなかったら
	if (!isAlive_ && !isEffective_)
	{
		return;
	}
	//フィールドにある
	else if (isAlive_)
	{
		DrawSphere3D(pos_, RADIUS, DIV_NUM, color_, color_, true);
	}
	//プレイヤーに取得されている
	else if (isEffective_)
	{
		DrawSphere3D(pos_, PLAYER_FOLLOW, PLAYER_GOT_DIV_NUM, color_, color_, false);
	}
}

void ItemBase::Release(void)
{
	follow_ = nullptr;
}

void ItemBase::EffectEnd(void)
{
	DeleteItem();
}

const VECTOR ItemBase::GetPos(void)
{
	return pos_;
}

const bool ItemBase::GetIsAlive(void)
{
	return isAlive_;
}

const bool ItemBase::GetIsEffective(void)
{
	return isEffective_;
}

void ItemBase::SetIsEffective(const bool _isEffective)
{
	isEffective_ = _isEffective;
}

const ItemBase::ITEM_TYPE ItemBase::GetItemType(void)
{
	return itemType_;
}

const Player* ItemBase::GetFollow(void)
{
	return follow_;
}

void ItemBase::SetFollow(Player* _followPlayer)
{
	follow_ = _followPlayer;
}

void ItemBase::KillItem(void)
{
	isAlive_ = false;
}

void ItemBase::DeleteItem(void)
{
	//終える
	effectTime_ = 0.0f;
	isAlive_ = false;
	isEffective_ = false;
	follow_ = nullptr;
}
