#pragma once

#include"../Scene/SceneGame.h"
#include"../Common/Vector2F.h"

class RuleBase
{
public:
	//定数
	//-------------------------
	
	//1秒(ミリ秒単位)
	static constexpr int ONE_SEC = 1000;

	//タイマーの大きさ倍率
	static constexpr float TIME_SIZE_PER = 5.0f / 3.0f;

	//カウントの大きさ
	static constexpr int TIME_SIZE = 50;

	//カウントの太さ
	static constexpr int TIME_THICKNESS = 10;

	//スコアの大きさ
	static constexpr int SCORE_SIZE = 25;

	//スコアの太さ
	static constexpr int SCORE_THICKNESS = 8;

	//秒数制限
	static constexpr int SEC_LIMIT = 59;
	static constexpr int SEC_2_MIN = 60;

	//ステータス欄の大きさ
	static constexpr int STATE_SIZE_X = 200;
	static constexpr int STATE_SIZE_Y = 50;

	//ステータス欄の画面からの位置
	static constexpr int STATE_POS_X = 230;
	static constexpr int STATE_POS_Y = 50;

	//ステータス欄の距離間
	static constexpr int STATE_DIS = 100;

	//カウンタを考慮したステータス欄の距離
	static constexpr int STATE_DIS_TO_TIME = 180;

	//範囲縮小警告画像の大きさ
	static constexpr int NARROW_INFO_SIZE_X = 600;
	static constexpr int NARROW_INFO_SIZE_Y = 200;

	//範囲縮小警告の出ている時間
	static constexpr float NARROW_INFO_TIME = Utility::DEFAULT_FPS * 5.0f;

	//メンバー関数
	//-------------------------

	//デフォルトコンストラクタ
	RuleBase(SceneGame* parent);
	//デストラクタ
	virtual ~RuleBase(void);

	//初期化処理
	virtual bool Init(void);
	//更新処理
	virtual void Update(void);
	//描画処理
	virtual void Draw(void);
	//解放処理
	virtual bool Release(void);

	/// <summary>
	/// ステージ縮小のフラグを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>縮小の有効判定(true:縮小する)</returns>
	const bool GetIsNarrowStage(void);

protected:
	//メンバー変数
	//-------------------------

	//ゲームシーンのポインタ
	SceneGame* sceneGame_;

	//タイマー関係
	DATEDATA timeLimit_;		//制限時間カウント
	int timeFont_;				//タイマーのフォント
	int timeFontSize_;			//タイマーのフォントの大きさ
	int timeFontThickness_;		//タイマーのフォントの太さ
	int secCount_;				//秒数数え用

	//スコア
	int scoreFont_;				//スコアのフォント
	int scoreFontSize_;			//スコアのフォントの大きさ
	int scoreFontThickness_;	//スコアのフォントの太さ

	//ステータス
	int stateImages_[static_cast<int>(CommonData::TYPE::P4)];	//ステータス欄
	Vector2F statePos_;											//ステータスの座標

	//ステージ縮小関係
	bool isNarrowStage_;		//ステージ縮小のフラグ(true:縮小する)
	int narrowInfoImage_;		//縮小警告画像
	float narrowInfoPos_;		//縮小警告の座標
	float narrowInfoTime_;		//縮小警告の表示時間

	//順位の初期化
	void InitRank(void);
};

