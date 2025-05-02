#pragma once
#include"RuleBase.h"

class RuleLife : public RuleBase
{
public:
	
	//���C�t�p�l���̑傫��
	static constexpr int LIFE_SIZE_X = 25;
	static constexpr int LIFE_SIZE_Y = 40;

	//���C�t�p�l���Ƙg�̍��W���킹
	static constexpr int LIFE_TO_STATE_POS = 10;

	//���C�t�p�l���̊Ԋu
	static constexpr int LIFE_DIS = 3;

	//�X�e�[�W�k���J�n����
	static constexpr DATEDATA NARROW_LIMIT = { 0,0,0,0,2,0 };

	//�����o�[�֐�
	//-------------------

	//�R���X�g���N�^
	RuleLife(SceneGame* parent);

	//����������
	bool Init(void) override;
	//�X�V����
	void Update(void) override;
	//�`�揈��
	void Draw(void) override;

private:

	//���C�t�p�l��
	int lifeImages_[static_cast<int>(CommonData::TYPE::P4)];

	//���Ԍo��
	DATEDATA timeCnt_;

	/// <summary>
	/// ���ꂽ���Ԃ̔�r
	/// </summary>
	/// <param name="_player">��r����v���C���[</param>
	void DeadTimeCompare(Player* _player);

	/// <summary>
	/// ���ꂽ���Ԃ����������r
	/// </summary>
	/// <param name="_player">��r����v���C���[</param>
	void IsSameDeadTime(Player* _player);
};