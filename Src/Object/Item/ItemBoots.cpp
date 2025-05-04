#include"../../Scene/SceneGame.h"
#include"../Player.h"
#include "ItemBoots.h"

ItemBoots::ItemBoots(SceneGame* parent) : ItemBase(parent)
{
	sceneGame_ = parent;

	model_ = 0;
	pos_ = Utility::VECTOR_ZERO;
	rot_ = Utility::VECTOR_ZERO;
	scale_ = Utility::VECTOR_ONE;
	isAlive_ = false;
	isEffective_ = false;
	effectTime_ = 0.0f;
	itemType_ = ITEM_TYPE::HAMMER;
	follow_ = nullptr;
	color_ = Utility::COLOR_BLACK;
}

void ItemBoots::SetParam(void)
{
	itemType_ = ITEM_TYPE::BOOTS;

	color_ = Utility::COLOR_RED;
}

void ItemBoots::EffectUpdate(void)
{
	follow_->SetSpeed(Player::SPEED_NORMAL * 2.0f);

	follow_->RunMoveInterval();
}

void ItemBoots::EffectEnd(void)
{
	follow_->SetSpeed(Player::SPEED_NORMAL);

	ItemBase::EffectEnd();
}
