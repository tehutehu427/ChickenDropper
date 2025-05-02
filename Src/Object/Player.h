#pragma once

#include<DxLib.h>
#include"../Common/CommonData.h"
#include"../Utility/NodyUtility.h"

class SceneGame;
class EffectManager;
class SoundManager;
class Attack;

class Player
{
public:
	//列挙型
	//-------------------
	//プレイヤーかCPUかの判断
	enum class CHARA_JUDGE
	{
		PLAYER		//プレイヤー
		,CPU		//CPU
		,MAX
	};

	//キャラの状態判定
	enum class STATE
	{
		NORMAL		//通常状態
		,ATTACK		//攻撃状態
		,DEAD		//やられ状態
		,MAX
	};

	//アニメーション番号
	enum class ANIM_NUM
	{
		NONE
		,IDLE		//待機
		,WALK		//歩き
		,ATTACK		//攻撃
		,FALL		//落下
		,RANK1ST	//1位
		,RANK2ND	//2位
		,RANK3RD	//3位
		,RANK4TH	//4位
	};

	//定数
	//-------------------
	static constexpr float RADIUS = 24.0;				//プレイヤーの半径

	static constexpr int MOVE_KEY_KEEP_TIME = 8;		//移動に必要なキー押し込み時間(フレーム)				

	static constexpr float SPEED_NORMAL = 4.0f;			//プレイヤーの通常速度

	static constexpr int MAX_LIFE = 5;					//プレイヤーの最大体力数

	static constexpr int DAMAGE = 1;					//基本ダメージ
	static constexpr int SELF_SUB_SCORE = 2;			//自滅点

	static constexpr int SUB_TILE_BROKE_SCORE = 5;		//下げるタイルスコアの割合

	static constexpr float FALL_SPEED = 5.0f;			//落下速度

	static constexpr float INVINCIBLE = 100.0f;			//無敵時間

	static constexpr float SPEED_ANIM = 60.0f;			//アニメーション速度

	static constexpr float ATK_COOLDOWN_CPU_EASY = 180.0f;		//EASYのCPU用の攻撃の再発動時間
	static constexpr float ATK_COOLDOWN_CPU_NORMAL = 100.0f;	//NORMALのCPU用の攻撃の再発動時間
	static constexpr float ATK_COOLDOWN_CPU_HARD = 60.0f;		//HARDのCPU用の攻撃の再発動時間

	static constexpr float CPU_MOVE_INTERVAL_EASY = 60.0f;		//EASYのCPUの移動間隔
	static constexpr float CPU_MOVE_INTERVAL_NORMAL = 40.0f;	//NORMALのCPUの移動間隔
	static constexpr float CPU_MOVE_INTERVAL_HARD = 20.0f;		//HARDのCPUの移動間隔

	static constexpr int CPU_MOVE_INTERVAL_RAND_EASY = 21;		//EASYのCPUの移動間隔をずらす用
	static constexpr int CPU_MOVE_INTERVAL_RAND_NORMAL = 10;	//NORMALのCPUの移動間隔をずらす用
	static constexpr int CPU_MOVE_INTERVAL_RAND_HARD = 7;		//HARDのCPUの移動間隔をずらす用

	static constexpr int NUM_SIZE = 70;				//番号の大きさ
	static constexpr int NUM_DIS = 35;				//番号とプレイヤーの距離

	static constexpr float NUM_VIEW_TIME = 180.0f;	//番号の表示時間
	
	//メンバー関数
	//-------------------
	
	//デフォルトコンストラクタ
	Player(void);					
	//デストラクタ
	~Player(void);					

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="_pos">初期位置</param>
	/// <param name="_charaNum">プレイヤー番号</param>
	/// <returns></returns>
	bool Init(SceneGame* parent, VECTOR _pos, CommonData::TYPE _charaNum);	

	//更新処理
	void Update(void);			

	//描画処理
	void Draw(void);														
	//解放処理
	bool Release(void);														

	//移動制御(プレイヤー)
	void MovePlayer(void);
	//移動制御(CPU)
	void MoveCPU(void);

	//回転制御
	void ProcessRot(void);

	/// <summary>
	/// ダメージ処理の準備
	/// </summary>
	/// <param name="_charaNum">攻撃してきたプレイヤーの番号</param>
	void PreDamage(CommonData::TYPE _charaNum);

	/// <summary>
	/// ダメージ制御
	/// </summary>
	/// <param name="_damage">与えるダメージ又は下げるスコアの量</param>
	void Damage(int _damage);

	/// <summary>
	/// 加点の処理
	/// </summary>
	/// <param name="_score">加点するスコア</param>
	void TakeScore(int _score);

	//現在の得点を返す
	int GetScore(void);

	//攻撃の制御(プレイヤー)
	void ProcessAttackPlayer(void);
	//攻撃の制御(CPU)
	void ProcessAttackCPU(void);
	//攻撃の生成
	void GenerateAttack(void);
	//有効な攻撃を取得
	Attack* GetValidAttack(void);		

	//プレイヤーの状態を返す
	STATE GetState(void);

	/// <summary>
	/// プレイヤーの状態変更
	/// </summary>
	/// <param name="_state">変更したい状態</param>
	void ChangeState(STATE _state);

	//プレイヤーの現在座標を返す
	VECTOR GetPos(void);				

	/// <summary>
	/// プレイヤーの座標を与える
	/// </summary>
	/// <param name="_setPos">与えたいpos</param>
	void SetPos(VECTOR _setPos);		

