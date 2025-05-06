#pragma once

#include<DxLib.h>
#include"../../Utility/Utility.h"

class SceneGame;
class Player;

class ItemBase
{
public:
	//�A�C�e���̎��	
	enum class ITEM_TYPE
	{
		HAMMER	//�n���}�[
		,BOOTS	//�u�[�c
		,MAX
	};

	//�萔
	//----------------------------

	static constexpr float RADIUS = 24.0f;					//���a

	static constexpr int MAX_NUM = 3;						//�A�C�e���̏o���ő吔

	static constexpr int APPEAR_INTERVAL_SEC = 20;												//�o���Ԋu(�b)
	static constexpr float APPEAR_INTERVAL = APPEAR_INTERVAL_SEC * Utility::DEFAULT_FPS;	//�o���Ԋu

	static constexpr float EFFECT_TIME = 5.0f * Utility::DEFAULT_FPS;				//���ʎ���
	
	//�����o�[�֐�
	//----------------------------
	
	//�R���X�g���N�^
	ItemBase(SceneGame* parent);

	//�f�X�g���N�^
	virtual ~ItemBase();

	virtual void Init(VECTOR _pos);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	//�p�����[�^�ݒ�
	virtual void SetParam(void) = 0;	

	//���ʂ��Ƃ̍X�V����
	virtual void EffectUpdate(void) = 0;

	//���ʂ��Ƃ̏I������
	virtual void EffectEnd(void);

	//�A�C�e���̈ʒu��Ԃ�
	const VECTOR GetPos(void);

	//�A�C�e���̐�������
	const bool GetIsAlive(void);

	//�A�C�e���̌��ʂ̗L������
	const bool GetIsEffective(void);

	//�A�C�e���̌��ʂ̗L����������߂�
	void SetIsEffective(const bool _isEffective);

	//�A�C�e���̎�ނ�Ԃ�
	const ITEM_TYPE GetItemType(void);

	//�Ǐ]�v���C���[��Ԃ�
	const Player* GetFollow(void);

	//�Ǐ]����v���C���[�����߂�
	void SetFollow(Player* _followPlayer);

	//�A�C�e���̎��̂̍폜
	void KillItem(void);

	//�A�C�e���̏���
	void DeleteItem(void);

protected:

	//�C���X�^���X

	//�V�[���Q�[��
	SceneGame* sceneGame_;
	
	//�Ǐ]
	Player* follow_;


	//���f��
	int model_;

	//���W
	VECTOR pos_;

	//��]
	VECTOR rot_;

	//�傫��
	VECTOR scale_;

	//�A�C�e���̎��
	ITEM_TYPE itemType_;

	//��������
	bool isAlive_;

	//���ʂ̗L������
	bool isEffective_;

	//���ʎ��ԃJ�E���^
	float effectTime_;	

	//�F�i���j
	unsigned int color_;
};

