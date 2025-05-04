#pragma once

#include<DxLib.h>
#include<map>
#include<functional>
#include"../Common/Vector2.h"
#include"../Common/Vector2F.h"
#include"../Common/CommonData.h"
#include"SceneBase.h"

class SoundManager;

class SceneSelect : public SceneBase
{
public:
	//�񋓌^
	//-------------------------------

	//���߂鍀��
	enum class SELECT_ITEM
	{
		BATTLE_PATTERN		//�ΐ�l��
		,CHECK_PAD			//�p�b�h�̊m�F
		,DIFFICULTY			//CPU�̋��������߂�
		,RULE				//���[��
	};

	//�萔
	//-------------------------------
	
	//�R���g���[���[�̃o�C�u���[�V�����̋���
	static constexpr int VIB_STR = 80;
	//�R���g���[���[�̃o�C�u���[�V�����̎���(�~���b)
	static constexpr int VIB_TIME = 1 * 1000;

	//�R���g���[���[�̑I����
	static constexpr int SELECT_PAD_P1 = 1;
	static constexpr int SELECT_KEYBOARD = 0;

	//�J�[�\���̑傫��
	static constexpr int CURSOR_SIZE = 10;		

	//�Ŕ̑傫��
	static constexpr int SIGNBOARD_SIZE_X = 1536;
	static constexpr int SIGNBOARD_SIZE_Y = 864;

	//�{�[�h�̑傫��
	static constexpr int BOARD_SIZE_X = 1216;
	static constexpr int BOARD_SIZE_Y = 608;

	//�{�[�h�̍���
	static constexpr int BOARD_HEIGHT = 170;

	//�l���摜�̑傫��
	static constexpr int BATTLE_NUM_SIZE_X = 400;
	static constexpr int BATTLE_NUM_SIZE_Y = 500;

	static constexpr int BATTLE_PAT_SHIFT_POS_X = 148;
	static constexpr int BATTLE_PAT_SHIFT_POS_Y = 60;

	static constexpr int PADNUM_SHIFT_POS_X = 40;
	static constexpr int PADNUM_SHIFT_POS_Y = 50;

	//���摜�̑傫��
	static constexpr float ARROW_DEFAULT_SIZE = 1.0f;
	static constexpr float ARROW_CHANGE_SIZE = 10.0f;
	static constexpr float ARROW_CHANGE_SIZE_SPEED = 5.0f;
	static constexpr int ARROW_SHIFT_POS_X = 80;
	static constexpr int ARROW_SHIFT_POS_Y = 200;

	//��Փx�摜�̑傫��
	static constexpr int DIFFICULTY_SIZE_X = 400;
	static constexpr int DIFFICULTY_SIZE_Y = 500;
	
	static constexpr int DIFFICULTY_SHIFT_SIZE_X = 218;
	static constexpr int DIFFICULTY_SHIFT_SIZE_Y = 60;

	//���[���摜�̑傫��
	static constexpr int RULE_SIZE_X = 320;
	static constexpr int RULE_SIZE_Y = 200;

	//���[���̍��ڊ�
	static constexpr int RULE_DIS_X = 40;		
	static constexpr int RULE_DIS_Y = 80;		

	//����(SE)
	//-----------------

	//���艹
	static constexpr int CLICK_VOLUME = 70;
	
	//�L�����Z����
	static constexpr int CANCEL_VOLUME = 500;

	//�����o�[�֐�
	//-------------------------------

	//�R���X�g���N�^
	SceneSelect();

	//�f�X�g���N�^
	~SceneSelect()override;

	//����������
	bool Init(void) override;		
	//�X�V����
	void Update(void) override;		
	//�`�揈��
	void Draw(void) override;		
	//�������
	bool Release(void) override;	

private:
	//�w�i
	enum class BACKGROUND_TYPE
	{
		BACK	//�w�i
		, SKY	//��
		, CLOUD	//�_
		, MAX
	};

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