	//プレイヤーの方向を返す
	NodyUtility::DIR_3D GetDir(void);	

	//プレイヤーが歩いているかを返す
	bool GetIsWalk(void);

	//プレイヤーの体力を返す
	int GetLife(void);

	//プレイヤーのやられた時間を返す
	int GetDeadTime(void);

	//プレイヤーのやられた時間を与える(GetNowCountを与える)
	void SetDeadTime(void);

	//プレイヤーの壊したタイルの数を返す
	int GetBrokeTileNum(void);

	//プレイヤーの壊したタイルの数を増やす
	void IncreaseBrokeTileNum(void);

	//プレイヤーの順位を返す
	int GetRank(void);

	//プレイヤーの順位を決める
	void SetRank(int _rank);

	/// <summary>
	/// プレイヤーのアニメーション番号を返す
	/// </summary>
	/// <returns>参照プレイヤーのアニメーション番号</returns>
	ANIM_NUM GetAnimNum(void);

	//アニメーション番号変更
	void ChangeAnim(ANIM_NUM _animNo);

	//プレイヤーの無敵時間を返す
	float GetInvincible(void);			

	//プレイヤーか敵かを返す
	CHARA_JUDGE GetCharaJudge(void);

	/// <summary>
	/// プレイヤーか敵かを与える
	/// </summary>
	/// <param name="_type">与えるタイプ</param>
	void SetCharaJudge(CHARA_JUDGE _type);	

	//プレイヤー番号を返す
	CommonData::TYPE GetCharaNum(void);

	//攻撃のポインタを返す
	std::vector<Attack*> GetAttack(void);	

	/// <summary>
	/// 難易度を設定
	/// </summary>
	/// <param name="_diff">設定する難易度</param>
	void SetDifficulty(CommonData::DIFFICULTY _diff);

	/// <summary>
	/// プレイヤーの速度を決める
	/// </summary>
	/// <param name="_speed">速度</param>
	void SetSpeed(float _speed);

	void RunMoveInterval(void);

private:
	//ポインタ
	//-------------------
	SceneGame* sceneGame_;		//ゲームシーンのポインタ

	EffectManager* effect_;		//エフェクトのインスタンス用

	SoundManager* se_;			//SEのインスタンス用

	std::vector<Attack*> attack_;	//攻撃のインスタンス用

	//メンバ変数
	//-------------------
	int model_;							//プレイヤーの画像格納領域

	VECTOR pos_;						//プレイヤーの座標
	VECTOR scale_;						//プレイヤーの大きさ
	VECTOR rot_;						//プレイヤーの回転

	NodyUtility::DIR_3D dir_;			//プレイヤーの向き

	STATE state_;						//プレイヤーの状態

	float speed_;						//プレイヤーの移動量

	bool isWalk_;						//歩いているか(true:歩いている)

	int killScore_;						//相手を倒した数

	int life_;							//プレイヤーの体力

	CommonData::TYPE attackedPlayer_;	//攻撃してきたプレイヤー

	int deadTime_;						//プレイヤーのやられた時間

	int brokeTileNum_;					//プレイヤーの壊したタイルの数

	int rank_;							//プレイヤーの順位

	int animAttachNo_;					//アニメーションアタッチ番号

	ANIM_NUM animNum_;					//アニメーションの番号

	float animTotalTime_;				//アニメーションの総再生時間

	float stepAnim_;					//再生中のアニメーション時間

	float speedAnim_;					//アニメーションの速度

	float atkCoolDown_;					//CPU用の攻撃の再発動時間のカウント用

	float isEndAtkCoolDownCPU_;			//CPU用の攻撃の再発動時間のカウントの判定(true:判定を終えた)

	float invincible_;					//無敵時間用

	CHARA_JUDGE charaJudge_;			//プレイヤーかCPUかの判断用

	CommonData::TYPE charaNum_;			//プレイヤー番号

	int  numImage_;						//プレイヤー番号の表示用
	float numViewTime_;					//プレイヤー番号の表示時間

	int padNum_;						//パッド番号

	CommonData::DIFFICULTY difficulty_;		//CPUの難易度

	float CPUMoveInterVal_;					//CPUの移動間隔カウンタ
	bool randInterval_;						//CPUの移動間隔ずらし用
	bool isEndMoveInterval_;				//CPUの移動間隔カウンタを終えたか(true:終えた)

	bool isMoveFront_;						//前に動けるか(true:動ける) CPU用
	bool isMoveRight_;						//右に動けるか(true:動ける) CPU用
	bool isMoveBack_;						//後ろに動けるか(true:動ける) CPU用
	bool isMoveLeft_;						//左に動けるか(true:動ける) CPU用		

	//メンバー関数
	//------------------------

	/// <summary>
	/// CPUの移動判断処理
	/// </summary>
	/// <param name="_dir">最初の参照方向</param>
	/// <returns>移動する方向</returns>
	NodyUtility::DIR_3D CPUMoveChack(NodyUtility::DIR_3D _dir);

	//状態ごとの更新処理
	void UpdateState(void);

	//アニメーションの更新処理
	void UpdateAnimation(void);

	//落下アニメーション
	void FallAnimation(void);

	//アニメーション
	void Animation(void);

	//更新処理(プレイヤー)
	void UpdatePlayer(void);

	//更新処理(CPU)
	void UpdateCPU(void);
};