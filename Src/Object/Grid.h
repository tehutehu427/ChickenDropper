#pragma once

class Grid
{
public:
	//�萔
	//--------------------------
	//���̒���
	static constexpr float LEN = 1200.0f;

	//���̒����̔���
	static constexpr float HLEN = LEN / 2.0f;

	//���̊Ԋu
	static constexpr float TERM = 50.0f;

	//���̐�
	static constexpr int NUM = static_cast<int>(LEN / TERM);

	//���̐��̔���
	static constexpr int HNUM = NUM / 2;

	//���̔��a
	static constexpr float RADIUS = 15.0f;

	//���̖ʐ�
	static constexpr int DIV_NUM = 10;

	//�����o�[�֐�
	//--------------------------
	
	//�f�t�H���g�R���X�g���N�^
	Grid(void);						
	//�f�X�g���N�^
	~Grid(void);					

	//����������
	bool Init(void);
	//�X�V����
	void Update(void);
	//�`�揈��
	void Draw(void);
	//�������
	bool Release(void);

};