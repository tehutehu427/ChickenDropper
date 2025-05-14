#include<DxLib.h>
#include"CommonData.h"

//�V���O���g����
//---------------------------
//instance�̏�����
CommonData* CommonData::instance_ = nullptr;

CommonData::CommonData(void)
{
	playerNum_ = TYPE::NONE;
	cpuNum_ = CPU_NUM::NONE;
	battlePattern_ = BATTLE_PATTERN::MAX;
	rule_ = RULE::NONE;
}

CommonData::~CommonData(void)
{
}

bool CommonData::Init(void)
{
	return true;
}

const CommonData::TYPE CommonData::GetPlayerNum(void)const
{
	return playerNum_;
}

void CommonData::SetPlayerNum(const TYPE _pNum)
{
	playerNum_ = _pNum;
}

const CommonData::CPU_NUM CommonData::GetCPUNum(void)const
{
	return cpuNum_;
}

void CommonData::SetCPUNum(const CPU_NUM _cNum)
{
	cpuNum_ = _cNum;
}

const CommonData::BATTLE_PATTERN CommonData::GetBattlePattern(void)const
{
	return battlePattern_;
}

void CommonData::SetBattlePattern(const BATTLE_PATTERN _pattern)
{
	//�ΐ�l��
	battlePattern_ = _pattern;

	switch (battlePattern_)
	{
	case CommonData::BATTLE_PATTERN::P1C1:
		playerNum_ = TYPE::P1;
		cpuNum_ = CPU_NUM::C1;
		break;

	case CommonData::BATTLE_PATTERN::P1C2:
		playerNum_ = TYPE::P1;
		cpuNum_ = CPU_NUM::C2;
		break;

	case CommonData::BATTLE_PATTERN::P1C3:
		playerNum_ = TYPE::P1;
		cpuNum_ = CPU_NUM::C3;
		break;

	case CommonData::BATTLE_PATTERN::P2:
		playerNum_ = TYPE::P2;
		cpuNum_ = CPU_NUM::NONE;
		break;

	case CommonData::BATTLE_PATTERN::P2C1:
		playerNum_ = TYPE::P2;
		cpuNum_ = CPU_NUM::C1;
		break;

	case CommonData::BATTLE_PATTERN::P2C2:
		playerNum_ = TYPE::P2;
		cpuNum_ = CPU_NUM::C2;
		break;

	case CommonData::BATTLE_PATTERN::P3:
		playerNum_ = TYPE::P3;
		cpuNum_ = CPU_NUM::NONE;
		break;

	case CommonData::BATTLE_PATTERN::P3C1:
		playerNum_ = TYPE::P3;
		cpuNum_ = CPU_NUM::C1;
		break;

	case CommonData::BATTLE_PATTERN::P4:
		playerNum_ = TYPE::P4;
		cpuNum_ = CPU_NUM::NONE;
		break;
	}
}

const CommonData::RULE CommonData::GetRule(void)const
{
	return rule_;
}

void CommonData::SetRule(const RULE _ruleType)
{
	rule_ = _ruleType;
}

const std::vector<int> CommonData::GetRank(void)const
{
	return rank_;
}

const int CommonData::GetRank(const int size)const
{
	return rank_[size];
}

void CommonData::SetRank(int _rank)
{
	rank_.push_back(_rank);
}

void CommonData::DeleteArray(void)
{
	size_t size = rank_.size();

	//�z������ׂĎ��o��
	for (int r = 0; r < size; r++)
	{
		rank_.pop_back();
	}

	size = difficulty_.size();

	//�z������ׂĎ��o��
	for (int r = 0; r < size; r++)
	{
		difficulty_.pop_back();
	}
}

const CommonData::DIFFICULTY CommonData::GetDifficulty(const int size)const
{
	return difficulty_[size];
}

const size_t CommonData::GetDifficultySize(void)const
{
	return difficulty_.size();
}

const CommonData::DIFFICULTY CommonData::PopDifficulty(void)
{
	//�z��̃T�C�Y
	size_t size = difficulty_.size();

	//�z�񂪂Ȃ��Ȃ�
	if (size <= 0)
	{
		//�G���[�@�����Ԃ��Ȃ�
		OutputDebugString("difficulty�z�񂪂���܂���");
		return DIFFICULTY();
	}

	//�T�C�Y�ɑ΂���z��ԍ�
	int dArray = size - 1;

	//��Ԍ��̔z�񂩂甲���o��
	DIFFICULTY difficulty;

	difficulty = difficulty_[dArray];

	//�z��̍폜
	difficulty_.pop_back();

	//��Ԍ��̔z��̒��g��Ԃ�
	return difficulty;

}

void CommonData::SetDifficulty(const DIFFICULTY _difficulty)
{
	difficulty_.push_back(_difficulty);
}

void CommonData::DeleteDiff(void)
{
	difficulty_.pop_back();
}

void CommonData::DeleteDiffAll(void)
{
	size_t size = difficulty_.size();

	while (size > 0)
	{
		difficulty_.pop_back();

		size = difficulty_.size();
	}
}

//�V���O���g����
//---------------------------
//�O������ÓI�ɃC���X�^���X�̐���������
void CommonData::CreateInstance(void)
{
	//instance_��nullptr�̏ꍇ�͏��������ꂽ�̂������������s��
	if (instance_ == nullptr)
	{
		instance_ = new CommonData();
	}

	instance_->Init();
}

//�O���ŃC���X�^���X�𗘗p�\�Ƃ��邽�߂ɁA
//�C���X�^���X��Ԃ��֐������
CommonData& CommonData::GetInstance(void)
{
	return *instance_;
}

//�C���X�^���X�̔j��
void CommonData::Destroy(void)
{
	delete instance_;		//�C���X�^���X�̍폜
	instance_ = nullptr;	//�C���X�^���X�̊i�[�̈��������
}