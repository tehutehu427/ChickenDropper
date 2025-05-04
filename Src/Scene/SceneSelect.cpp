#include <DxLib.h>
#include "../Application.h"
#include"../Utility/Utility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/SceneManager.h"
#include "../Common/CommonData.h"
#include "SceneSelect.h"

SceneSelect::SceneSelect()
{
}

SceneSelect::~SceneSelect()
{
}

bool SceneSelect::Init(void)
{
	//�T�E���h�̃C���X�^���X
	sound_ = new SoundManager();
	sound_->SoundInit();

	//�T�E���h�̃��[�h
	sound_->BGMLoad(SoundManager::BGM_TYPE::SELECT);
	sound_->SELoad(SoundManager::SE_TYPE::CLICK);
	sound_->SELoad(SoundManager::SE_TYPE::CANCEL);

	//BGM�Đ�
	sound_->PlayBGM(SoundManager::BGM_TYPE::SELECT, DX_PLAYTYPE_LOOP);

	//�摜�̓ǂݍ���
	bgImages_[BACKGROUND_TYPE::BACK] = LoadGraph((Application::PATH_IMAGE + "SelectBack.png").c_str());
	bgImages_[BACKGROUND_TYPE::SKY] = LoadGraph((Application::PATH_IMAGE + "SelectBackSky.png").c_str());
	bgImages_[BACKGROUND_TYPE::CLOUD] = LoadGraph((Application::PATH_IMAGE + "SelectBackCloud.png").c_str());
	cloudPos_ = { 0.0f,0.0f };

	bpImages_[CommonData::BATTLE_PATTERN::P1C1] = LoadGraph((Application::PATH_IMAGE + "BattleP1C1.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P1C2] = LoadGraph((Application::PATH_IMAGE + "BattleP1C2.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P1C3] = LoadGraph((Application::PATH_IMAGE + "BattleP1C3.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P2] = LoadGraph((Application::PATH_IMAGE + "BattleP2.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P2C1] = LoadGraph((Application::PATH_IMAGE + "BattleP2C1.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P2C2] = LoadGraph((Application::PATH_IMAGE + "BattleP2C2.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P3] = LoadGraph((Application::PATH_IMAGE + "BattleP3.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P3C1] = LoadGraph((Application::PATH_IMAGE + "BattleP3C1.png").c_str());
	bpImages_[CommonData::BATTLE_PATTERN::P4] = LoadGraph((Application::PATH_IMAGE + "BattleP4.png").c_str());

	arrowImage_ = LoadGraph((Application::PATH_IMAGE + "arrow.png").c_str());

	padNumImage_ = LoadGraph((Application::PATH_IMAGE + "PadNum.png").c_str());

	diffImages_[CommonData::DIFFICULTY::EASY] = LoadGraph((Application::PATH_IMAGE + "difficultyEasy.png").c_str());
	diffImages_[CommonData::DIFFICULTY::NORMAL] = LoadGraph((Application::PATH_IMAGE + "difficultyNormal.png").c_str());
	diffImages_[CommonData::DIFFICULTY::HARD] = LoadGraph((Application::PATH_IMAGE + "difficultyHard.png").c_str());


	ruleImages_[CommonData::RULE::TIME] = LoadGraph((Application::PATH_IMAGE + "RuleScore.png").c_str());
	ruleImages_[CommonData::RULE::LIFE] = LoadGraph((Application::PATH_IMAGE + "RuleLife.png").c_str());
	ruleImages_[CommonData::RULE::BREAK_TILE] = LoadGraph((Application::PATH_IMAGE + "RuleTileBreak.png").c_str());

	ruleExplainImages_[CommonData::RULE::TIME] = LoadGraph((Application::PATH_IMAGE + "RuleScoreExplain.png").c_str());
	ruleExplainImages_[CommonData::RULE::LIFE] = LoadGraph((Application::PATH_IMAGE + "RuleLifeExplain.png").c_str());
	ruleExplainImages_[CommonData::RULE::BREAK_TILE] = LoadGraph((Application::PATH_IMAGE + "RuleTileBreakExplain.png").c_str());

	tutorialImage_ = LoadGraph((Application::PATH_IMAGE + "Tutorial.png").c_str());

	//���f���̓ǂݍ���
	pModel_[static_cast<int>(CommonData::TYPE::P1) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenBlue.mv1").c_str());
	pModel_[static_cast<int>(CommonData::TYPE::P2) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenRed.mv1").c_str());
	pModel_[static_cast<int>(CommonData::TYPE::P3) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenGreen.mv1").c_str());
	pModel_[static_cast<int>(CommonData::TYPE::P4) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenYellow.mv1").c_str());

	//�ϐ��̏�����
	//-----------------------
	selectItem_ = SELECT_ITEM::BATTLE_PATTERN;
	playerNum_ = 1;
	cpuNum_ = CommonData::CPU_NUM::C1;
	compareCpuNum_ = 0;
	isVib_ = true;
	arrowSize_ = ARROW_DEFAULT_SIZE;
	step_ = 0.0f;

	pScl_ = VScale(Utility::VECTOR_ONE,2.0f);
	pRot_ = Utility::VECTOR_ZERO;
	pPos_ = Utility::VECTOR_ZERO;
	stepAnim_ = 0.0f;

	if (GetJoypadNum() >= 1)
	{
		selectPadNum_ = 1;
	}
	else
	{
		selectPadNum_ = 0;
	}

	selectPattern_ = CommonData::BATTLE_PATTERN::P1C1;
	selectRule_ = CommonData::RULE::NONE;
	
	imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };
	cursorPos_ = imgsPos_;
	
	distance_ = Application::SCREEN_SIZE_X / static_cast<int>(CommonData::TYPE::P4);

	int padFontSize = 30;
	padFont_ = CreateFontToHandle(NULL, padFontSize, 10);

	//�t�H���g�f�[�^�p
	int fontSize = 300;
	fontHandle_ = CreateFontToHandle(NULL, fontSize, 3);

	for (int i = 0; i < static_cast<int>(CommonData::TYPE::P4); i++)
	{
		//���f���̈ʒu�A�傫���A�p�x
		MV1SetPosition(pModel_[i], pPos_);
		MV1SetScale(pModel_[i], pScl_);
		MV1SetRotationXYZ(pModel_[i], pRot_);

		animAttachNo_[i] = MV1AttachAnim(pModel_[i], static_cast<int>(ANIM_NUM::WALK));
		animTotalTime_[i] = MV1GetAttachAnimTotalTime(pModel_[i], animAttachNo_[i]);
	}

	//�֐��|�C���^������
	itemUpdate_.emplace(SELECT_ITEM::BATTLE_PATTERN, std::bind(&SceneSelect::BattlePatternUpdate, this));
	itemUpdate_.emplace(SELECT_ITEM::CHECK_PAD, std::bind(&SceneSelect::CheckPadUpdate, this));
	itemUpdate_.emplace(SELECT_ITEM::DIFFICULTY, std::bind(&SceneSelect::DifficultyUpdate, this));
	itemUpdate_.emplace(SELECT_ITEM::RULE, std::bind(&SceneSelect::RuleUpdate, this));

	itemDraw_.emplace(SELECT_ITEM::BATTLE_PATTERN, std::bind(&SceneSelect::BattlePatternDraw, this));
	itemDraw_.emplace(SELECT_ITEM::CHECK_PAD, std::bind(&SceneSelect::CheckPadDraw, this));
	itemDraw_.emplace(SELECT_ITEM::DIFFICULTY, std::bind(&SceneSelect::DifficultyDraw, this));
	itemDraw_.emplace(SELECT_ITEM::RULE, std::bind(&SceneSelect::RuleDraw, this));

	return true;
}

void SceneSelect::Update(void)
{
	//���̓}�l�[�W���[�̃C���X�^���X
	auto ins = InputManager::GetInstance();

	//���݂̃p�b�h�ڑ���
	int padNum = GetJoypadNum();

	//���ɐi�߂邩
	bool isNextSelect = true;

	//�_�̓���
	cloudPos_.x--;
	if (cloudPos_.x < -Application::SCREEN_SIZE_X)
	{
		cloudPos_.x = 0;
	}

	//���Ԑi�s
	step_ += Utility::DELTA_TIME;

	//���̑傫��
	arrowSize_ = ARROW_DEFAULT_SIZE + Utility::Deg2RadF(sinf(step_ * ARROW_CHANGE_SIZE_SPEED) * ARROW_CHANGE_SIZE);

	//���ڂ��Ƃ̍X�V
	itemUpdate_[selectItem_]();
}

void SceneSelect::Draw(void)
{
	//���݂̃p�b�h�ڑ���
	int padNum = GetJoypadNum();

	//�t�H���g�f�[�^�p
	int fontSize = 0;

	//�F
	unsigned int color;

	//�w�i�摜
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImages_[BACKGROUND_TYPE::SKY], true, false);
	DrawRotaGraph(cloudPos_.x, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImages_[BACKGROUND_TYPE::CLOUD], true, false);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImages_[BACKGROUND_TYPE::CLOUD], true, false);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X * 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImages_[BACKGROUND_TYPE::CLOUD], true, false);
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImages_[BACKGROUND_TYPE::BACK], true, false);

	//�I�����ڂ��Ƃ̕`��
	itemDraw_[selectItem_]();

	//�t�H���g�̉��
	DeleteFontToHandle(fontHandle_);
}

bool SceneSelect::Release(void)
{
	//�T�E���h�̉��
	sound_->SoundRelease();
	delete sound_;
	sound_ = nullptr;

	//�摜�̉��
	for (int bg = 0; bg < static_cast<int>(BACKGROUND_TYPE::MAX); bg++)
	{
		DeleteGraph(bgImages_[static_cast<BACKGROUND_TYPE>(bg)]);
	}

	for (int bp = 0; bp < static_cast<int>(CommonData::BATTLE_PATTERN::MAX); bp++)
	{
		DeleteGraph(bpImages_[static_cast<CommonData::BATTLE_PATTERN>(bp)]);
	}

	DeleteGraph(arrowImage_);
	
	DeleteGraph(padNumImage_);

	for(int rule = 0;rule<static_cast<int>(CommonData::RULE::BREAK_TILE);rule++)
	{
		DeleteGraph(ruleImages_[static_cast<CommonData::RULE>(rule)]);
		DeleteGraph(ruleExplainImages_[static_cast<CommonData::RULE>(rule)]);
	}

	DeleteGraph(tutorialImage_);

	//���f���̉��
	for (int pl = 0; pl < static_cast<int>(CommonData::TYPE::P4); pl++)
	{
		MV1DeleteModel(pModel_[pl]);
	}

	//�t�H���g�f�[�^���
	DeleteFontToHandle(fontHandle_);
	DeleteFontToHandle(padFont_);

	return true;
}

void SceneSelect::CursorDraw(int _sizeX, int _sizeY)
{
	if (static_cast<int>(step_ * Utility::DEFAULT_FPS / 20) % 2 == 0)
	{
		DrawBox(cursorPos_.x - CURSOR_SIZE - _sizeX / 2
			, cursorPos_.y - CURSOR_SIZE - _sizeY / 2
			, cursorPos_.x + CURSOR_SIZE + _sizeX / 2
			, cursorPos_.y + CURSOR_SIZE + _sizeY / 2
			, 0x44ff44, true);
	}
}

void SceneSelect::BattlePatternUpdate(void)
{
	//���̓}�l�[�W���[�̃C���X�^���X
	auto ins = InputManager::GetInstance();

	//���݂̃p�b�h�ڑ���
	int padNum = GetJoypadNum();

	//���ɐi�߂邩
	bool isNextSelect = true;

	//�摜�̈ʒu
	imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };

	//�J�[�\���ʒu
	cursorPos_ = imgsPos_;

	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_RIGHT))
	{
		//���̑ΐ�l����I��
		selectPattern_ = static_cast<CommonData::BATTLE_PATTERN>(static_cast<int>(selectPattern_) + 1);

		if (selectPattern_ > CommonData::BATTLE_PATTERN::P4)
		{
			selectPattern_ = CommonData::BATTLE_PATTERN::P1C1;
		}
	}
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_LEFT))
	{
		//�O�̑ΐ�l����I��
		selectPattern_ = static_cast<CommonData::BATTLE_PATTERN>(static_cast<int>(selectPattern_) - 1);

		if (selectPattern_ < CommonData::BATTLE_PATTERN::P1C1)
		{
			selectPattern_ = CommonData::BATTLE_PATTERN::P4;
		}
	}

	//����
	switch (selectPattern_)
	{
	case CommonData::BATTLE_PATTERN::P1C1:
	case CommonData::BATTLE_PATTERN::P1C2:
	case CommonData::BATTLE_PATTERN::P1C3:
		break;

	case CommonData::BATTLE_PATTERN::P2:
	case CommonData::BATTLE_PATTERN::P2C1:
	case CommonData::BATTLE_PATTERN::P2C2:
		if (padNum < 2)
		{
			isNextSelect = false;
		}
		break;

	case CommonData::BATTLE_PATTERN::P3:
	case CommonData::BATTLE_PATTERN::P3C1:
		if (padNum < 3)
		{
			isNextSelect = false;
		}
		break;

	case CommonData::BATTLE_PATTERN::P4:
		if (padNum < 4)
		{
			isNextSelect = false;
		}
		break;
	}

	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_1) && isNextSelect)
	{
		//���艹
		sound_->PlaySE(SoundManager::SE_TYPE::CLICK, DX_PLAYTYPE_BACK, CLICK_VOLUME);

		//�p�b�h���Ȃ��ł��Ȃ��Ƃ�
		if (padNum == 0)
		{
			//�G�̓�Փx���߂Ɉڍs
			selectItem_ = SELECT_ITEM::DIFFICULTY;

			return;
		}

		//�p�b�h�̊m�F�Ɉڍs
		selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);
	}
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_2))
	{
		//�L�����Z����
		sound_->PlaySE(SoundManager::SE_TYPE::CANCEL, DX_PLAYTYPE_BACK, CANCEL_VOLUME);

		//�^�C�g���ֈڍs
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
	}

	//����
	switch (selectPattern_)
	{
	case CommonData::BATTLE_PATTERN::P1C1:
	case CommonData::BATTLE_PATTERN::P1C2:
	case CommonData::BATTLE_PATTERN::P1C3:
		playerNum_ = static_cast<int>(CommonData::TYPE::P1);
		break;

	case CommonData::BATTLE_PATTERN::P2:
	case CommonData::BATTLE_PATTERN::P2C1:
	case CommonData::BATTLE_PATTERN::P2C2:
		playerNum_ = static_cast<int>(CommonData::TYPE::P2);
		break;

	case CommonData::BATTLE_PATTERN::P3:
	case CommonData::BATTLE_PATTERN::P3C1:
		playerNum_ = static_cast<int>(CommonData::TYPE::P3);
		break;

	case CommonData::BATTLE_PATTERN::P4:
		playerNum_ = static_cast<int>(CommonData::TYPE::P4);
		break;
	}

	switch (selectPattern_)
	{
	case CommonData::BATTLE_PATTERN::P2:
	case CommonData::BATTLE_PATTERN::P3:
	case CommonData::BATTLE_PATTERN::P4:

		cpuNum_ = CommonData::CPU_NUM::NONE;
		break;

	case CommonData::BATTLE_PATTERN::P1C1:
	case CommonData::BATTLE_PATTERN::P2C1:
	case CommonData::BATTLE_PATTERN::P3C1:

		cpuNum_ = CommonData::CPU_NUM::C1;
		break;

	case CommonData::BATTLE_PATTERN::P1C2:
	case CommonData::BATTLE_PATTERN::P2C2:

		cpuNum_ = CommonData::CPU_NUM::C2;
		break;

	case CommonData::BATTLE_PATTERN::P1C3:

		cpuNum_ = CommonData::CPU_NUM::C3;
		break;
	}
}

