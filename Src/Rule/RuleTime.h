#pragma once

#include"../Common/CommonData.h"
#include"RuleBase.h"

class Player;

class RuleTime : public RuleBase
{
public:
	//定数
	//-------------------

	//制限時間	
	static constexpr DATEDATA TIME_LIMIT = { 0,0,0,0,2,0 };	

	//ステージ縮小開始時間
	static constexpr DATEDATA NARROW_LIMIT = { 0,0,0,0,1,0 };

	//スコアとタイルイメージの距離
	static constexpr int SCORE_2_KILLSCORE_DIS = 15;

	//メンバー関数
	//-------------------

	//コンストラクタ
	RuleTime(SceneGame* parent);

	//初期化処理
	bool Init(void) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;

private:

	//キルスコアの画像
	int killScoreImage_;

	/// <summary>
	/// プレイヤーのスコア比較
	/// </summary>
	/// <param name="_player">比較するプレイヤー</param>
	void ScoreCompare(Player* _player);		

	/// <summary>
	/// スコアが同じかどうか確かめる
	/// </summary>
	/// <param name="_player">比較するプレイヤー</param>
	void IsSameScore(Player* _player);
};