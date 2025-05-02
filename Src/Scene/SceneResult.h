#pragma once

#include<vector>
#include"../Common/CommonData.h"
#include"SceneBase.h"

class SoundManager;

class SceneResult : public SceneBase
{
public:
	//�����o�[�֐�
	//-------------------

	//����������
	bool Init(void) override;
	//�X�V����
	void Update(void) override;
	//�`�揈��
	void Draw(void) override;
	//�������
	bool Release(void) override;

private:

	//�A�j���[�V�����ԍ�
	enum class ANIM_NUM
	{
		NONE
		, IDLE		//�ҋ@
		, WALK		//����
		, ATTACK	//�U��
		, FALL		//����
		, RANK1ST	//1��
		, RANK2ND	//2��
		, RANK3RD	//3��
		, RANK4TH	//4��
	};

	//���艹��
	static constexpr int CLICK_VOLUME = 70;

	//�J�[�e������
	static constexpr int CURTAIN_VOLUME = 100;

	//�v���C���[�̑傫���{��
	static constexpr float P_SCALE = 0.8f;

	//�v���C���[�̃A�j���[�V�������x
	static constexpr float SPEED_ANIM = 60.0f;

	//�v���C���[���m�̋���
	static constexpr float PL_DIS = 60.0f;

	//���ʉ摜�֌W
	static constexpr int RANK_SIZE = 45;				//�ԍ��̑傫��
	static constexpr int RANK_DIS_Y = 20;				//�ԍ��ƃv���C���[��Y����
	static constexpr int RANK_DIS_Z = 35;				//�ԍ��ƃv���C���[��Z����

	static constexpr VECTOR CURTAIN_INIT_POS = { 0.0f, 78.0f, -160.0f };	//���䖋�̊J�n���W�p

	static constexpr float CURTAIN_SHIFT_POS = 30.0f;	//���䖋�̊J�n���W�̂��炵�p

	static constexpr float CURTAIN_SPEED = 1.0f;		//���䖋�̈ړ���

	static constexpr float CURTAIN_FEEDBACK_TIME = NodyUtility::DEFAULT_FPS * 5.0f;	//���䖋�̃t�B�[�h�o�b�N����܂ł̎���

	//�C���X�^���X
	//---------------

	//�T�E���h�̃C���X�^���X
	SoundManager* sound_;

	int bgImage_;			//�w�i�摜
	int bgCurtainImage_;	//�w�i�̕��䖋�摜
	int curtainImage_;		//���䖋�̉摜
	bool curtainFlag_;		//���䖋�̊J�t���O(true:�J��)

	VECTOR curtainPos_;		//���䖋�̍��W

	float curtainFeedBackTime_;	//���䖋�̃t�B�[�h�o�b�N����܂ł̎��ԃJ�E���g

	std::vector<int> rank_;		//����
		
	//���U���g�̃v���C���[
	int pModel_[static_cast<int>(CommonData::TYPE::P4)];	//�v���C���[�̃��f��
	VECTOR pScl_;											//�v���C���[�̑傫��
	VECTOR pRot_;											//�v���C���[�̉�]
	VECTOR pPos_[static_cast<int>(CommonData::TYPE::P4)];	//�v���C���[�̈ʒu
	float stepAnim_[static_cast<int>(CommonData::TYPE::P4)];		//�Đ����̃A�j���[�V��������
	float speedAnim_[static_cast<int>(CommonData::TYPE::P4)];		//�A�j���[�V�������x
	float animTotalTime_[static_cast<int>(CommonData::TYPE::P4)];	//�A�j���[�V�����̑��Đ�����
	int animAttachNo_[static_cast<int>(CommonData::TYPE::P4)];		//�A�^�b�`���Ă���A�j���[�V����

	int  rankImage_[static_cast<int>(CommonData::TYPE::P4)];		//�v���C���[�ԍ��̕\���p
};

