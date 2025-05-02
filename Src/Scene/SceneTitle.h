//#pragma once

#include"../Common/Vector2F.h"
#include"SceneBase.h"

class SoundManager;

class SceneTitle : public SceneBase
{
public:
	//�萔
	//-------------------

	//����(SE)
	//-----------------

	//���艹
	static constexpr int CLICK_VOLUME = 70;

	//�����̈ʒu�p
	static constexpr int CHAR_POS_X = 300;	//������X�ړ��p
	static constexpr int CHAR_POS_Y = 200;	//������Y�ړ��p

	//�����o�[�֐�
	//-------------------

	//����������
	bool Init(void) override;		
	//�X�V����
	void Update(void) override;		
	//�`�揈��
	void Draw(void) override;		
	//�������
	bool Release(void) override;	

private:
	//�|�C���^
	//--------------------------

	//�T�E���h�̃C���X�^���X
	SoundManager* sound_;

	//�����o�[�ϐ�
	//--------------------------
	//�w�i�摜�̊i�[�̈�
	int bgImage_;

	//�_�摜
	int cloudImage_;

	//�_�摜�̍��W
	Vector2F cloudPos_;

	//�^�C�g�����S
	int logoImage_;
};