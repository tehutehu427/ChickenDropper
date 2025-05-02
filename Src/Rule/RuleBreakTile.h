#pragma once
#include"RuleBase.h"

class RuleBreakTile : public RuleBase
{
public:
	//�萔
	//-------------------

	//��������	
	static constexpr DATEDATA TIME_LIMIT = { 0,0,0,0,1,30 };

	//�X�e�[�W�k���J�n����
	static constexpr DATEDATA NARROW_LIMIT = { 0,0,0,0,0,30 };

	//�X�R�A�ƃ^�C���C���[�W�̋���
	static constexpr int SCORE_2_TILE_DIS = 15;

	//�����o�[�֐�
	//-------------------

	//�R���X�g���N�^
	RuleBreakTile(SceneGame* parent);

	//����������
	bool Init(void) override;
	//�X�V����
	void Update(void) override;
	//�`�揈��
	void Draw(void) override;

private:

	//�j�󂵂��^�C�����摜
	int tileNumImage_;
	
	/// <summary>
	/// �󂵂��^�C���̐����r
	/// </summary>
	/// <param name="_player">��r����v���C���[</param>
	void BrokeTimeNumCompare(Player* _player);

	/// <summary>
	/// �󂵂��^�C���̐�����������r
	/// </summary>
	/// <param name="_player">��r����v���C���[</param>
	void IsSameBrokeTileNum(Player* _player);
};