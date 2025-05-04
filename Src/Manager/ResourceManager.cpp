#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

}

void ResourceManager::InitTitle(void)
{
	//*********************************************
	// タイトルシーンで使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	//タイトル背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleBack.png");
	resourcesMap_.emplace(SRC::TITLE_BACK, res);

	//タイトル背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "cloud.png");
	resourcesMap_.emplace(SRC::CLOUD, res);

	//タイトル背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleLogo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, res);
}

void ResourceManager::InitSelect(void)
{
	//*********************************************
	// セレクトシーンで使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	//セレクト背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectBack.png");
	resourcesMap_.emplace(SRC::SELECT_BACK, res);

	//セレクト空
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectBackSky.png");
	resourcesMap_.emplace(SRC::SELECT_SKY, res);

	//セレクト用雲
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectBackCloud.png");
	resourcesMap_.emplace(SRC::SELECT_CLOUD, res);
	
	//プレイヤー1人 CPU1人
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP1C1.png");
	resourcesMap_.emplace(SRC::BATTLE_P1C1, res);
	
	//プレイヤー1人 CPU2人
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP1C2.png");
	resourcesMap_.emplace(SRC::BATTLE_P1C2, res);
	
	//プレイヤー1人 CPU3人
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP1C3.png");
	resourcesMap_.emplace(SRC::BATTLE_P1C3, res);

	//プレイヤー2人
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP2.png");
	resourcesMap_.emplace(SRC::BATTLE_P2, res);

	//プレイヤー2人 CPU1人
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP2C1.png");
	resourcesMap_.emplace(SRC::BATTLE_P2C1, res);

	//プレイヤー2人 CPU2人
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP2C2.png");
	resourcesMap_.emplace(SRC::BATTLE_P2C2, res);

	//プレイヤー3人
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP3.png");
	resourcesMap_.emplace(SRC::BATTLE_P3, res);

	//プレイヤー3人 CPU1人
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP3C1.png");
	resourcesMap_.emplace(SRC::BATTLE_P3C1, res);

	//プレイヤー4人
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP4.png");
	resourcesMap_.emplace(SRC::BATTLE_P4, res);

	//矢印
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Arrow.png");
	resourcesMap_.emplace(SRC::ARROW, res);

	//パッド数
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "PadNum.png");
	resourcesMap_.emplace(SRC::PAD_NUM, res);

	//難易度 簡単
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "DifficultyEasy.png");
	resourcesMap_.emplace(SRC::EASY, res);

	//難易度 普通
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "DifficultyNormal.png");
	resourcesMap_.emplace(SRC::NORMAL, res);

	//難易度 難しい
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "DifficultyHard.png");
	resourcesMap_.emplace(SRC::HARD, res);

	//スコア制
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleScore.png");
	resourcesMap_.emplace(SRC::RULE_SCORE, res);

	//ライフ制
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleLife.png");
	resourcesMap_.emplace(SRC::RULE_LIFE, res);

	//タイル破壊制
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleTileBreak.png");
	resourcesMap_.emplace(SRC::RULE_TILE, res);

	//スコア制説明
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleScoreExplain.png");
	resourcesMap_.emplace(SRC::SCORE_EXPLAIN, res);

	//ライフ制説明
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleLifeExplain.png");
	resourcesMap_.emplace(SRC::LIFE_EXPLAIN, res);

	//タイル破壊制説明
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleTileBreakExplain.png");
	resourcesMap_.emplace(SRC::TILE_EXPLAIN, res);

	//プレイヤー
	ResourcePlayer();
}

void ResourceManager::InitGame(void)
{
	//*********************************************
	// ゲームシーンで使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	ResourcePlayer();
}

void ResourceManager::InitResult(void)
{
	//*********************************************
	// リザルトシーンで使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;
}

void ResourceManager::ResourcePlayer(void)
{
	//*********************************************
	// プレイヤーが使うリソース
	//*********************************************

	Resource res;

	//ニワトリ青
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "ChickenBlue.mv1");
	resourcesMap_.emplace(SRC::CHICKEN_BLUE, res);

	//ニワトリ赤
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "ChickenRed.mv1");
	resourcesMap_.emplace(SRC::CHICKEN_RED, res);

	//ニワトリ緑
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "ChickenGreen.mv1");
	resourcesMap_.emplace(SRC::CHICKEN_GREEN, res);

	//ニワトリ黄
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "ChickenYellow.mv1");
	resourcesMap_.emplace(SRC::CHICKEN_YELLOW, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
	resourcesMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
