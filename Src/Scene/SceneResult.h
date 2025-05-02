#pragma once

#include<vector>
#include"../Common/CommonData.h"
#include"SceneBase.h"

class SoundManager;

class SceneResult : public SceneBase
{
public:
	//メンバー関数
	//-------------------

	//初期化処理
	bool Init(void) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//解放処理
	bool Release(void) override;

private:

	//アニメーション番号
	enum class ANIM_NUM
	{
		NONE
		, IDLE		//待機
		, WALK		//歩き
		, ATTACK	//攻撃
		, FALL		//落下
		, RANK1ST	//1位
		, RANK2ND	//2位
		, RANK3RD	//3位
		, RANK4TH	//4位
	};

	//決定音量
	static constexpr int CLICK_VOLUME = 70;

	//カーテン音量
	static constexpr int CURTAIN_VOLUME = 100;

	//プレイヤーの大きさ倍率
	static constexpr float P_SCALE = 0.8f;

	//プレイヤーのアニメーション速度
	static constexpr float SPEED_ANIM = 60.0f;

	//プレイヤー同士の距離
	static constexpr float PL_DIS = 60.0f;

	//順位画像関係
	static constexpr int RANK_SIZE = 45;				//番号の大きさ
	static constexpr int RANK_DIS_Y = 20;				//番号とプレイヤーのY距離
	static constexpr int RANK_DIS_Z = 35;				//番号とプレイヤーのZ距離

	static constexpr VECTOR CURTAIN_INIT_POS = { 0.0f, 78.0f, -160.0f };	//舞台幕の開始座標用

	static constexpr float CURTAIN_SHIFT_POS = 30.0f;	//舞台幕の開始座標のずらし用

	static constexpr float CURTAIN_SPEED = 1.0f;		//舞台幕の移動量

	static constexpr float CURTAIN_FEEDBACK_TIME = NodyUtility::DEFAULT_FPS * 5.0f;	//舞台幕のフィードバックするまでの時間

	//インスタンス
	//---------------

	//サウンドのインスタンス
	SoundManager* sound_;

	int bgImage_;			//背景画像
	int bgCurtainImage_;	//背景の舞台幕画像
	int curtainImage_;		//舞台幕の画像
	bool curtainFlag_;		//舞台幕の開閉フラグ(true:開く)

	VECTOR curtainPos_;		//舞台幕の座標

	float curtainFeedBackTime_;	//舞台幕のフィードバックするまでの時間カウント

	std::vector<int> rank_;		//順位
		
	//リザルトのプレイヤー
	int pModel_[static_cast<int>(CommonData::TYPE::P4)];	//プレイヤーのモデル
	VECTOR pScl_;											//プレイヤーの大きさ
	VECTOR pRot_;											//プレイヤーの回転
	VECTOR pPos_[static_cast<int>(CommonData::TYPE::P4)];	//プレイヤーの位置
	float stepAnim_[static_cast<int>(CommonData::TYPE::P4)];		//再生中のアニメーション時間
	float speedAnim_[static_cast<int>(CommonData::TYPE::P4)];		//アニメーション速度
	float animTotalTime_[static_cast<int>(CommonData::TYPE::P4)];	//アニメーションの総再生時間
	int animAttachNo_[static_cast<int>(CommonData::TYPE::P4)];		//アタッチしているアニメーション

	int  rankImage_[static_cast<int>(CommonData::TYPE::P4)];		//プレイヤー番号の表示用
};