void SceneSelect::CheckPadUpdate(void)
{
	//���̓}�l�[�W���[�̃C���X�^���X
	auto ins = InputManager::GetInstance();

	//���݂̃p�b�h�ڑ���
	int padNum = GetJoypadNum();

	//���f���̈ʒu�A�傫���A�p�x
	MV1SetPosition(pModel_[selectPadNum_ - 1], pPos_);
	MV1SetScale(pModel_[selectPadNum_ - 1], pScl_);
	MV1SetRotationXYZ(pModel_[selectPadNum_ - 1], pRot_);

	//�A�j���[�V�����Đ�
	//-------------------------------------------

	//�A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (speedAnim_ * Utility::DELTA_TIME);
	if (stepAnim_ > animTotalTime_[selectPadNum_ - 1])
	{
		//���[�v�Đ�
		stepAnim_ = 0.0f;
	}

	//�Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(pModel_[selectPadNum_ - 1], animAttachNo_[selectPadNum_ - 1], stepAnim_);

	//�A�j���[�V�������x
	speedAnim_ = SPEED_ANIM;

	//�p�b�h���Ȃ��ł��Ȃ��Ƃ�
	if (padNum == 0)
	{
		//�G�̓�Փx���߂Ɉڍs
		selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);
	}
	//�p�b�h���Ȃ��ł���Ƃ�
	else if (playerNum_ >= selectPadNum_)
	{
		//�o�C�u���[�V���������邩
		if (isVib_)
		{
			//�p�b�h�̃o�C�u���[�V�������n�߂�
			StartJoypadVibration(selectPadNum_, VIB_STR, VIB_TIME);

			//�o�C�u���[�V�����I��
			isVib_ = false;
		}

		//�I�𒆂̃p�b�h��A�{�^������������
		if (ins.IsJoypadKeyPush(selectPadNum_, PAD_INPUT_1))
		{
			//���艹
			sound_->PlaySE(SoundManager::SE_TYPE::CLICK, DX_PLAYTYPE_BACK, CLICK_VOLUME);

			//�p�b�h�̃o�C�u���[�V�������~�߂�
			StopJoypadVibration(selectPadNum_);

			//���̃p�b�h��
			selectPadNum_++;

			//�o�C�u���[�V�������n�߂�
			isVib_ = true;
		}
		//�I�𒆂̃p�b�h��B�{�^������������
		else if (ins.IsJoypadKeyPush(selectPadNum_, PAD_INPUT_2))
		{
			//�L�����Z����
			sound_->PlaySE(SoundManager::SE_TYPE::CANCEL, DX_PLAYTYPE_BACK, CANCEL_VOLUME);

			//�p�b�h�̃o�C�u���[�V�������~�߂�
			StopJoypadVibration(selectPadNum_);

			//�O�̃p�b�h��
			selectPadNum_--;

			//�o�C�u���[�V�������n�߂�
			isVib_ = true;
		}

		//�S���̊m�F���I�����
		if (playerNum_ < selectPadNum_)
		{
			//�I������1P�ɓn��
			selectPadNum_ = padNum > 0 ? SELECT_PAD_P1 : SELECT_KEYBOARD;


			//CPU�̓�Փx���߂Ɉڍs
			selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);
		}
		//�߂铮�������
		else if (selectPadNum_ <= 0)
		{
			//�I������1P�ɓn��
			selectPadNum_ = padNum > 0 ? SELECT_PAD_P1 : SELECT_KEYBOARD;

			//�o�C�u���[�V�����t���O�̏�����
			isVib_ = true;

			//�ΐ�l�����߂ɖ߂�
			selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) - 1);
		}
	}
	//���ׂẴp�b�h�̊m�F���I�������
	else
	{
		//�I������1P�ɓn��
		selectPadNum_ = padNum > 0 ? SELECT_PAD_P1 : SELECT_KEYBOARD;

		//CPU�̓�Փx���߂Ɉڍs
		selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);
	}
}

