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

	//雲
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "cloud.png");
	resourcesMap_.emplace(SRC::TITLE_CLOUD, res);

	//タイトルロゴ
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

	//セレクト雲
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

	//P1の表示
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "P1.png");
	resourcesMap_.emplace(SRC::P1_INDICATION, res);

	//P2の表示
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "P2.png");
	resourcesMap_.emplace(SRC::P2_INDICATION, res);

	//P3の表示
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "P3.png");
	resourcesMap_.emplace(SRC::P3_INDICATION, res);

	//P4の表示
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "P4.png");
	resourcesMap_.emplace(SRC::P4_INDICATION, res);

	//プレイヤー
	ResourcePlayer();

	//ステージ
	ResourceStage();
}

void ResourceManager::InitResult(void)
{
	//*********************************************
	// リザルトシーンで使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	//リザルト背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ResultBack.png");
	resourcesMap_.emplace(SRC::RESULT_BACK, res);

	//舞台幕背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ResultBackCurtain.png");
	resourcesMap_.emplace(SRC::CURTAIN_BACK, res);
	
	//舞台幕
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Curtain.png");
	resourcesMap_.emplace(SRC::CURTAIN, res);

	//1位
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "1stImage.png");
	resourcesMap_.emplace(SRC::RANK_1ST, res);

	//2位
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "2ndImage.png");
	resourcesMap_.emplace(SRC::RANK_2ND, res);

	//3位
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "3rdImage.png");
	resourcesMap_.emplace(SRC::RANK_3RD, res);

	//4位
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "4thImage.png");
	resourcesMap_.emplace(SRC::RANK_4TH, res);

	//プレイヤー
	ResourcePlayer();
}

void ResourceManager::ResourcePlayer(void)
{
	//*********************************************
	// プレイヤーが使うリソース
	//*********************************************

	//リソース書き込み用
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

void ResourceManager::ResourceStage(void)
{
	//*********************************************
	// ステージが使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	//ゲーム背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleBack.png");
	resourcesMap_.emplace(SRC::GAME_BACK, res);

	//雲
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "cloud.png");
	resourcesMap_.emplace(SRC::GAME_CLOUD, res);

	//タイルパターン1
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-1.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_1, res);

	//タイルパターン2
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-2.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_2, res);

	//タイルパターン1青
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-1-Blue.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_1_BLUE, res);

	//タイルパターン2青
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-2-Blue.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_2_BLUE, res);

	//タイルパターン1赤
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-1-Red.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_1_RED, res);

	//タイルパターン2赤
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-2-Red.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_2_RED, res);

	//タイルパターン1緑
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-1-Green.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_1_GREEN, res);

	//タイルパターン2緑
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-2-Green.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_2_GREEN, res);

	//タイルパターン1黄
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-1-Yellow.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_1_YELLOW, res);

	//タイルパターン2黄
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-2-Yellow.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_2_YELLOW, res);

	//タイルパターン1終
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-1-END.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_1_END, res);

	//タイルパターン2終
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "TilePat1-2-END.mv1");
	resourcesMap_.emplace(SRC::TILE_PAT_2_END, res);
}

void ResourceManager::ResourceRule(void)
{
	//*********************************************
	// ルール共通で使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	//ステータス欄青
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "State Blue.png");
	resourcesMap_.emplace(SRC::STATE_BLUE, res);

	//ステータス欄赤
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "State Red.png");
	resourcesMap_.emplace(SRC::STATE_RED, res);

	//ステータス欄緑
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "State Green.png");
	resourcesMap_.emplace(SRC::STATE_GREEN, res);

	//ステータス欄黄
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "State Yellow.png");
	resourcesMap_.emplace(SRC::STATE_YELLOW, res);

	//縮小警告
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "NarrowInfo.png");
	resourcesMap_.emplace(SRC::NARROW_INFO, res);
}

void ResourceManager::ResourceScore(void)
{
	//*********************************************
	// スコアルールが使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	//撃墜数
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "killScore.png");
	resourcesMap_.emplace(SRC::KILL_SCORE, res);

	//ルール共通
	ResourceRule();
}

void ResourceManager::ResourceLife(void)
{
	//*********************************************
	// 体力ルールが使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	//体力青
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Life Blue.png");
	resourcesMap_.emplace(SRC::LIFE_BLUE, res);

	//体力赤
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Life Red.png");
	resourcesMap_.emplace(SRC::LIFE_RED, res);

	//体力緑
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Life Green.png");
	resourcesMap_.emplace(SRC::LIFE_GREEN, res);

	//体力黄
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Life Yellow.png");
	resourcesMap_.emplace(SRC::LIFE_YELLOW, res);

	//ルール共通
	ResourceRule();
}

void ResourceManager::ResourceTileBraek(void)
{
	//*********************************************
	// 床破壊ルールが使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	//床数
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TileNum.png");
	resourcesMap_.emplace(SRC::TILE_NUM, res);

	//ルール共通
	ResourceRule();
}

void ResourceManager::ResourceBGM(void)
{
	//*********************************************
	// BGMが使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	//タイトルBGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "title.mp3");
	resourcesMap_.emplace(SRC::TITLE_BGM, res);

	//セレクトBGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Select.wav");
	resourcesMap_.emplace(SRC::SELECT_BGM, res);

	//バトル前半BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "BattleFirstHalf.mp3");
	resourcesMap_.emplace(SRC::BATTLE_FIRST_HALF_BGM, res);

	//バトル後半BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "BattleSecondHalf.mp3");
	resourcesMap_.emplace(SRC::BATTLE_SECOND_HALF_BGM, res);

	//リザルトBGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Result.mp3");
	resourcesMap_.emplace(SRC::RESULT_BGM, res);
}

void ResourceManager::ResourceSE(void)
{
	//*********************************************
	// SEが使うリソース
	//*********************************************

	//リソース書き込み用
	Resource res;

	//決定SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Click.mp3");
	resourcesMap_.emplace(SRC::CLICK_SE, res);

	//キャンセルSE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Cancel.mp3");
	resourcesMap_.emplace(SRC::CANCEL_SE, res);

	//落下SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Fall.mp3");
	resourcesMap_.emplace(SRC::FALL_SE, res);

	//攻撃SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Attack.mp3");
	resourcesMap_.emplace(SRC::ATTACK_SE, res);

	//床破壊SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "TileBreak.mp3");
	resourcesMap_.emplace(SRC::TILEBREAK_SE, res);

	//舞台幕SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "CurtainOpen.mp3");
	resourcesMap_.emplace(SRC::CURTAIN_SE, res);
}

void ResourceManager::ResourceSound(void)
{
	//サウンド系リソース
	ResourceBGM();
	ResourceSE();
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
