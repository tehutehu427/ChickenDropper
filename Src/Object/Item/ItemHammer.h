#pragma once
#include "ItemBase.h"
class ItemHammer :
    public ItemBase
{
public:
    //コンストラクタ
    ItemHammer(SceneGame* parent);

    //パラメータ設定
    void SetParam(void)override;
   
    //効果ごとの更新処理
    void EffectUpdate(void)override;

    //効果ごとの終了処理
    void EffectEnd(void)override;
};

