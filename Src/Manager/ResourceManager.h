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
		TITLE_CLOUD,	//タイトル雲
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

		//ゲームシーン
		P1_INDICATION,		//P1の表示
		P2_INDICATION,		//P2の表示
		P3_INDICATION,		//P3の表示
		P4_INDICATION,		//P4の表示

		//ステージ
		GAME_BACK,			//ゲーム背景
		GAME_CLOUD,			//ゲーム雲
		TILE_PAT_1,			//タイルパターン1
		TILE_PAT_2,			//タイルパターン2
		TILE_PAT_1_BLUE,	//タイルパターン1青 
		TILE_PAT_2_BLUE,	//タイルパターン2青
		TILE_PAT_1_RED,		//タイルパターン1赤 
		TILE_PAT_2_RED,		//タイルパターン2赤
		TILE_PAT_1_GREEN,	//タイルパターン1緑 
		TILE_PAT_2_GREEN,	//タイルパターン2緑
		TILE_PAT_1_YELLOW,	//タイルパターン1黄 
		TILE_PAT_2_YELLOW,	//タイルパターン2黄
		TILE_PAT_1_END,		//タイルパターン1終 
		TILE_PAT_2_END,		//タイルパターン2終

		//ルール共通
		STATE_BLUE,			//ステータス欄青
		STATE_RED,			//ステータス欄赤
		STATE_GREEN,		//ステータス欄緑
		STATE_YELLOW,		//ステータス欄黄
		NARROW_INFO,		//縮小警告

		//スコアルール
		KILL_SCORE,			//撃墜数

		//体力ルール
		LIFE_BLUE,			//体力青
		LIFE_RED,			//体力赤
		LIFE_GREEN,			//体力緑
		LIFE_YELLOW,		//体力黄

		//床破壊数ルール
		TILE_NUM,			//床数

		//リザルト
		RESULT_BACK,		//リザルト背景
		CURTAIN_BACK,		//舞台幕背景
		CURTAIN,			//舞台幕
		RANK_1ST,			//1位
		RANK_2ND,			//2位
		RANK_3RD,			//3位
		RANK_4TH,			//4位

		//プレイヤー
		CHICKEN_BLUE,		//ニワトリ青
		CHICKEN_RED,		//ニワトリ赤
		CHICKEN_GREEN,		//ニワトリ緑
		CHICKEN_YELLOW,		//ニワトリ黄

		//BGM
		TITLE_BGM,					//タイトルBGM
		SELECT_BGM,					//セレクトBGM
		BATTLE_FIRST_HALF_BGM,		//バトル前半BGM
		BATTLE_SECOND_HALF_BGM,		//バトル後半BGM
		RESULT_BGM,					//リザルトBGM

		//SE
		CLICK_SE,			//決定SE
		CANCEL_SE,			//キャンセルSE
		FALL_SE,			//落下SE
		ATTACK_SE,			//攻撃SE
		TILEBREAK_SE,		//床破壊SE
		CURTAIN_SE,			//舞台幕SE
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);
	//タイトルで使うリソース
	void InitTitle(void);
	//ゲームオーバーで使うリソース
	void InitSelect(void);
	//ゲームで使うリソース
	void InitGame(void);
	//ゲームクリアで使うリソース
	void InitResult(void);

	//プレイヤーが使うリソース
	void ResourcePlayer(void);

	//ステージが使うリソース
	void ResourceStage(void);

	//ルール共通で使うリソース
	void ResourceRule(void);

	//スコアルールで使うリソース
	void ResourceScore(void);

	//体力ルールで使うリソース
	void ResourceLife(void);

	//床破壊数ルールで使うリソース
	void ResourceTileBraek(void);

	//BGMリソース
	void ResourceBGM(void);

	//SEリソース
	void ResourceSE(void);

	//サウンド全体リソース
	void ResourceSound(void);

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

	//

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

};

