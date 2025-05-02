#pragma once
#include"RuleBase.h"

class RuleLife : public RuleBase
{
public:
	
	//ライフパネルの大きさ
	static constexpr int LIFE_SIZE_X = 25;
	static constexpr int LIFE_SIZE_Y = 40;

	//ライフパネルと枠の座標合わせ
	static constexpr int LIFE_TO_STATE_POS = 10;

	//ライフパネルの間隔
	static constexpr int LIFE_DIS = 3;

	//ステージ縮小開始時間
	static constexpr DATEDATA NARROW_LIMIT = { 0,0,0,0,2,0 };

	//メンバー関数
	//-------------------

	//コンストラクタ
	RuleLife(SceneGame* parent);

	//初期化処理
	bool Init(void) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;

private:

	//ライフパネル
	int lifeImages_[static_cast<int>(CommonData::TYPE::P4)];

	//時間経過
	DATEDATA timeCnt_;

	/// <summary>
	/// やられた時間の比較
	/// </summary>
	/// <param name="_player">比較するプレイヤー</param>
	void DeadTimeCompare(Player* _player);

	/// <summary>
	/// やられた時間が同時かを比較
	/// </summary>
	/// <param name="_player">比較するプレイヤー</param>
	void IsSameDeadTime(Player* _player);
};