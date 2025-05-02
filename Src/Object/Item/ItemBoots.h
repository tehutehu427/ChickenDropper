#pragma once

#include"ItemBase.h"

class ItemBoots : public ItemBase
{
public:

	//コンストラクタ
	ItemBoots(SceneGame* parent);

	//パラメータ設定
	void SetParam(void)override;
	
	//効果ごとの更新処理
	void EffectUpdate(void)override;

	//効果ごとの終了処理
	void EffectEnd(void)override;
};

