//#pragma once

#include"../Common/CommonData.h"
#include"../Utility/NodyUtility.h"

class SceneGame;
class Player;

class Attack
{
public:
	//定数
	//-------------------
	static constexpr float ATK_SPEED = 10.0;		//攻撃の速度

	//メンバー関数
	//-------------------
	
	//デフォルトコンストラクタ
	Attack(void);			

	//デストラクタ
	~Attack(void);			
	
	/// <summary>
	/// 初期化処理 
	/// </summary>
	/// <param name="parent">シーンゲーム</param>
	/// <param name="parPlayer">親プレイヤー</param>
	/// <param name="_pos">初期位置</param>
	/// <param name="_dir">初期方向</param>
	/// <param name="_CharaNum">親プレイヤーのプレイヤー番号</param>
	/// <returns></returns>
	bool Init(SceneGame* parent
			, Player* parPlayer
			, VECTOR _pos
			, NodyUtility::DIR_3D _dir
			, CommonData::TYPE _charaNum);

	//更新処理
	void Update(void);							

	//描画処理
	void Draw(void);							

	//解放処理
	bool Release(void);							

	//攻撃の現在座標を返す
	VECTOR GetPos(void);				
	//攻撃の座標を決める
	void SetPos(VECTOR _setPos);		

	//攻撃の生存判定を返す
	bool GetIsAlive(void);				
	//攻撃の生存判定を設定
	void SetIsAlive(bool _isAttack);

	//攻撃の方向を返す
	NodyUtility::DIR_3D GetAttackDir(void);				
	//攻撃の方向設定
	void SetAttackDir(NodyUtility::DIR_3D _setDir);		

	//攻撃のプレイヤー番号を返す
	CommonData::TYPE GetCharaNum(void);

	//攻撃最中の経過時間を返す
	float GetAtkCnt(void);			

	//攻撃の削除
	void Destroy(void);

private:
	//ポインタ
	//-------------------
	SceneGame* sceneGame_;		//ゲームシーンのポインタ

	Player* player_;			//プレイヤーのポインタ

	//メンバー変数
	//-------------------
	int images_;				//攻撃の画像格納領域
	
	VECTOR pos_;				//攻撃の座標

	NodyUtility::DIR_3D dir_;	//攻撃の向き

	CommonData::TYPE charaNum_;	//親プレイヤー番号

	bool isAlive_;			//攻撃の生存判定

	float speed_;			//攻撃の速度

	float atkCnt_;			//攻撃最中の経過時間カウント用

	//メンバー関数
	//-------------------
};