	//�v���C���[�̃A�j���[�V�������x
	static constexpr float SPEED_ANIM = 60.0f;

	//�C���X�^���X
	//------------------
	SoundManager* sound_;

	//�����o�[�ϐ�
	//-------------------------------
	
	int fontHandle_;				//�t�H���g�p
	int padFont_;					//�p�b�h���̃t�H���g�p

	SELECT_ITEM selectItem_;		//���݂̑I������
	Vector2F cursorPos_;			//�J�[�\���̈ʒu
	int selectPadNum_;				//�I������Ă���p�b�h�̔ԍ�
	bool isVib_;					//�o�C�u���[�V�����̃t���O
	int playerNum_;					//�Q���v���C���[�̐�
	CommonData::CPU_NUM cpuNum_;	//CPU�̐�
	int compareCpuNum_;				//CPU�̐���r�p

	int pModel_[static_cast<int>(CommonData::TYPE::P4)];	//�v���C���[�̃��f��
	VECTOR pScl_;											//�v���C���[�̑傫��
	VECTOR pRot_;											//�v���C���[�̉�]
	VECTOR pPos_;											//�v���C���[�̈ʒu
	float stepAnim_;												//�Đ����̃A�j���[�V��������
	float speedAnim_;												//�A�j���[�V�������x
	float animTotalTime_[static_cast<int>(CommonData::TYPE::P4)];	//�A�j���[�V�����̑��Đ�����
	int animAttachNo_[static_cast<int>(CommonData::TYPE::P4)];		//�A�^�b�`���Ă���A�j���[�V����

	CommonData::BATTLE_PATTERN selectPattern_;	//���ݑI�𒆂̑ΐ�l��
	CommonData::RULE selectRule_;				//���ݑI�𒆂̃��[��
	CommonData::DIFFICULTY selectDifficulty_;	//���ݑI�𒆂̓�Փx

	std::map<SELECT_ITEM, std::function<void(void)>> itemUpdate_;	//���ڂ��Ƃ̍X�V
	std::map<SELECT_ITEM, std::function<void(void)>> itemDraw_;		//���ڂ��Ƃ̕`��
	
	//�摜�n

	//�w�i�摜�̊i�[�̈�
	std::map<BACKGROUND_TYPE, int> bgImages_;
	Vector2F cloudPos_;

	//�ΐ�l���̉摜
	std::map<CommonData::BATTLE_PATTERN, int> bpImages_;

	//���摜
	int arrowImage_;

	//���Ԍv��
	float step_;

	//���̑傫��
	float arrowSize_;

	//�R���g���[���[���̉摜�p
	int padNumImage_;

	//��Փx�̉摜
	std::map<CommonData::DIFFICULTY, int> diffImages_;

	//���[���̉摜
	std::map<CommonData::RULE,int> ruleImages_;	

	//���[�������̉摜
	std::map<CommonData::RULE,int> ruleExplainImages_;	

	//�`���[�g���A���p�摜
	int tutorialImage_;

	//�摜�̈ʒu
	Vector2F imgsPos_;					

	float distance_;				//���ڊԂ̋���		

	/// <summary>
	/// �J�[�\���`��
	/// </summary>
	/// <param name="_sizeX">�J�[�\�������Ă�摜��X�T�C�Y</param>
	/// <param name="_sizeY">�J�[�\�������Ă�摜��Y�T�C�Y</param>
	void CursorDraw(int _sizeX, int _sizeY);

	//���ڂ��Ƃ̍X�V
	void BattlePatternUpdate(void);
	void CheckPadUpdate(void);
	void DifficultyUpdate(void);
	void RuleUpdate(void);

	//���ڂ��Ƃ̕`��
	void BattlePatternDraw(void);
	void CheckPadDraw(void);
	void DifficultyDraw(void);
	void RuleDraw(void);
};