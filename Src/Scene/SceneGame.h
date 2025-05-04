#pragma once

#include<vector>
#include"../Common/CommonData.h"
#include"../Utility/Utility.h"
#include"SceneBase.h"

//クラスの前方宣言
class EffectManager;
class SoundManager;
class Stage;
class RuleBase;
class Player;
class ItemBase;

class SceneGame : public SceneBase
{
public:
	//列挙型
	//--------------------

	//ゲームの状態
	enum class GAME_STATE
	{
		PRE_START	//ゲーム開始準備
		,START		//ゲーム開始
		,GAME		//ゲーム中
		,END		//ゲーム終了
	};

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

	//１ラインのタイル数を返す
	int GetTileNum(void);		
	//１ライン半のタイル数を返す
	int GetTileHNum(void);		

	//タイルのXサイズを返す
	float GetTileSizeX(void);	
	//タイルのZサイズを返す
	float GetTileSizeZ(void);	

	//ステージのXサイズの半分を返す
	float GetStageHSizeX(void);	
	//ステージのZサイズの半分を返す
	float GetStageHSizeZ(void);	

	//ゲームの状態を返す
	GAME_STATE GetGameState(void);

	/// <summary>
	/// ゲームの状態を変更する
	/// </summary>
	/// <param name="_state">変更するゲームの状態</param>
	void ChangeGameState(GAME_STATE _state);

	/// <summary>
	/// 攻撃時間を返す
	/// </summary>
	/// <param name="ix">Xのタイル番号</param>
	/// <param name="iz">Zのタイル番号</param>
	/// <returns>返す攻撃時間 ※StageのatkCnt_を返す</returns>
	float GetAtkCnt(int ix, int iz);				

	/// <summary>
	/// 攻撃時間を与える
	/// </summary>
	/// <param name="ix">Xのタイル番号</param>
	/// <param name="iz">Zのタイル番号</param>
	/// <param name="_cnt">与える時間</param>
	void SetAtkCnt(int ix, int iz, float _cnt);		

	/// <summary>
	/// ステージ端との衝突判定
	/// </summary>
	/// <param name="_pos">調べたいプレイヤーの座標</param>
	/// <param name="_dir">調べたいプレイヤーの方向</param>
	/// <returns>衝突したらtrue</returns>
	bool IsCollisionStageEnd(VECTOR _pos, Utility::DIR_3D _dir);

	/// <summary>
	/// 隣のタイルが壊れているかを確かめる
	/// </summary>
	/// <param name="_pos">参照するプレイヤーのpos</param>
	/// <param name="_dir">参照するプレイヤーのdir</param>
	/// <returns>壊れていたらtrue</returns>
	bool IsNextTileBroken(VECTOR _pos, Utility::DIR_3D _dir);

	/// <summary>
	/// 隣のタイルが攻撃状態かを確かめる
	/// </summary>
	/// <param name="_pos">参照するプレイヤーのpos</param>
	/// <param name="_dir">参照するプレイヤーのdir</param>
	/// <returns>攻撃状態ならtrue</returns>
	bool IsNextTileAttack(VECTOR _pos, Utility::DIR_3D _dir);

	/// <summary>
	/// 今立っているのタイルが攻撃状態かを確かめる
	/// </summary>
	/// <param name="_pos">参照するプレイヤーのpos</param>
	/// <returns>壊れていたらtrue</returns>
	bool IsThisTileAttack(VECTOR _pos);

	/// <summary>
	/// 隣のタイルが攻撃状態かを確かめる
	/// </summary>
	/// <param name="_pos">参照するプレイヤーのpos</param>
	/// <param name="_dir">参照するプレイヤーのdir</param>
	/// <returns>攻撃状態ならtrue</returns>
	bool IsNextTilePreEnd(VECTOR _pos, Utility::DIR_3D _dir);

	/// <summary>
	/// 今立っているのタイルが攻撃状態かを確かめる
	/// </summary>
	/// <param name="_pos">参照するプレイヤーのpos</param>
	/// <returns>壊れていたらtrue</returns>
	bool IsThisTilePreEnd(VECTOR _pos);

	/// <summary>
	/// 隣のタイルが攻撃状態かを確かめる
	/// </summary>
	/// <param name="_pos">参照するプレイヤーのpos</param>
	/// <param name="_dir">参照するプレイヤーのdir</param>
	/// <returns>攻撃状態ならtrue</returns>
	bool IsNextTileEnd(VECTOR _pos, Utility::DIR_3D _dir);
	
	//プレイヤーのポインタを返す
	std::vector<Player*> GetPlayer(void);

	/// <summary>
	/// プレイヤーの得点を返す
	/// </summary>
	/// <param name="_player">参加プレイヤー</param>
	/// <returns>参照プレイヤーの得点</returns>
	int GetPlayerScore(Player* _player);

	/// <summary>
	/// プレイヤーの加点処理
	/// </summary>
	/// <param name="_charaNum">加点するプレイヤー</param>
	void TakeScore(CommonData::TYPE _charaNum);

	/// <summary>
	/// プレイヤーの体力を返す
	/// </summary>
	/// <param name="_player">参照プレイヤー</param>
	/// <returns>参照プレイヤーの体力</returns>
	int GetLife(Player* _player);

