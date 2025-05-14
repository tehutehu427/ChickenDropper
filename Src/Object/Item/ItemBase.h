#pragma once

#include<DxLib.h>
#include"../../Utility/Utility.h"

class SceneGame;
class Player;

class ItemBase
{
public:
	//アイテムの種類	
	enum class ITEM_TYPE
	{
		HAMMER	//ハンマー
		,BOOTS	//ブーツ
		,MAX
	};

	//定数
	//----------------------------

	static constexpr float RADIUS = 24.0f;					//半径
	static constexpr int DIV_NUM = 20;						//球の分割
	static constexpr float PLAYER_FOLLOW = RADIUS * 1.5f;	//プレイヤーを囲う用
	static constexpr int PLAYER_GOT_DIV_NUM = 2;			//プレイヤー取得時の球の分割

	static constexpr int MAX_NUM = 3;			//アイテムの出現最大数

	static constexpr int APPEAR_INTERVAL_SEC = 20;											//出現間隔(秒)
	static constexpr float APPEAR_INTERVAL = APPEAR_INTERVAL_SEC * Utility::DEFAULT_FPS;	//出現間隔

	static constexpr float EFFECT_TIME = 5.0f * Utility::DEFAULT_FPS;				//効果時間
	
	//メンバー関数
	//----------------------------
	
	//コンストラクタ
	ItemBase(SceneGame* parent);

	//デストラクタ
	virtual ~ItemBase();

	virtual void Init(VECTOR _pos);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	//パラメータ設定
	virtual void SetParam(void) = 0;	

	//効果ごとの更新処理
	virtual void EffectUpdate(void) = 0;

	//効果ごとの終了処理
	virtual void EffectEnd(void);

	//アイテムの位置を返す
	const VECTOR GetPos(void);

	//アイテムの生存判定
	const bool GetIsAlive(void);

	//アイテムの効果の有効判定
	const bool GetIsEffective(void);

	//アイテムの効果の有効判定を決める
	void SetIsEffective(const bool _isEffective);

	//アイテムの種類を返す
	const ITEM_TYPE GetItemType(void);

	//追従プレイヤーを返す
	const Player* GetFollow(void);

	//追従するプレイヤーを決める
	void SetFollow(Player* _followPlayer);

	//アイテムの実体の削除
	void KillItem(void);

	//アイテムの消去
	void DeleteItem(void);

protected:

	//インスタンス
	SceneGame* sceneGame_;	//シーンゲーム
	Player* follow_;		//追従対象

	//モデル関係
	int model_;		//モデル
	VECTOR pos_;	//座標
	VECTOR rot_;	//回転
	VECTOR scale_;	//大きさ

	ITEM_TYPE itemType_;	//アイテムの種類

	bool isAlive_;			//生存判定

	bool isEffective_;		//効果の有効判定

	float effectTime_;		//効果時間カウンタ

	unsigned int color_;	//色
};

