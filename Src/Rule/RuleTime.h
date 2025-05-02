#pragma once

#include"../Common/CommonData.h"
#include"RuleBase.h"

class Player;

class RuleTime : public RuleBase
{
public:
	//�萔
	//-------------------

	//��������	
	static constexpr DATEDATA TIME_LIMIT = { 0,0,0,0,2,0 };	

	//�X�e�[�W�k���J�n����
	static constexpr DATEDATA NARROW_LIMIT = { 0,0,0,0,1,0 };

	//�X�R�A�ƃ^�C���C���[�W�̋���
	static constexpr int SCORE_2_KILLSCORE_DIS = 15;

	//�����o�[�֐�
	//-------------------

	//�R���X�g���N�^
	RuleTime(SceneGame* parent);

	//����������
	bool Init(void) override;
	//�X�V����
	void Update(void) override;
	//�`�揈��
	void Draw(void) override;

private:

	//�L���X�R�A�̉摜
	int killScoreImage_;

	/// <summary>
	/// �v���C���[�̃X�R�A��r
	/// </summary>
	/// <param name="_player">��r����v���C���[</param>
	void ScoreCompare(Player* _player);		

	/// <summary>
	/// �X�R�A���������ǂ����m���߂�
	/// </summary>
	/// <param name="_player">��r����v���C���[</param>
	void IsSameScore(Player* _player);
};