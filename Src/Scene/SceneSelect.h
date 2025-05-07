#pragma once

#include<DxLib.h>
#include<map>
#include<functional>
#include"../Common/Vector2.h"
#include"../Common/Vector2F.h"
#include"../Common/CommonData.h"
#include"../Application.h"
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

	//定数
	//-------------------------------

	//コントローラー
	static constexpr int VIB_STR = 80;			//コントローラーのバイブレーションの強さ
	static constexpr int VIB_TIME = 1 * 1000;	//コントローラーのバイブレーションの時間(ミリ秒)
	static constexpr int SELECT_PAD_P1 = 1;		//コントローラーの選択権(パッド)
	static constexpr int SELECT_KEYBOARD = 0;	//コントローラーの選択権(キーボード)

	//カーソル
	static constexpr int CURSOR_SIZE = 10;				//カーソルの大きさ
	static constexpr int CURSOR_BLINKING_INTERVAL = 20;	//カーソルの点滅間隔

	//看板
	static constexpr int SIGNBOARD_SIZE_X = 1536;	//看板の横の大きさ
	static constexpr int SIGNBOARD_SIZE_Y = 864;	//看板の縦の大きさ

	//ボード
	static constexpr int BOARD_SIZE_X = 1216;	//ボードの横の大きさ
	static constexpr int BOARD_SIZE_Y = 608;	//ボードの縦の大きさ
	static constexpr int BOARD_HEIGHT = 170;	//ボードの高さ

	//対戦人数画像
	static constexpr int BATTLE_NUM_SIZE_X = 400;		//対戦人数画像の横の大きさ
	static constexpr int BATTLE_NUM_SIZE_Y = 500;		//対戦人数画像の縦の大きさ
	static constexpr int BATTLE_PAT_SHIFT_POS_X = 148;	//対戦人数画像の横の位置
	static constexpr int BATTLE_PAT_SHIFT_POS_Y = 60;	//対戦人数画像の縦の位置
	static constexpr float BATTLE_RELATIVE_POS_X =		//対戦人数画像用の横の相対座標
		BOARD_SIZE_X / 2 - BATTLE_NUM_SIZE_X / 1.5;		

	//パッド数画像
	static constexpr int PADNUM_SHIFT_POS_X = 40;	//パッド数画像の横の位置
	static constexpr int PADNUM_SHIFT_POS_Y = 50;	//パッド数画像の縦の位置
	static constexpr int PADNUM_FONT_SIZE = 70;		//パッド数のフォントの大きさ
	static constexpr int PADNUM_FONT_TICKNESS = 3;	//パッド数のフォントの太さ
	static constexpr float STRING_POS_X =			//文字の横の表示位置
		PADNUM_FONT_SIZE * 5.5f;
	static constexpr float STRING_POS_Y =			//文字の縦の表示位置
		Application::SCREEN_SIZE_Y / 1.5f;

	//矢印画像
	static constexpr float ARROW_DEFAULT_SIZE = 1.0f;		//矢印画像の大きさ
	static constexpr float ARROW_CHANGE_SIZE = 10.0f;		//矢印画像の変化後の大きさ
	static constexpr float ARROW_CHANGE_SIZE_SPEED = 5.0f;	//矢印画像の大きさ変化の速度
	static constexpr int ARROW_SHIFT_POS_X = 80;			//矢印画像の横の位置
	static constexpr int ARROW_SHIFT_POS_Y = 200;			//矢印画像の縦の位置

	//難易度画像
	static constexpr int DIFFICULTY_SIZE_X = 400;			//難易度画像の横の大きさ
	static constexpr int DIFFICULTY_SIZE_Y = 500;			//難易度画像の縦の大きさ
	static constexpr int DIFFICULTY_SHIFT_POS_X = 218;		//難易度画像の横の位置
	static constexpr int DIFFICULTY_SHIFT_POS_Y = 60;		//難易度画像の縦の位置

	//ルール画像
	static constexpr int RULE_SIZE_X = 320;	//ルール画像の横の大きさ
	static constexpr int RULE_SIZE_Y = 200;	//ルール画像の縦の大きさ
	static constexpr int RULE_DIS_X = 40;	//ルールの項目の横の間
	static constexpr int RULE_DIS_Y = 80;	//ルールの項目の縦の間
	
	//フォント
	static constexpr int PAD_FONT_SIZE = 30;		//パッド数用フォントの大きさ
	static constexpr int PAD_FONT_TICKNESS = 10;	//パッド数用フォントの太さ

	//音
	static constexpr int CLICK_VOLUME = 70;		//決定音
	static constexpr int CANCEL_VOLUME = 500;	//キャンセル音

	//プレイヤー
	static constexpr float SPEED_ANIM = 60.0f;	//プレイヤーのアニメーション速度

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

	//画像の位置
	Vector2F imgsPos_;					

	float distance_;				//項目間の距離		

	/// <summary>
	/// カーソル描画
	/// </summary>
	/// <param name="_sizeX">カーソルをあてる画像のXサイズ</param>
	/// <param name="_sizeY">カーソルをあてる画像のYサイズ</param>
	void CursorDraw(const int _sizeX, const int _sizeY);

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