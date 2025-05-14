#pragma once

#include"ItemBase.h"

class ItemBoots : public ItemBase
{
public:

	static constexpr float SPEED_MULTI = 2.0f;	//移動速度の倍率

	//コンストラクタ
	ItemBoots(SceneGame* parent);

	//パラメータ設定
	void SetParam(void)override;
	
	//効果ごとの更新処理
	void EffectUpdate(void)override;

	//効果ごとの終了処理
	void EffectEnd(void)override;
};