void SceneSelect::DifficultyUpdate(void)
{
	//���̓}�l�[�W���[�̃C���X�^���X
	auto ins = InputManager::GetInstance();

	//�摜�̈ʒu
	imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };

	//�J�[�\���ʒu
	cursorPos_ = imgsPos_;

	//CPU�����Ȃ��Ȃ�
	if (cpuNum_ == CommonData::CPU_NUM::NONE)
	{
		//�X�L�b�v
		selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);
	}

	//�I�𗓂��E��
	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_RIGHT))
	{
		//�E�Ɉ���炷
		selectDifficulty_ = static_cast<CommonData::DIFFICULTY>(static_cast<int>(selectDifficulty_) + 1);

		//�[�Ȃ�
		if (static_cast<int>(selectDifficulty_) > static_cast<int>(CommonData::DIFFICULTY::HARD))
		{
			//���[�ɖ߂�
			selectDifficulty_ = CommonData::DIFFICULTY::EASY;
		}
	}
	//�I�𗓂�����
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_LEFT))
	{
		//���Ɉ���炷
		selectDifficulty_ = static_cast<CommonData::DIFFICULTY>(static_cast<int>(selectDifficulty_) - 1);

		//�p�Ȃ�
		if (static_cast<int>(selectDifficulty_) < static_cast<int>(CommonData::DIFFICULTY::EASY))
		{
			//�E�[�ɖ߂�
			selectDifficulty_ = CommonData::DIFFICULTY::HARD;
		}
	}

	//����
	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_1))
	{
		//���艹
		sound_->PlaySE(SoundManager::SE_TYPE::CLICK, DX_PLAYTYPE_BACK, CLICK_VOLUME);

		//����CPU�̐ݒ�Ɉڍs
		compareCpuNum_++;

		//��̕��̓�Փx��n��
		CommonData::GetInstance().SetDifficulty(selectDifficulty_);

		//������
		selectDifficulty_ = CommonData::DIFFICULTY::EASY;
	}
	//�L�����Z��
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_2))
	{
		//�L�����Z����
		sound_->PlaySE(SoundManager::SE_TYPE::CANCEL, DX_PLAYTYPE_BACK, CANCEL_VOLUME);

		//�O��CPU�̐ݒ�Ɉڍs
		compareCpuNum_--;

		//�ŏ���CPU�ȊO�Ȃ�
		if (compareCpuNum_ >= 0)
		{
			//��̕��̓�Փx���폜
			CommonData::GetInstance().DeleteDiff();

			//������
			selectDifficulty_ = CommonData::DIFFICULTY::EASY;
		}

		//�摜�̈ʒu
		imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };

		//�J�[�\���ʒu
		cursorPos_ = imgsPos_;
	}

	//�ŏ���CPU�̐ݒ肩��L�����Z�������Ȃ�
	if (compareCpuNum_ < 0)
	{
		//�ϐ��̏�����
		compareCpuNum_ = 0;

		//�ΐ�l�����߂ɖ߂�
		selectItem_ = SELECT_ITEM::BATTLE_PATTERN;

		//�摜�̈ʒu
		imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };

		//�J�[�\���ʒu
		cursorPos_ = imgsPos_;
	}
	//�SCPU�̐ݒ肪�I�����
	else if (compareCpuNum_ >= static_cast<int>(cpuNum_))
	{
		//�ϐ��̏�����
		compareCpuNum_ = 0;

		//���[�����߂ɐi��
		selectItem_ = static_cast<SELECT_ITEM>(static_cast<int>(selectItem_) + 1);

		//�摜�̈ʒu
		imgsPos_ = { (Application::SCREEN_SIZE_X - BOARD_SIZE_X) +
			((BOARD_SIZE_X - ((RULE_SIZE_X) * static_cast<int>(CommonData::RULE::BREAK_TILE) +
				(RULE_DIS_X * (static_cast<int>(CommonData::RULE::BREAK_TILE) - 1)))) / 2)
			,Application::SCREEN_SIZE_Y / 2 - RULE_SIZE_Y };

		//�J�[�\���̈ʒu
		cursorPos_ = { imgsPos_.x + ((RULE_SIZE_X + RULE_DIS_X) * (static_cast<int>(selectRule_) - 1)),imgsPos_.y };
	}
}

