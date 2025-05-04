//#pragma once

#include"../Utility/Utility.h"
#include"../Common/Vector2F.h"
#include"../Common/CommonData.h"

class SoundManager;
class SceneGame;

class Stage
{
public:
	//列挙型
	//-------------------
	enum class TILE_STATE
	{
		NORMAL		//通常
		,ATTACK		//攻撃
		,BROKEN		//破壊
		,PREEND		//終了前
		,END		//終了
		,MAX
	};

	enum class TILE_TYPE
	{
		PAT_1_1			//パターン1-1
		,PAT_1_2		//パターン1-2
		, P1_PAT_1_1	//プレイヤー1のパターン1-1
		, P1_PAT_1_2	//プレイヤー1のパターン1-2
		, P2_PAT_1_1	//プレイヤー2のパターン1-1
		, P2_PAT_1_2	//プレイヤー2のパターン1-2
		, P3_PAT_1_1	//プレイヤー3のパターン1-1
		, P3_PAT_1_2	//プレイヤー3のパターン1-2
		, P4_PAT_1_1	//プレイヤー4のパターン1-1
		, P4_PAT_1_2	//プレイヤー4のパターン1-2
		, PAT_1_1_END	//パターン1-1のEND
		, PAT_1_2_END	//パターン1-2のEND
		,MAX
	};

	//定数
	//-------------------
	//タイル
	static constexpr float TILE_SIZE_X = 64.0f;				//タイルの横サイズ
	static constexpr float TILE_SIZE_Y = 20.0f;				//タイルの縦サイズ
	static constexpr float TILE_SIZE_Z = 64.0f;				//タイルの奥行サイズ

	static constexpr float TILE_HSIZE_X = TILE_SIZE_X / 2;	//タイルの横サイズの半分
	static constexpr float TILE_HSIZE_Y = TILE_SIZE_Y / 2;	//タイルの縦サイズの半分
	static constexpr float TILE_HSIZE_Z = TILE_SIZE_Z / 2;	//タイルの奥行サイズの半分

	static constexpr int TILE_NUM = 9;						//１ラインのタイル数

	static constexpr int TILE_HNUM = TILE_NUM / 2;			//１ラインのタイル数の半分

	static constexpr float TILE_BREAK_TIME = 5.0f;				//タイルが壊れるまでの時間
	static constexpr float TILE_REPAIR_TIME = 90.0f;			//タイル修復までの時間
	static constexpr float TILE_PREEND_TIME = 5.0f;				//タイル終了前までの時間
	static constexpr float TILE_END_TIME = 60.0f;				//タイル終了までの時間

	//ステージ
	static constexpr float STAGE_SIZE_X = TILE_SIZE_X * TILE_NUM;	//ステージの横サイズ
	static constexpr float STAGE_SIZE_Z = TILE_SIZE_Z * TILE_NUM;	//ステージの縦サイズ

	static constexpr float STAGE_HSIZE_X = STAGE_SIZE_X / 2;		//ステージの横サイズの半分
	static constexpr float STAGE_HSIZE_Z = STAGE_SIZE_Z / 2;		//ステージの縦サイズの半分
	
	static constexpr int STAGE_NARROW_RANGE = 2;														//ステージ縮小範囲
	static constexpr Utility::DIR_3D STAGE_NARROW_DIR = Utility::DIR_3D::RIGHT;					//ステージ縮小開始方向

	//メンバー関数
	//-------------------
	
	//デフォルトコンストラクタ
	Stage(void);					
	//デストラクタ
	~Stage(void);					

	//初期化処理
	bool Init(SceneGame* parent);		
	//更新処理
	void Update(void);					
	//描画処理
	void Draw(void);					
	//解放処理
	bool Release(void);					

	/// <summary>
	/// タイルの状態遷移処理
	/// </summary>
	/// <param name="ix">Xのタイル番号</param>
	/// <param name="iz">Zのタイル番号</param>
	/// <param name="_stateSwap">変化させる状態</param>
	void TileSwap(int ix,int iz , TILE_STATE _stateSwap);

