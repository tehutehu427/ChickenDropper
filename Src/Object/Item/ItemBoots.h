#pragma once

#include"ItemBase.h"

class ItemBoots : public ItemBase
{
public:

	//�R���X�g���N�^
	ItemBoots(SceneGame* parent);

	//�p�����[�^�ݒ�
	void SetParam(void)override;
	
	//���ʂ��Ƃ̍X�V����
	void EffectUpdate(void)override;

	//���ʂ��Ƃ̏I������
	void EffectEnd(void)override;
};

