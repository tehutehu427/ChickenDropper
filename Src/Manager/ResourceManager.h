#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		//************************************************************
		// ここに保存しておきたい画像やモデル、音源などの名前を記す
		//************************************************************
		
		//タイトル
		TITLE_BACK,		//タイトル背景
		CLOUD,			//タイトル雲
		TITLE_LOGO,		//タイトルロゴ

		//セレクト
		SELECT_BACK,		//セレクト背景
		SELECT_SKY,			//セレクト空
		SELECT_CLOUD,		//セレクト用雲
		BATTLE_P1C1,		//プレイヤー1人 CPU1人
		BATTLE_P1C2,		//プレイヤー1人 CPU2人
		BATTLE_P1C3,		//プレイヤー1人 CPU3人
		BATTLE_P2,			//プレイヤー2人
		BATTLE_P2C1,		//プレイヤー2人 CPU1人
		BATTLE_P2C2,		//プレイヤー2人 CPU2人
		BATTLE_P3,			//プレイヤー3人
		BATTLE_P3C1,		//プレイヤー3人 CPU1人
		BATTLE_P4,			//プレイヤー4人
		ARROW,				//矢印
		PAD_NUM,			//パッド数確認
		EASY,				//難易度 簡単
		NORMAL,				//難易度 普通
		HARD,				//難易度 難しい
		RULE_SCORE,			//スコア制
		RULE_LIFE,			//ライフ制
		RULE_TILE,			//タイル破壊制
		SCORE_EXPLAIN,		//スコア制説明
		LIFE_EXPLAIN,		//ライフ制説明
		TILE_EXPLAIN,		//タイル破壊制説明

		//プレイヤー
		CHICKEN_BLUE,		//ニワトリ青
		CHICKEN_RED,		//ニワトリ赤
		CHICKEN_GREEN,		//ニワトリ緑
		CHICKEN_YELLOW,		//ニワトリ黄
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);
	//タイトルで使うリソース初期化
	void InitTitle(void);
	//ゲームオーバーで使うリソース初期化
	void InitSelect(void);
	//ゲームで使うリソース初期化
	void InitGame(void);
	//ゲームクリアで使うリソース初期化
	void InitResult(void);

	//プレイヤーが使うリソース
	void ResourcePlayer(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

};

