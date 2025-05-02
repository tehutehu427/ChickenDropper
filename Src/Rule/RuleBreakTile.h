#pragma once
#include"RuleBase.h"

class RuleBreakTile : public RuleBase
{
public:
	//定数
	//-------------------

	//制限時間	
	static constexpr DATEDATA TIME_LIMIT = { 0,0,0,0,1,30 };

	//ステージ縮小開始時間
	static constexpr DATEDATA NARROW_LIMIT = { 0,0,0,0,0,30 };

	//スコアとタイルイメージの距離
	static constexpr int SCORE_2_TILE_DIS = 15;

	//メンバー関数
	//-------------------

	//コンストラクタ
	RuleBreakTile(SceneGame* parent);

	//初期化処理
	bool Init(void) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;

private:

	//破壊したタイル数画像
	int tileNumImage_;
	
	/// <summary>
	/// 壊したタイルの数を比較
	/// </summary>
	/// <param name="_player">比較するプレイヤー</param>
	void BrokeTimeNumCompare(Player* _player);

	/// <summary>
	/// 壊したタイルの数が同じか比較
	/// </summary>
	/// <param name="_player">比較するプレイヤー</param>
	void IsSameBrokeTileNum(Player* _player);
};