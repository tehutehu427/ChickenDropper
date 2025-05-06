#pragma once

#include<vector>

class CommonData
{
public:
	//列挙型
	//--------------------
	//プレイヤーの種別
	enum class TYPE
	{
		NONE
		, P1		//プレイヤー１
		, P2		//プレイヤー２
		, P3		//プレイヤー３
		, P4		//プレイヤー４(最大数)
	};

	//CPU数
	enum class CPU_NUM
	{
		NONE	//なし
		,C1		//CPU 1人
		,C2		//CPU 2人
		,C3		//CPU 3人
	};

	//対戦形式
	enum class BATTLE_PATTERN
	{
		P1C1		//プレイヤー1人 CPU1人
		,P1C2		//プレイヤー1人 CPU2人
		,P1C3		//プレイヤー1人 CPU3人
		,P2			//プレイヤー2人
		,P2C1		//プレイヤー2人 CPU1人
		,P2C2		//プレイヤー2人 CPU2人
		,P3			//プレイヤー3人
		,P3C1		//プレイヤー3人 CPU1人
		,P4			//プレイヤー4人
		,MAX
	};

	//ルール
	enum class RULE
	{
		NONE
		, TIME			//時間制
		, LIFE			//ライフ制
		, BREAK_TILE	//床の破壊数
	};

	//難易度
	enum class DIFFICULTY
	{
		EASY		//簡単
		,NORMAL		//普通
		,HARD		//難しい
	};

	//メンバー関数
	//--------------------------
	
	//初期化処理
	bool Init(void);	

	//インスタンスの破棄
	void Destroy(void);

	//プレイヤーの数を返す
	const TYPE GetPlayerNum(void)const;

	/// <summary>
	/// プレイヤーの数を決める
	/// </summary>
	/// <param name="_pNum">プレイヤーの数</param>
	void SetPlayerNum(const TYPE _pNum);

	//CPUの数を返す
	const CPU_NUM GetCPUNum(void)const;

	/// <summary>
	/// CPUの数を決める
	/// </summary>
	/// <param name="_cNum">CPUの数</param>
	void SetCPUNum(const CPU_NUM _cNum);

	//対戦人数を返す
	const BATTLE_PATTERN GetBattlePattern(void)const;

	/// <summary>
	/// 対戦人数を決める
	/// </summary>
	/// <param name="_pattern">対戦人数</param>
	void SetBattlePattern(const BATTLE_PATTERN _pattern);

	//ルールの情報を返す
	const RULE GetRule(void)const;

	/// <summary>
	/// ルールを決める
	/// </summary>
	/// <param name="_ruleType">ルール</param>
	void SetRule(const RULE _ruleType);

	//順位を決めれるプレイヤーの数を返す
	const std::vector<int> GetRank(void)const;

	//順位を返す
	const int GetRank(const int size)const;

	/// <summary>
	/// 順位を決める(P1から順番に)
	/// </summary>
	/// <param name="_rank">順位</param>
	void SetRank(const int _rank);

	//配列を削除
	void DeleteArray(void);

	/// <summary>
	/// 難易度を返す
	/// </summary>
	/// <param name="size">ほしいCPUの配列番号</param>
	/// <returns>難易度</returns>
	const DIFFICULTY GetDifficulty(const int size)const;

	//難易度配列のサイズを返す
	const size_t GetDifficultySize(void)const;

	/// <summary>
	/// 難易度を配列の最後から抜き出す
	/// </summary>
	/// <param name=""></param>
	/// <returns>難易度</returns>
	const DIFFICULTY PopDifficulty(void);

	//難易度を決める(CPUの番号が早い順に)
	void SetDifficulty(const DIFFICULTY _difficulty);

	//難易度の配列を一つ削除
	void DeleteDiff(void);

	//難易度の配列を全て削除
	void DeleteDiffAll(void);

	//シングルトン化
	//--------------------------
	
	//外部から静的にインスタンスの生成をする
	static void CreateInstance(void);

	//外部でインスタンスを利用可能とするために、
	//インスタンスを返す関数を作る
	static CommonData& GetInstance(void);

private:
	//メンバー変数
	//--------------------------
	TYPE playerNum_;				//プレイヤーの人数

	CPU_NUM cpuNum_;				//CPUの数

	BATTLE_PATTERN battlePattern_;	//対戦人数

	RULE rule_;						//ルール		

	std::vector<int> rank_;			//順位

	std::vector<DIFFICULTY> difficulty_;			//難易度

	//シングルトン化
	//---------------------------
	
	//コンストラクタ
	CommonData(void);		
	//デストラクタ
	~CommonData(void);		

	//コピーコンストラクタを利用できないようにする
	CommonData(const CommonData& ins);

	//静的なインスタンス格納領域
	static CommonData* instance_;

};