void SceneSelect::RuleUpdate(void)
{
	//���̓}�l�[�W���[�̃C���X�^���X
	auto ins = InputManager::GetInstance();

	//�摜�̈ʒu
	imgsPos_ = { (Application::SCREEN_SIZE_X - BOARD_SIZE_X) +
		((BOARD_SIZE_X - ((RULE_SIZE_X) * static_cast<int>(CommonData::RULE::BREAK_TILE) +
			(RULE_DIS_X * (static_cast<int>(CommonData::RULE::BREAK_TILE) - 1)))) / 2)
		,Application::SCREEN_SIZE_Y / 2 - RULE_SIZE_Y };

	//�J�[�\���̈ʒu
	cursorPos_ = { imgsPos_.x + ((RULE_SIZE_X + RULE_DIS_X) * (static_cast<int>(selectRule_) - 1)),imgsPos_.y };

	//�߂������̏�����
	if (selectRule_ == CommonData::RULE::NONE)
	{
		CommonData::GetInstance().SetRule(selectRule_);
		selectRule_ = CommonData::RULE::TIME;
	}

	//�E��������
	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_RIGHT))
	{
		//���̃��[����I��
		selectRule_ = static_cast<CommonData::RULE>(static_cast<int>(selectRule_) + 1);

		//�Ō�̃��[���܂ōs���Ă��Ȃ�
		if (selectRule_ > CommonData::RULE::BREAK_TILE)
		{
			//�ŏ��̃��[���ɖ߂�
			selectRule_ = CommonData::RULE::TIME;
		}
	}
	//����������
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_LEFT))
	{
		//�O�̃��[����I��
		selectRule_ = static_cast<CommonData::RULE>(static_cast<int>(selectRule_) - 1);

		//�ŏ��̃��[���܂ōs���Ă��Ȃ�
		if (selectRule_ < CommonData::RULE::TIME)
		{
			//�Ō�̃��[���ɖ߂�
			selectRule_ = CommonData::RULE::BREAK_TILE;
		}
	}

	//����
	if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_1))
	{
		//���艹
		sound_->PlaySE(SoundManager::SE_TYPE::CLICK, DX_PLAYTYPE_BACK, CLICK_VOLUME);

		//�ΐ�l���̍ŏI����
		CommonData::GetInstance().SetBattlePattern(selectPattern_);

		//���[���̍ŏI����
		CommonData::GetInstance().SetRule(selectRule_);

		//�Q�[���V�[���Ɉڂ�
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
	}
	else if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_2))
	{
		//�L�����Z����
		sound_->PlaySE(SoundManager::SE_TYPE::CANCEL, DX_PLAYTYPE_BACK, CANCEL_VOLUME);

		//�v���C���[�����߂ɖ߂�
		selectItem_ = SELECT_ITEM::BATTLE_PATTERN;

		//CPU��Փx�̔z����폜
		CommonData::GetInstance().DeleteDiffAll();

		//�摜�̈ʒu
		imgsPos_ = { Application::SCREEN_SIZE_X / 2 , Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT };

		//�J�[�\���ʒu
		cursorPos_ = imgsPos_;
	}
}

