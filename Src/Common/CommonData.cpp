#include<DxLib.h>
#include"CommonData.h"

//シングルトン化
//---------------------------
//instanceの初期化
CommonData* CommonData::instance_ = nullptr;

CommonData::CommonData(void)
{
}

CommonData::~CommonData(void)
{
}

bool CommonData::Init(void)
{
	return true;
}

//プレイヤーの数を返す
CommonData::TYPE CommonData::GetPlayerNum(void)
{
	return playerNum_;
}

//プレイヤーの数を決める
void CommonData::SetPlayerNum(TYPE _pNum)
{
	playerNum_ = _pNum;
}

//CPUの数を返す
CommonData::CPU_NUM CommonData::GetCPUNum(void)
{
	return cpuNum_;
}

//CPUの数を決める
void CommonData::SetCPUNum(CPU_NUM _cNum)
{
	cpuNum_ = _cNum;
}

CommonData::BATTLE_PATTERN CommonData::GetBattlePattern(void)
{
	return battlePattern_;
}

void CommonData::SetBattlePattern(BATTLE_PATTERN _pattern)
{
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

//ルールの情報を返す
CommonData::RULE CommonData::GetRule(void)
{
	return rule_;
}

//ルールを決める
void CommonData::SetRule(RULE _ruleType)
{
	rule_ = _ruleType;
}

std::vector<int> CommonData::GetRank(void)
{
	return rank_;
}

int CommonData::GetRank(int size)
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

	//配列をすべて取り出す
	for (int r = 0; r < size; r++)
	{
		rank_.pop_back();
	}

	size = difficulty_.size();

	//配列をすべて取り出す
	for (int r = 0; r < size; r++)
	{
		difficulty_.pop_back();
	}
}

CommonData::DIFFICULTY CommonData::GetDifficulty(int size)
{
	return difficulty_[size];
}

size_t CommonData::GetDifficultySize(void)
{
	return difficulty_.size();
}

CommonData::DIFFICULTY CommonData::PopDifficulty(void)
{
	//配列のサイズ
	size_t size = difficulty_.size();

	//配列がないなら
	if (size <= 0)
	{
		//エラー　何も返さない
		OutputDebugString("difficulty配列がありません");
		return DIFFICULTY();
	}

	//サイズに対する配列番号
	int dArray = size - 1;

	//一番後ろの配列から抜き出す
	DIFFICULTY difficulty;

	difficulty = difficulty_[dArray];

	//配列の削除
	difficulty_.pop_back();

	//一番後ろの配列の中身を返す
	return difficulty;

}

void CommonData::SetDifficulty(DIFFICULTY _difficulty)
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

bool CommonData::GetIsTutorial(void)
{
	return isTutorial_;
}

void CommonData::SetIsTutorial(bool _isTutorial)
{
	isTutorial_ = _isTutorial;
}

//シングルトン化
//---------------------------
//外部から静的にインスタンスの生成をする
void CommonData::CreateInstance(void)
{
	//instance_がnullptrの場合は初期化されたのち生成処理を行う
	if (instance_ == nullptr)
	{
		instance_ = new CommonData();
	}

	instance_->Init();
}

//外部でインスタンスを利用可能とするために、
//インスタンスを返す関数を作る
CommonData& CommonData::GetInstance(void)
{
	return *instance_;
}

//インスタンスの破棄
void CommonData::Destroy(void)
{
	delete instance_;		//インスタンスの削除
	instance_ = nullptr;	//インスタンスの格納領域を初期化
}