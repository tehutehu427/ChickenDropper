#pragma once
#include<DxLib.h>

class Camera
{
public:
	//�����o�[�֐�
	//-------------------
	
	//�f�t�H���g�R���X�g���N�^
	Camera(void);					
	//�f�X�g���N�^
	~Camera(void);					

	//����������
	bool Init(void);					
	//�X�V����
	void Update(void);					

	//�J�����ݒ�(���t���[�����s)
	void SetBeforeDraw(void);			
	//�`�揈��
	void Draw(void);					
	//�������(��{�I�ɍŌ�̂P�񂾂����s)
	bool Release(void);					

	/// <summary>
	/// �J�������[�N�̐ݒ�
	/// </summary>
	/// <param name="_pos">�J�����ʒu</param>
	/// <param name="_angles">�J�����p�x(���W�A���ϊ��s�v)</param>
	void SetCamerawork(const VECTOR _pos, const VECTOR _angles);

private:
	//�J�����̈ʒu
	VECTOR pos_;

	//�J�����̊p�x
	VECTOR angles_;
};