#pragma once

class Vector2;

class Vector2F
{
public:
	//�����p�[�֐�
	//-------------------------
	
	//�f�t�H���g�R���X�g���N�^
	Vector2F(void);

	//�R���X�g���N�^
	Vector2F(float ix, float iy);

	//�f�X�g���N�^
	~Vector2F(void);

	//Vector2F�^����Vector2�^�ɕύX
	Vector2 ToVector2(void)const;

	//�����o�[�ϐ�
	float x;
	float y;

private:

};