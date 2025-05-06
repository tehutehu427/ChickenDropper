#pragma once

#include"../Scene/SceneGame.h"
#include"../Common/Vector2F.h"

class RuleBase
{
public:
	//�萔
	//-------------------------
	
	//1�b(�~���b�P��)
	static constexpr int ONE_SEC = 1000;

	//�^�C�}�[�̑傫���{��
	static constexpr float TIME_SIZE_PER = 5.0f / 3.0f;

	//�J�E���g�̑傫��
	static constexpr int TIME_SIZE = 50;

	//�J�E���g�̑���
	static constexpr int TIME_THICKNESS = 10;

	//�X�R�A�̑傫��
	static constexpr int SCORE_SIZE = 25;

	//�X�R�A�̑���
	static constexpr int SCORE_THICKNESS = 8;

	//�b������
	static constexpr int SEC_LIMIT = 59;
	static constexpr int SEC_2_MIN = 60;

	//�X�e�[�^�X���̑傫��
	static constexpr int STATE_SIZE_X = 200;
	static constexpr int STATE_SIZE_Y = 50;

	//�X�e�[�^�X���̉�ʂ���̈ʒu
	static constexpr int STATE_POS_X = 230;
	static constexpr int STATE_POS_Y = 50;

	//�X�e�[�^�X���̋�����
	static constexpr int STATE_DIS = 100;

	//�J�E���^���l�������X�e�[�^�X���̋���
	static constexpr int STATE_DIS_TO_TIME = 180;

	//�͈͏k���x���摜�̑傫��
	static constexpr int NARROW_INFO_SIZE_X = 600;
	static constexpr int NARROW_INFO_SIZE_Y = 200;

	//�͈͏k���x���̏o�Ă��鎞��
	static constexpr float NARROW_INFO_TIME = Utility::DEFAULT_FPS * 5.0f;

	//�����o�[�֐�
	//-------------------------

	//�f�t�H���g�R���X�g���N�^
	RuleBase(SceneGame* parent);
	//�f�X�g���N�^
	virtual ~RuleBase(void);

	//����������
	virtual bool Init(void);
	//�X�V����
	virtual void Update(void);
	//�`�揈��
	virtual void Draw(void);
	//�������
	virtual bool Release(void);

	/// <summary>
	/// �X�e�[�W�k���̃t���O��Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns>�k���̗L������(true:�k������)</returns>
	const bool GetIsNarrowStage(void);

protected:
	//�����o�[�ϐ�
	//-------------------------

	//�Q�[���V�[���̃|�C���^
	SceneGame* sceneGame_;

	DATEDATA timeLimit_;		//�������ԃJ�E���g

	int timeFont_;				//���Ԃ̃t�H���g
	int timeFontSize_;			//���Ԃ̃t�H���g�̑傫��
	int timeFontThickness_;		//���Ԃ̃t�H���g�̑���

	int scoreFont_;				//�X�R�A�̃t�H���g
	int scoreFontSize_;			//�X�R�A�̃t�H���g�̑傫��
	int scoreFontThickness_;	//�X�R�A�̃t�H���g�̑���

	int secCount_;				//�b�������p

	//�X�e�[�^�X��
	int stateImages_[static_cast<int>(CommonData::TYPE::P4)];

	//�X�e�[�^�X�̍��W
	Vector2F statePos_;

	//�X�e�[�W�k���̃t���O(true:�k������)
	bool isNarrowStage_;

	//�k���x���p
	int narrowInfoImage_;
	float narrowInfoPos_;
	float narrowInfoTime_;

	//���ʂ̏�����
	void InitRank(void);
};

