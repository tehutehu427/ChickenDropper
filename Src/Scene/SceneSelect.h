#pragma once

#include<DxLib.h>
#include<map>
#include<functional>
#include"../Common/Vector2.h"
#include"../Common/Vector2F.h"
#include"../Common/CommonData.h"
#include"SceneBase.h"

class SoundManager;

class SceneSelect : public SceneBase
{
public:
	//列挙型
	//-------------------------------

	//決める項目
	enum class SELECT_ITEM
	{
		BATTLE_PATTERN		//対戦人数
		,CHECK_PAD			//パッドの確認
		,DIFFICULTY			//CPUの強さを決める
		,RULE				//ルール
	};

	//定数
	//-------------------------------
	
	//コントローラーのバイブレーションの強さ
	static constexpr int VIB_STR = 80;
	//コントローラーのバイブレーションの時間(ミリ秒)
	static constexpr int VIB_TIME = 1 * 1000;

	//コントローラーの選択権
	static constexpr int SELECT_PAD_P1 = 1;
	static constexpr int SELECT_KEYBOARD = 0;

	//カーソルの大きさ
	static constexpr int CURSOR_SIZE = 10;		

	//看板の大きさ
	static constexpr int SIGNBOARD_SIZE_X = 1536;
	static constexpr int SIGNBOARD_SIZE_Y = 864;

	//ボードの大きさ
	static constexpr int BOARD_SIZE_X = 1216;
	static constexpr int BOARD_SIZE_Y = 608;

	//ボードの高さ
	static constexpr int BOARD_HEIGHT = 170;

	//人数画像の大きさ
	static constexpr int BATTLE_NUM_SIZE_X = 400;
	static constexpr int BATTLE_NUM_SIZE_Y = 500;

	static constexpr int BATTLE_PAT_SHIFT_POS_X = 148;
	static constexpr int BATTLE_PAT_SHIFT_POS_Y = 60;

	static constexpr int PADNUM_SHIFT_POS_X = 40;
	static constexpr int PADNUM_SHIFT_POS_Y = 50;

	//矢印画像の大きさ
	static constexpr float ARROW_DEFAULT_SIZE = 1.0f;
	static constexpr float ARROW_CHANGE_SIZE = 10.0f;
	static constexpr float ARROW_CHANGE_SIZE_SPEED = 5.0f;
	static constexpr int ARROW_SHIFT_POS_X = 80;
	static constexpr int ARROW_SHIFT_POS_Y = 200;

	//難易度画像の大きさ
	static constexpr int DIFFICULTY_SIZE_X = 400;
	static constexpr int DIFFICULTY_SIZE_Y = 500;
	
	static constexpr int DIFFICULTY_SHIFT_SIZE_X = 218;
	static constexpr int DIFFICULTY_SHIFT_SIZE_Y = 60;

	//ルール画像の大きさ
	static constexpr int RULE_SIZE_X = 320;
	static constexpr int RULE_SIZE_Y = 200;

	//ルールの項目間
	static constexpr int RULE_DIS_X = 40;		
	static constexpr int RULE_DIS_Y = 80;		

	//音量(SE)
	//-----------------

	//決定音
	static constexpr int CLICK_VOLUME = 70;
	
	//キャンセル音
	static constexpr int CANCEL_VOLUME = 500;

	//メンバー関数
	//-------------------------------

	//コンストラクタ
	SceneSelect();

	//デストラクタ
	~SceneSelect()override;

	//初期化処理
	bool Init(void) override;		
	//更新処理
	void Update(void) override;		
	//描画処理
	void Draw(void) override;		
	//解放処理
	bool Release(void) override;	

private:
	//背景
	enum class BACKGROUND_TYPE
	{
		BACK	//背景
		, SKY	//空
		, CLOUD	//雲
		, MAX
	};

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

	//プレイヤーのアニメーション速度
	static constexpr float SPEED_ANIM = 60.0f;

	//インスタンス
	//------------------
	SoundManager* sound_;

	//メンバー変数
	//-------------------------------
	
	int fontHandle_;				//フォント用
	int padFont_;					//パッド数のフォント用

	SELECT_ITEM selectItem_;		//現在の選択項目
	Vector2F cursorPos_;			//カーソルの位置
	int selectPadNum_;				//選択されているパッドの番号
	bool isVib_;					//バイブレーションのフラグ
	int playerNum_;					//参加プレイヤーの数
	CommonData::CPU_NUM cpuNum_;	//CPUの数
	int compareCpuNum_;				//CPUの数比較用

	int pModel_[static_cast<int>(CommonData::TYPE::P4)];	//プレイヤーのモデル
	VECTOR pScl_;											//プレイヤーの大きさ
	VECTOR pRot_;											//プレイヤーの回転
	VECTOR pPos_;											//プレイヤーの位置
	float stepAnim_;												//再生中のアニメーション時間
	float speedAnim_;												//アニメーション速度
	float animTotalTime_[static_cast<int>(CommonData::TYPE::P4)];	//アニメーションの総再生時間
	int animAttachNo_[static_cast<int>(CommonData::TYPE::P4)];		//アタッチしているアニメーション

	CommonData::BATTLE_PATTERN selectPattern_;	//現在選択中の対戦人数
	CommonData::RULE selectRule_;				//現在選択中のルール
	CommonData::DIFFICULTY selectDifficulty_;	//現在選択中の難易度

	std::map<SELECT_ITEM, std::function<void(void)>> itemUpdate_;	//項目ごとの更新
	std::map<SELECT_ITEM, std::function<void(void)>> itemDraw_;		//項目ごとの描画
	
	//画像系

	//背景画像の格納領域
	std::map<BACKGROUND_TYPE, int> bgImages_;
	Vector2F cloudPos_;

	//対戦人数の画像
	std::map<CommonData::BATTLE_PATTERN, int> bpImages_;

	//矢印画像
	int arrowImage_;

	//時間計測
	float step_;

	//矢印の大きさ
	float arrowSize_;

	//コントローラー数の画像用
	int padNumImage_;

	//難易度の画像
	std::map<CommonData::DIFFICULTY, int> diffImages_;

	//ルールの画像
	std::map<CommonData::RULE,int> ruleImages_;	

	//ルール説明の画像
	std::map<CommonData::RULE,int> ruleExplainImages_;	

	//チュートリアル用画像
	int tutorialImage_;

	//画像の位置
	Vector2F imgsPos_;					

	float distance_;				//項目間の距離		

	/// <summary>
	/// カーソル描画
	/// </summary>
	/// <param name="_sizeX">カーソルをあてる画像のXサイズ</param>
	/// <param name="_sizeY">カーソルをあてる画像のYサイズ</param>
	void CursorDraw(int _sizeX, int _sizeY);

	//項目ごとの更新
	void BattlePatternUpdate(void);
	void CheckPadUpdate(void);
	void DifficultyUpdate(void);
	void RuleUpdate(void);

	//項目ごとの描画
	void BattlePatternDraw(void);
	void CheckPadDraw(void);
	void DifficultyDraw(void);
	void RuleDraw(void);
};