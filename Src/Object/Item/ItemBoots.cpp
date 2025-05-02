#include"../../Scene/SceneGame.h"
#include"../Player.h"
#include "ItemBoots.h"

ItemBoots::ItemBoots(SceneGame* parent) : ItemBase(parent)
{
	sceneGame_ = parent;

	model_ = 0;
	pos_ = NodyUtility::VECTOR_ZERO;
	rot_ = NodyUtility::VECTOR_ZERO;
	scale_ = NodyUtility::VECTOR_ONE;
	isAlive_ = false;
	isEffective_ = false;
	effectTime_ = 0.0f;
	itemType_ = ITEM_TYPE::HAMMER;
	follow_ = nullptr;
	color_ = NodyUtility::COLOR_BLACK;
}

void ItemBoots::SetParam(void)
{
	itemType_ = ITEM_TYPE::BOOTS;

	color_ = NodyUtility::COLOR_RED;
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
