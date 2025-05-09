#pragma once

#include<map>
#include<string>

class SoundManager
{
public:

	enum class BGM_TYPE
	{
		TITLE,					//タイトル
		SELECT,					//セレクト
		BATTLE_FIRST_HALF,		//バトル前半
		BATTLE_SECOND_HALF,		//バトル後半
		RESULT,					//リザルト
	};

	enum class SE_TYPE
	{
		CLICK,		//決定
		CANCEL,		//キャンセル
		FALL,		//落下
		ATTACK,		//攻撃
		TILEBREAK,	//床破壊
		CURTAIN,	//舞台幕
	};

	//音量
	static constexpr int MAX_PERCENT = 100;		//最大％
	static constexpr int MAX_VOLUME = 255;		//最大音量

	//メンバー関数
	//-------------------------

	//サウンドの初期化
	void SoundInit(void);

	//BGMのパスの初期化
	void BGMInit(void);

	/// <summary>
	/// BGMのロード
	/// </summary>
	/// <param name="_bgm">BGMの種類</param>
	void BGMLoad(const BGM_TYPE _bgm);

	//SEのパスの初期化
	void SEInit(void);

	/// <summary>
	/// SEのロード	
	/// </summary>
	/// <param name="_bgm">SEの種類</param>
	void SELoad(const SE_TYPE _se);

	/// <summary>
	/// BGMの再生
	/// </summary>
	/// <param name="_bgm">BGMの種類</param>
	/// <param name="_playType">再生形式</param>
	/// <param name="_volumePar">音量(%表記)</param>
	/// <param name="_topPositionFlag">最初から再生を始めるか (true:最初から)</param>
	void PlayBGM(const BGM_TYPE _bgm, const int _playType, const int _volumePar = MAX_PERCENT, const bool _topPositionFlag = true);

	/// <summary>
	/// SEの再生
	/// </summary>
	/// <param name="_se">SEの種類</param>
	/// <param name="_playType">再生形式</param>
	/// <param name="_volumePar">音量(%表記)</param>
	/// <param name="_topPositionFlag">最初から再生を始めるか (true:最初から)</param>
	void PlaySE(const SE_TYPE _se, const int _playType, const int _volumePar = MAX_PERCENT, const bool _topPositionFlag = true);

	/// <summary>
	/// BGMが再生中かどうかを返す
	/// </summary>
	/// <param name="_bgm">BGMの種類</param>
	/// <returns>true:再生中</returns>
	bool CheckBGMPlay(const BGM_TYPE _bgm);

	/// <summary>
	/// SEが再生中かどうかを返す
	/// </summary>
	/// <param name="_se">SEの種類</param>
	/// <returns>true:再生中</returns>
	bool CheckSEPlay(const SE_TYPE _se);

	/// <summary>
	/// BGMを止める
	/// </summary>
	/// <param name="_bgm">止めるBGM</param>
	void StopBGM(const BGM_TYPE _bgm);

	/// <summary>
	/// SEを止める
	/// </summary>
	/// <param name="_se">止めるSE</param>
	void StopSE(const SE_TYPE _se);

private:

	//メンバー変数
	//--------------------------

	//サウンドのパス
	std::map<BGM_TYPE, std::string> bgmPass_;	//BGMのパス

	std::map<SE_TYPE, std::string> sePass_;		//SEのパス

	//サウンドのハンドル
	std::map<BGM_TYPE,int> bgm_;	//BGMハンドル

	std::map<SE_TYPE, int> se_;		//SEハンドル
};

