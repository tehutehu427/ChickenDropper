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
		CLOUD,			//雲
		TITLE_LOGO,		//タイトルロゴ
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
	//敵が使うリソース
	void ResourceEnemy(void);

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