void SceneSelect::BattlePatternDraw(void)
{
	//�J�[�\���`��
	CursorDraw(BATTLE_NUM_SIZE_X, BATTLE_NUM_SIZE_Y);

	//�ΐ�l���̑I����
	DrawRotaGraph(imgsPos_.x, imgsPos_.y, 1.0, 0.0, bpImages_[selectPattern_], true);

	//�ΐ�l���̕����p�摜
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 2)
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT
		, 1.0
		, 0.0
		, padNumImage_
		, true);

	//�ΐ�l��
	DrawFormatStringToHandle(Application::SCREEN_SIZE_X / 2 - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) - BATTLE_PAT_SHIFT_POS_X
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT - BATTLE_PAT_SHIFT_POS_Y
		, Utility::COLOR_BLACK
		, padFont_
		, " �ΐ�l��\nPlayer �~%d\n CPU �~%d"
		, playerNum_
		, cpuNum_);

	//�R���g���[���[�̐ڑ����摜
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 + (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 2)
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT
		, 1.0
		, 0.0
		, padNumImage_
		, true);

	//�R���g���[���[�̐ڑ���
	DrawFormatStringToHandle(Application::SCREEN_SIZE_X / 2 + (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) - PADNUM_SHIFT_POS_X
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT - PADNUM_SHIFT_POS_Y
		, Utility::COLOR_BLACK
		, padFont_
		, "�ڑ�����Ă���\n�R���g���[���[\n   ����%d��"
		, GetJoypadNum());

	//�E���摜
	DrawRotaGraph(imgsPos_.x + (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) - ARROW_SHIFT_POS_X
		, imgsPos_.y + ARROW_SHIFT_POS_Y
		, arrowSize_
		, 0.0
		, arrowImage_
		, true);

	//�����摜
	DrawRotaGraph(imgsPos_.x - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) + ARROW_SHIFT_POS_X
		, imgsPos_.y + ARROW_SHIFT_POS_Y
		, arrowSize_
		, 0.0
		, arrowImage_
		, true
		, true);
}

