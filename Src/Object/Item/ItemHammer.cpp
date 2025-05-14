#include"../../Scene/SceneGame.h"
#include"../Player.h"
#include "ItemHammer.h"

ItemHammer::ItemHammer(SceneGame* parent) : ItemBase(parent)
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
	color_ = Utility::COLOR_BLACK;
}

void ItemHammer::SetParam(void)
{
	itemType_ = ITEM_TYPE::HAMMER;

	color_ = Utility::COLOR_GREEN;
}

void ItemHammer::EffectUpdate(void)
{
	sceneGame_->TileNowBreak(follow_->GetCharaNum());
}

void ItemHammer::EffectEnd(void)
{
	ItemBase::EffectEnd();
}
