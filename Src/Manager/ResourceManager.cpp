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
	// �^�C�g���V�[���Ŏg�����\�[�X
	//*********************************************

	//���\�[�X�������ݗp
	Resource res;

	//�^�C�g���w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleBack.png");
	resourcesMap_.emplace(SRC::TITLE_BACK, res);

	//�^�C�g���w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "cloud.png");
	resourcesMap_.emplace(SRC::CLOUD, res);

	//�^�C�g���w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleLogo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, res);
}

void ResourceManager::InitSelect(void)
{
	//*********************************************
	// �Z���N�g�V�[���Ŏg�����\�[�X
	//*********************************************

	//���\�[�X�������ݗp
	Resource res;

	//�Z���N�g�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectBack.png");
	resourcesMap_.emplace(SRC::SELECT_BACK, res);

	//�Z���N�g��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectBackSky.png");
	resourcesMap_.emplace(SRC::SELECT_SKY, res);

	//�Z���N�g�p�_
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectBackCloud.png");
	resourcesMap_.emplace(SRC::SELECT_CLOUD, res);
	
	//�v���C���[1�l CPU1�l
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP1C1.png");
	resourcesMap_.emplace(SRC::BATTLE_P1C1, res);
	
	//�v���C���[1�l CPU2�l
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP1C2.png");
	resourcesMap_.emplace(SRC::BATTLE_P1C2, res);
	
	//�v���C���[1�l CPU3�l
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP1C3.png");
	resourcesMap_.emplace(SRC::BATTLE_P1C3, res);

	//�v���C���[2�l
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP2.png");
	resourcesMap_.emplace(SRC::BATTLE_P2, res);

	//�v���C���[2�l CPU1�l
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP2C1.png");
	resourcesMap_.emplace(SRC::BATTLE_P2C1, res);

	//�v���C���[2�l CPU2�l
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP2C2.png");
	resourcesMap_.emplace(SRC::BATTLE_P2C2, res);

	//�v���C���[3�l
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP3.png");
	resourcesMap_.emplace(SRC::BATTLE_P3, res);

	//�v���C���[3�l CPU1�l
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP3C1.png");
	resourcesMap_.emplace(SRC::BATTLE_P3C1, res);

	//�v���C���[4�l
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BattleP4.png");
	resourcesMap_.emplace(SRC::BATTLE_P4, res);

	//���
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Arrow.png");
	resourcesMap_.emplace(SRC::ARROW, res);

	//�p�b�h��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "PadNum.png");
	resourcesMap_.emplace(SRC::PAD_NUM, res);

	//��Փx �ȒP
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "DifficultyEasy.png");
	resourcesMap_.emplace(SRC::EASY, res);

	//��Փx ����
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "DifficultyNormal.png");
	resourcesMap_.emplace(SRC::NORMAL, res);

	//��Փx ���
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "DifficultyHard.png");
	resourcesMap_.emplace(SRC::HARD, res);

	//�X�R�A��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleScore.png");
	resourcesMap_.emplace(SRC::RULE_SCORE, res);

	//���C�t��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleLife.png");
	resourcesMap_.emplace(SRC::RULE_LIFE, res);

	//�^�C���j��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleTileBreak.png");
	resourcesMap_.emplace(SRC::RULE_TILE, res);

	//�X�R�A������
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleScoreExplain.png");
	resourcesMap_.emplace(SRC::SCORE_EXPLAIN, res);

	//���C�t������
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleLifeExplain.png");
	resourcesMap_.emplace(SRC::LIFE_EXPLAIN, res);

	//�^�C���j�󐧐���
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "RuleTileBreakExplain.png");
	resourcesMap_.emplace(SRC::TILE_EXPLAIN, res);

	//�v���C���[
	ResourcePlayer();
}

void ResourceManager::InitGame(void)
{
	//*********************************************
	// �Q�[���V�[���Ŏg�����\�[�X
	//*********************************************

	//���\�[�X�������ݗp
	Resource res;

	//�v���C���[
	ResourcePlayer();

	//�X�e�[�W
	ResourceStage();
}

void ResourceManager::InitResult(void)
{
	//*********************************************
	// ���U���g�V�[���Ŏg�����\�[�X
	//*********************************************

	//���\�[�X�������ݗp
	Resource res;

	//���U���g�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ResultBack.png");
	resourcesMap_.emplace(SRC::RESULT_BACK, res);

	//���䖋�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ResultBackCurtain.png");
	resourcesMap_.emplace(SRC::CURTAIN_BACK, res);
	
	//���䖋
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Curtain.png");
	resourcesMap_.emplace(SRC::CURTAIN, res);

	//1��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "1stImage.png");
	resourcesMap_.emplace(SRC::RANK_1ST, res);

	//2��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "2ndImage.png");
	resourcesMap_.emplace(SRC::RANK_2ND, res);

	//3��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "3rdImage.png");
	resourcesMap_.emplace(SRC::RANK_3RD, res);

	//4��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "4thImage.png");
	resourcesMap_.emplace(SRC::RANK_4TH, res);

	//�v���C���[
	ResourcePlayer();
}

void ResourceManager::ResourcePlayer(void)
{
	//*********************************************
	// �v���C���[���g�����\�[�X
	//*********************************************

	//���\�[�X�������ݗp
	Resource res;

	//�j���g����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "ChickenBlue.mv1");
	resourcesMap_.emplace(SRC::CHICKEN_BLUE, res);

	//�j���g����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "ChickenRed.mv1");
	resourcesMap_.emplace(SRC::CHICKEN_RED, res);

	//�j���g����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "ChickenGreen.mv1");
	resourcesMap_.emplace(SRC::CHICKEN_GREEN, res);

	//�j���g����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "ChickenYellow.mv1");
	resourcesMap_.emplace(SRC::CHICKEN_YELLOW, res);
}

void ResourceManager::ResourceStage(void)
{
	//*********************************************
	// �X�e�[�W���g�����\�[�X
	//*********************************************

	//���\�[�X�������ݗp
	Resource res;
}

void ResourceManager::ResourceRule(void)
{
	//*********************************************
	// ���[�����ʂŎg�����\�[�X
	//*********************************************

	//���\�[�X�������ݗp
	Resource res;
}

void ResourceManager::ResourceScore(void)
{
	//*********************************************
	// �X�R�A���[�����g�����\�[�X
	//*********************************************

	//���\�[�X�������ݗp
	Resource res;
}

void ResourceManager::ResourceLife(void)
{
	//*********************************************
	// �̗̓��[�����g�����\�[�X
	//*********************************************

	//���\�[�X�������ݗp
	Resource res;
}

void ResourceManager::ResourceTileBraek(void)
{
	//*********************************************
	// ���j�󃋁[�����g�����\�[�X
	//*********************************************

	//���\�[�X�������ݗp
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
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