void SceneSelect::CheckPadDraw(void)
{
	//�t�H���g
	int fontSize = 70;
	fontHandle_ = CreateFontToHandle(NULL, fontSize, 3, DX_FONTTYPE_EDGE);
	int color = Utility::COLOR_WHITE;

	switch (selectPadNum_)
	{
	case 1:
		color = 0x0000ff;
		break;

	case 2:
		color = 0xff0000;
		break;

	case 3:
		color = 0x00ff00;
		break;

	case 4:
		color = 0xffff00;
		break;
	}

	MV1DrawModel(pModel_[selectPadNum_ - 1]);

	DrawFormatStringFToHandle(fontSize * 5.5f, Application::SCREEN_SIZE_Y / (3.0f / 2.0f) - fontSize, color, fontHandle_, "     P%d Press A!!", selectPadNum_);
}

void SceneSelect::DifficultyDraw(void)
{
	//�J�[�\��
	CursorDraw(DIFFICULTY_SIZE_X, DIFFICULTY_SIZE_Y);

	//�t�H���g
	int fontSize = 30;
	fontHandle_ = CreateFontToHandle(NULL, fontSize, 3, DX_FONTTYPE_EDGE);
	int color = 0xffffff;

	switch (selectPattern_)
	{
	case CommonData::BATTLE_PATTERN::P1C1:
	case CommonData::BATTLE_PATTERN::P1C2:
	case CommonData::BATTLE_PATTERN::P1C3:
		if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C1)))
		{
			color = 0xff0000;
		}
		else if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C2)))
		{
			color = 0x00ff00;
		}
		else if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C3)))
		{
			color = 0xffff00;
		}

		break;

	case CommonData::BATTLE_PATTERN::P2C1:
	case CommonData::BATTLE_PATTERN::P2C2:
		if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C1)))
		{
			color = 0x00ff00;
		}
		else if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C2)))
		{
			color = 0xffff00;
		}

		break;

	case CommonData::BATTLE_PATTERN::P3C1:
		if (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_) == (static_cast<int>(CommonData::CPU_NUM::C1)))
		{
			color = 0xffff00;
		}

		break;
	}

	//�ΐ�l���̕����p�摜
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 2)
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT
		, 1.0
		, 0.0
		, padNumImage_
		, true);

	//�ΐ�l��
	DrawFormatStringToHandle(Application::SCREEN_SIZE_X / 2 - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) - DIFFICULTY_SHIFT_SIZE_X
		, Application::SCREEN_SIZE_Y - BOARD_SIZE_Y / 2 - BOARD_HEIGHT - DIFFICULTY_SHIFT_SIZE_Y
		, color
		, fontHandle_
		, "�v���C���[%d�̋�����\n �I�����Ă�������"
		, static_cast<int>(playerNum_) + (static_cast<int>(cpuNum_) + 1 - (static_cast<int>(cpuNum_) - compareCpuNum_)));

	//�E���摜
	DrawRotaGraph(imgsPos_.x + (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) - ARROW_SHIFT_POS_X
		, imgsPos_.y + ARROW_SHIFT_POS_Y
		, arrowSize_
		, 0.0
		, arrowImage_
		, true);

	//�����摜
	DrawRotaGraph(imgsPos_.x - (BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5) + ARROW_SHIFT_POS_X
		, imgsPos_.y + ARROW_SHIFT_POS_Y
		, arrowSize_
		, 0.0
		, arrowImage_
		, true
		, true);

	DrawRotaGraph(imgsPos_.x, imgsPos_.y, 1.0, 0.0, diffImages_[selectDifficulty_], true);
}

void SceneSelect::RuleDraw(void)
{
	//�J�[�\��
	CursorDraw(RULE_SIZE_X, RULE_SIZE_Y);

	//�t�H���g
	int fontSize = 100;
	fontHandle_ = CreateFontToHandle(NULL, fontSize, 3);

	for (int r = 0; r < static_cast<int>(CommonData::RULE::BREAK_TILE); r++)
	{
		DrawRotaGraph(imgsPos_.x + ((RULE_SIZE_X + RULE_DIS_X) * r), imgsPos_.y, 1.0, 0.0, ruleImages_[static_cast<CommonData::RULE>(r + 1)], true, false);
	}

	DrawRotaGraph(imgsPos_.x + (RULE_SIZE_X + RULE_DIS_X), Application::SCREEN_SIZE_Y / 2 + RULE_DIS_Y, 1.0, 0.0, ruleExplainImages_[selectRule_], true);
}