	/// <summary>
	/// 攻撃したプレイヤーの番号をタイルに与える
	/// </summary>
	/// <param name="ix">Xのタイル番号</param>
	/// <param name="iz">Zのタイル番号</param>
	/// <param name="_charaNum">攻撃したプレイヤー</param>
	void TileAttackPlayer(int ix, int iz, CommonData::TYPE _charaNum);

	/// <summary>
	/// そのタイルを攻撃したプレイヤーの番号を返す
	/// </summary>
	/// <param name="ix">Xのタイル番号</param>
	/// <param name="iz">Zのタイル番号</param>
	/// <returns>攻撃したプレイヤーの番号</returns>
	CommonData::TYPE GetTileAttackPlayer(int ix, int iz);

	/// <summary>
	/// 攻撃時間を返す
	/// </summary>
	/// <param name="ix">Xのタイル番号</param>
	/// <param name="iz">Zのタイル番号</param>
	/// <returns></returns>
	float GetAtkCnt(int ix, int iz);					

	/// <summary>
	/// 攻撃時間を与える
	/// </summary>
	/// <param name="ix">Xのタイル番号</param>
	/// <param name="iz">Zのタイル番号</param>
	/// <param name="_cnt">与える時間</param>
	void SetAtkCnt(int ix, int iz, float _cnt);			
	
	//1ラインのタイル数を返す
	int GetTileHNum(void);				

	/// <summary>
	/// タイルの座標を返す
	/// </summary>
	/// <param name="ix">Xのタイル番号</param>
	/// <param name="iz">Zのタイル番号</param>
	/// <returns></returns>
	VECTOR GetPos(int ix, int iz);		
	
	/// <summary>
	/// タイルの状態を返す
	/// </summary>
	/// <param name="ix">Xのタイル番号</param>
	/// <param name="iz">Zのタイル番号</param>
	/// <returns></returns>
	TILE_STATE GetTileState(int ix, int iz);		

	/// <summary>
	/// タイルの番地をboolで返す
	/// </summary>
	/// <param name="ix">Xのタイル番号</param>
	/// <param name="iz">Zのタイル番号</param>
	/// <returns>その番地をtrueで返す</returns>
	bool GetTileAdd(int ix, int iz);

private:
	//ポインタ
	//-------------------
	SceneGame* sceneGame_;		//ゲームシーンのポインタ

	SoundManager* se_;			//SEのポインタ

	//メンバー変数
	//--------------------------
	int backImage_;				//背景画像の格納領域
	int cloudImage_;			//背景の雲画像の格納領域
	Vector2F cloudPos_;			//雲画像の座標

	int model_[static_cast<int>(TILE_TYPE::MAX)];		//ステージのモデル格納領域
	int map_[TILE_NUM][TILE_NUM][static_cast<int>(TILE_TYPE::MAX)];						//ステージの格納領域
	
	VECTOR pos_[TILE_NUM][TILE_NUM];		//タイルの座標
	VECTOR scale_;							//タイルの大きさ	
	VECTOR rot_;							//タイルの回転

	TILE_STATE tileState_[TILE_NUM][TILE_NUM];		//タイルの状態格納領域

	CommonData::TYPE attackPlayer_[TILE_NUM][TILE_NUM];	//タイルに攻撃したプレイヤー

	float tileSwapTime_[TILE_NUM][TILE_NUM];		//タイルの状態遷移カウント
	float swapTimeCompare_[TILE_NUM][TILE_NUM];		//状態遷移カウントの時間調整用

	float atkCnt_[TILE_NUM][TILE_NUM];				//攻撃から攻撃終わりまでの計測時間

	int endCnt_;						//END状態にするためのカウンタ

	int endTileNumX_;					//END状態にするタイルのX番号
	int endTileNumZ_;					//END状態にするタイルのZ番号
	Utility::DIR_3D endMoveDir_;	//END状態にしていく方向

	int endTileNum_;					//消したタイルの数
	int stageNarrowRange_;				//ステージ縮小カウンタ

	//メンバー関数
	//--------------------------

	//ステージを狭める
	void NarrowStage(void);
};