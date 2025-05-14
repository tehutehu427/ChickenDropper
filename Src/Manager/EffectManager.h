#pragma once

#include<vector>
#include<map>

class EffectManager
{
public:

	//エフェクトの種類
	enum class EFF_TYPE
	{
		START
		,REVIVAL
	};

	//エフェクトの固有のデータ
	struct EFFDATA
	{
		//エフェクトのリソースハンドルID
		int effectResId_ = -1;

		//エフェクトの再生ハンドルID
		std::vector<int> effectPlayId_;
	};


	//エフェクシアの初期化
	void EffectInit(void);

	//エフェクシアの終了
	void EffectEnd(void);

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="_effect">再生するエフェクトの種類</param>
	/// <param name="_pos">再生する座標</param>
	/// <param name="_scale">再生する時の大きさ</param>
	/// <param name="_rot">再生する時の回転</param>
	void EffectPlay(const EFF_TYPE _effect, const VECTOR _pos, const VECTOR _scale, const VECTOR _rot);

	/// <summary>
	/// エフェクトの停止
	/// </summary>
	/// <param name="_effect">止めたいエフェクト</param>
	void EffectStop(const EFF_TYPE _effect);

	/// <summary>
	/// エフェクトの座標変化
	/// </summary>
	/// <param name="_effect">エフェクト</param>
	/// <param name="_pos">変化後の座標</param>
	void EffectChangePos(const EFF_TYPE _effect, const VECTOR _pos);

	/// <summary>
	/// エフェクトの大きさ変化
	/// </summary>
	/// <param name="_effect">エフェクト</param>
	/// <param name="_scl">変化後の大きさ</param>
	void EffectChangeScl(const EFF_TYPE _effect, const VECTOR _scl);
	
	/// <summary>
	/// エフェクトの回転変化
	/// </summary>
	/// <param name="_effect">エフェクト</param>
	/// <param name="_rot">変化後の回転</param>
	void EffectChangeRot(const EFF_TYPE _effect, const VECTOR _rot);

private:

	//エフェクトデータの格納領域
	std::map<EFF_TYPE, EFFDATA> effDatas_;

	//エフェクトのロード
	void EffectLoad(void);
};

