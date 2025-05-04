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

	//����
	static constexpr int CHAR_POS_X = 300;				//������X�ړ��p
	static constexpr int CHAR_POS_Y = 200;				//������Y�ړ��p
	static constexpr int FONT_SIZE = 45;				//�t�H���g�̑傫��
	static constexpr int FONT_TICKNESS = 5;				//�t�H���g�̑���
	static constexpr int FONT_BLINKING_INTERVAL = 20;	//�t�H���g�̓_�ŊԊu

	//�����o�[�֐�
	//-------------------

	//�R���X�g���N�^
	SceneTitle();

	//�f�X�g���N�^
	~SceneTitle()override;

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