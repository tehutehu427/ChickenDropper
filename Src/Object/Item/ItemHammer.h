#pragma once
#include "ItemBase.h"
class ItemHammer :
    public ItemBase
{
public:
    //�R���X�g���N�^
    ItemHammer(SceneGame* parent);

    //�p�����[�^�ݒ�
    void SetParam(void)override;
   
    //���ʂ��Ƃ̍X�V����
    void EffectUpdate(void)override;

    //���ʂ��Ƃ̏I������
    void EffectEnd(void)override;
};

