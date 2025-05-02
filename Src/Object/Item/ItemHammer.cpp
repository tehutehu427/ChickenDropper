#include"../../Scene/SceneGame.h"
#include"../Player.h"
#include "ItemHammer.h"

ItemHammer::ItemHammer(SceneGame* parent) : ItemBase(parent)
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

void ItemHammer::SetParam(void)
{
	itemType_ = ITEM_TYPE::HAMMER;

	color_ = NodyUtility::COLOR_GREEN;
}

void ItemHammer::EffectUpdate(void)
{
	sceneGame_->TileNowBreak(follow_->GetCharaNum());
}

void ItemHammer::EffectEnd(void)
{
	ItemBase::EffectEnd();
}
