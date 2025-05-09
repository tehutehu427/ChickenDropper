#pragma once
#include<vector>

class Player;

class PlayerManager
{
public:

	//�R���X�g���N�^
	PlayerManager(void);
	//�f�X�g���N�^
	~PlayerManager(void);

	//������
	void Init(void);
	//�X�V
	void Update(void);
	//�`��
	void Draw(void);
	//���
	void Release(void);

private:

	//�v���C���[�̃C���X�^���X�p
	std::vector<Player*> player_;

};

