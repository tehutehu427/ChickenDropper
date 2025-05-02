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
	virtual ~SceneBase(void);			

	//����������
	virtual bool Init(void);					
	//�X�V����
	virtual void Update(void);					
	//�`�揈��
	virtual void Draw(void);					
	//�������
	virtual bool Release(void);
};