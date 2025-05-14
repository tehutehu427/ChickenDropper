#include"../Application.h"
#include"../Common/Easing.h"
#include"../Manager/Resource.h"
#include"../Manager/ResourceManager.h"
#include"../Object/Player.h"
#include "RuleBase.h"

RuleBase::RuleBase(SceneGame* parent)
{
	sceneGame_ = parent;
}

RuleBase::~RuleBase(void)
{
}

bool RuleBase::Init(void)
{
	//���\�[�X�}�l�[�W���擾
	ResourceManager& res = ResourceManager::GetInstance();

	//�X�e�[�^�X���̓ǂݍ���
	stateImages_[static_cast<int>(CommonData::TYPE::P1) - 1] = res.Load(ResourceManager::SRC::STATE_BLUE).handleId_;
	stateImages_[static_cast<int>(CommonData::TYPE::P2) - 1] = res.Load(ResourceManager::SRC::STATE_RED).handleId_;
	stateImages_[static_cast<int>(CommonData::TYPE::P3) - 1] = res.Load(ResourceManager::SRC::STATE_GREEN).handleId_;
	stateImages_[static_cast<int>(CommonData::TYPE::P4) - 1] = res.Load(ResourceManager::SRC::STATE_YELLOW).handleId_;

	//�͈͏k���x���̉摜�ǂݍ���
	narrowInfoImage_ = res.Load(ResourceManager::SRC::NARROW_INFO).handleId_;

	//���ԃJ�E���g
	secCount_ = GetNowCount();

	//�^�C�}�[�t�H���g�p
	timeFontSize_ = TIME_SIZE;
	timeFontThickness_ = TIME_THICKNESS;
	timeFont_ = CreateFontToHandle(NULL, timeFontSize_, timeFontThickness_, DX_FONTTYPE_EDGE);

	//�X�R�A�t�H���g�p
	scoreFontSize_ = SCORE_SIZE;
	scoreFontThickness_ = SCORE_THICKNESS;
	scoreFont_ = CreateFontToHandle(NULL, scoreFontSize_, scoreFontThickness_, DX_FONTTYPE_EDGE);

	//�X�e�[�W�k���̃t���O
	isNarrowStage_ = false;

	//�ϐ��̏�����
	narrowInfoPos_ = 0.0f;
	narrowInfoTime_ = 0.0f;

	return true;
}

void RuleBase::Update(void)
{
	//�X�e�[�W�k������
	if (!isNarrowStage_)
	{
		//�������Ȃ�
		return;
	}

	//�k���x��
	if (narrowInfoTime_ < NARROW_INFO_TIME)
	{
		//�k���x���̕\������
		narrowInfoTime_++;
		
		//�k���x���̍��W
		narrowInfoPos_ = SineOut(narrowInfoTime_
			, NARROW_INFO_TIME
			, Application::SCREEN_SIZE_X + NARROW_INFO_SIZE_X / 2
			, -NARROW_INFO_SIZE_X);
	}
}

void RuleBase::Draw(void)
{
	//�X�e�[�W�k������
	if (!isNarrowStage_)
	{
		return;
	}

	//�͈͏k���x��
	DrawRotaGraph(narrowInfoPos_, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, narrowInfoImage_, true);
}

bool RuleBase::Release(void)
{
	//�t�H���g�̉��
	DeleteFontToHandle(timeFont_);
	DeleteFontToHandle(scoreFont_);

	return true;
}

const bool RuleBase::GetIsNarrowStage(void)
{
	return isNarrowStage_;
}

void RuleBase::InitRank(void)
{
	//�v���C���[
	auto playerIns = sceneGame_->GetPlayer();

	for (auto player : playerIns)
	{
		//�����ʂŏ�����
		player->SetRank(static_cast<int>(player->GetCharaNum()));
	}
}
