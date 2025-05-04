#pragma once

#include<DxLib.h>

class SceneBase
{
public:
	//�����o�[�֐�
	//-------------------------
	
	//�f�t�H���g�R���X�g���N�^
	SceneBase(void);					
	//�f�X�g���N�^
	virtual ~SceneBase(void) = 0;			

	//����������
	virtual bool Init(void) = 0;					
	//�X�V����
	virtual void Update(void) = 0;					
	//�`�揈��
	virtual void Draw(void) = 0;					
	//�������
	virtual bool Release(void) = 0;
};