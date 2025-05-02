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
		,ATTACK
	};

	//エフェクトの固有のデータ
	struct EFFDATA
	{
		//エフェクトのリソースハンドルID
		int effectResId_;

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
	void EffectPlay(EFF_TYPE _effect, VECTOR _pos, VECTOR _scale,VECTOR _rot);

	//エフェクトの停止
	void EffectStop(EFF_TYPE _effect);

	//エフェクトの座標変化
	void EffectChangePos(EFF_TYPE _effect, VECTOR _pos);

	//エフェクトの大きさ変化
	void EffectChangeScl(EFF_TYPE _effect, VECTOR _scl);
	
	//エフェクトの回転変化
	void EffectChangeRot(EFF_TYPE _effect, VECTOR _rot);

private:

	//エフェクトデータの格納領域
	std::map<EFF_TYPE, EFFDATA> effDatas_;

	//エフェクトのロード
	void EffectLoad(void);
};

