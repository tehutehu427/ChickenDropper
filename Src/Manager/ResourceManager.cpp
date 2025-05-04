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
}

void ResourceManager::InitGame(void)
{
	//*********************************************
	// ゲームシーンで使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	ResourcePlayer();
	ResourceEnemy();
}

void ResourceManager::InitResult(void)
{
	//*********************************************
	// リザルトシーンで使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;
}

//プレイヤー
//*************************************************************************************************************
void ResourceManager::ResourcePlayer(void)
{
	//*********************************************
	// プレイヤーが使うリソース
	//*********************************************

	Resource res;
}

//敵
//*************************************************************************************************************
void ResourceManager::ResourceEnemy(void)
{
	//*********************************************
	// 敵が使うリソース
	//*********************************************

	Resource res;

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