	/// <summary>
	/// プレイヤーの壊したタイルの数を返す
	/// </summary>
	/// <param name="_player">参照プレイヤー</param>
	/// <returns>参照プレイヤーの壊したタイルの数</returns>
	int GetBrokeTileNum(Player* _player);

	/// <summary>
	/// プレイヤーの壊したタイルの数を増加させる
	/// </summary>
	/// <param name="_charaNum">数を増加させるプレイヤーの番号</param>
	void IncreaseBrokeTileNum(CommonData::TYPE _charaNum);

	/// <summary>
	/// プレイヤー番号を返す
	/// </summary>
	/// <param name="_player">参照プレイヤー</param>
	/// <returns>参照プレイヤーのプレイヤー番号</returns>
	CommonData::TYPE GetCharaNum(Player* _player);

	/// <summary>
	///プレイヤーの復活処理
	/// </summary>
	/// <param name="_player">復活させるプレイヤー</param>
	void ReviavalPlayer(Player* _player);

	/// <summary>
	/// プレイヤー同士の当たり判定
	/// </summary>
	/// <param name="_pos">参照プレイヤーのpos</param>
	/// <param name="_dir">参照プレイヤーのdir</param>
	/// <param name="_num">参照プレイヤーのcharaNum</param>
	/// <returns>衝突したらtrue</returns>
	bool HitP2P(VECTOR _pos, Utility::DIR_3D _dir, CommonData::TYPE _num);

	/// <summary>
	/// 狙うプレイヤーを決める(CPU用)
	/// </summary>
	/// <param name="_pos">参照プレイヤーのpos</param>
	/// <param name="_num">参照プレイヤーのcharaNum</param>
	/// <returns>狙うプレイヤーへの方向</returns>
	Utility::DIR_3D AimPlayer(VECTOR _pos ,CommonData::TYPE _num);
	
	/// <summary>
	/// 狙ったプレイヤーが攻撃できるかを求める(CPU用)
	/// </summary>
	/// <param name="_pos">参照プレイヤーのpos</param>
	/// <param name="_num">参照プレイヤーのcharaNum</param>
	/// <param name="_dir">参照プレイヤーのdir</param>
	/// <returns>攻撃できるならtrue</returns>
	bool AimPlayerAttack(VECTOR _pos, CommonData::TYPE _num, Utility::DIR_3D _dir);

	/// <summary>
	/// 特定プレイヤーのタイルをすぐに壊す処理
	/// </summary>
	/// <param name="_charaNum">壊したいプレイヤータイル</param>
	void TileNowBreak(CommonData::TYPE _charaNum);

	//ステージ縮小のフラグ
	bool IsNarrowStage(void);

	/// <summary>
	/// 座標からタイルのX配列番号を返す
	/// </summary>
	/// <param name="_posX">参照X座標</param>
	/// <returns>タイルのX配列番号</returns>
	int Pos2TileAddX(float _posX);

	/// <summary>
	/// 座標からタイルのZ配列番号を返す
	/// </summary>
	/// <param name="_posZ">参照Z座標</param>
	/// <returns>タイルのZ配列番号</returns>
	int Pos2TileAddZ(float _posZ);

	/// <summary>
	/// タイルのX配列番号から座標を返す
	/// </summary>
	/// <param name="_tileAddX">参照するタイルのX配列番号</param>
	/// <returns>タイルのあるX座標</returns>
	float TileAdd2PosX(int _tileAddX);

	/// <summary>
	/// タイルのZ配列番号から座標を返す
	/// </summary>
	/// <param name="_tileAddZ">参照するタイルのZ配列番号</param>
	/// <returns>タイルのあるZ座標</returns>
	float TileAdd2PosZ(int _tileAddZ);

	/// <summary>
	/// ゲームの終了処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>ゲーム終了</returns>
	void EndGame(void);

private:
	//メンバー変数
	//--------------------------
	//インスタンス用

	//エフェクトのインスタンス用
	EffectManager* effect_;

	//サウンドのインスタンス用
	SoundManager* sound_;

	//ステージのインスタンス用
	Stage* stage_;					

	//ルールのインスタンス用
	RuleBase* rule_;					

	//プレイヤーのインスタンス用
	std::vector<Player*> player_;	

	//アイテムのインスタンス用
	std::vector<ItemBase*>item_;	

	//ゲームの状態
	GAME_STATE gameState_;

	//アイテムの出現間隔
	float itemInterval_;

	//メンバー関数
	//--------------------------

	//ルール設定
	void SetRule(void);
	
	//プレイヤーの処理
	void ProcessPlayer(void);			

	//アイテムの出現処理
	void AppearItem(void);

	/// <summary>
	/// 有効なアイテム配列番号を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>使用可能なアイテムのポインタ</returns>
	ItemBase* GetValidItem(void);

	/// <summary>
	/// アイテムを生成する
	/// </summary>
	/// <param name="_item">生成するアイテムのポインタ</param>
	void GenerateItem(ItemBase* _item);

	/// <summary>
	/// プレイヤーのアイテム取得処理
	/// </summary>
	/// <param name="_player">取得したプレイヤー</param>
	/// <param name="_item">取得したアイテム</param>
	void PlayerGotItem(Player* _player, ItemBase* _item);
};