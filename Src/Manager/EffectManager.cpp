#include<EffekseerForDXLib.h>
#include"../Application.h"
#include "EffectManager.h"

void EffectManager::EffectInit(void)
{
	//エフェクシアの初期化
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	EffectLoad();
}

void EffectManager::EffectEnd(void)
{
	//エフェクシアの終了
	Effkseer_End();
}

void EffectManager::EffectLoad(void)
{
	EFFDATA data;

	//エフェクトのロード

	//開始エフェクト
	data.effectResId_ = LoadEffekseerEffect((Application::PATH_EFFECT + "Basic/Start.efkefc").c_str());
	effDatas_.emplace(EFF_TYPE::START, data);

	//復活エフェクト
	data.effectResId_ = LoadEffekseerEffect((Application::PATH_EFFECT + "Basic/Revival.efkefc").c_str());
	effDatas_.emplace(EFF_TYPE::REVIVAL,data);
}

void EffectManager::EffectPlay(const EFF_TYPE _effect, const VECTOR _pos, const VECTOR _scale, const VECTOR _rot)
{
	//エフェクトハンドル
	int effHandle = PlayEffekseer3DEffect(effDatas_[_effect].effectResId_);

	//エフェクトの再生
	effDatas_[_effect].effectPlayId_.emplace_back(effHandle);

	//以降、再生するエフェクトの制御は必ずプレイハンドルIDを使用すること
	//エフェクトの大きさ設定(XYZ)
	SetScalePlayingEffekseer3DEffect(effHandle, _scale.x, _scale.y, _scale.z);

	//エフェクトの角度を設定
	SetRotationPlayingEffekseer3DEffect(effHandle, _rot.x, _rot.y, _rot.z);

	//エフェクトの位置を設定
	SetPosPlayingEffekseer3DEffect(effHandle, _pos.x, _pos.y, _pos.z);
}

void EffectManager::EffectStop(const EFF_TYPE _effect)
{
	//指定タイプのエフェクトを止める
	for (auto playId : effDatas_[_effect].effectPlayId_)
	{
		StopEffekseer3DEffect(playId);
	}
}

void EffectManager::EffectChangePos(const EFF_TYPE _effect, const VECTOR _pos)
{
	//指定タイプの座標変更
	for (auto playId : effDatas_[_effect].effectPlayId_)
	{
		//プレイ中のエフェクトか調べる
		if (IsEffekseer3DEffectPlaying(playId))
		{
			//指定座標に変更
			SetPosPlayingEffekseer3DEffect(playId, _pos.x, _pos.y, _pos.z);
		}
	}
}

void EffectManager::EffectChangeScl(const EFF_TYPE _effect, const VECTOR _scl)
{
	//指定タイプの大きさ変更
	for (auto playId : effDatas_[_effect].effectPlayId_)
	{
		//プレイ中のエフェクトか調べる
		if (IsEffekseer3DEffectPlaying(playId))
		{
			//指定した大きさに変更
			SetScalePlayingEffekseer3DEffect(playId, _scl.x, _scl.y, _scl.z);
		}
	}
}

void EffectManager::EffectChangeRot(const EFF_TYPE _effect, const VECTOR _rot)
{
	//指定タイプの回転変更
	for (auto playId : effDatas_[_effect].effectPlayId_)
	{
		//プレイ中のエフェクトか調べる
		if (IsEffekseer3DEffectPlaying(playId))
		{
			//指定した回転にする
			SetRotationPlayingEffekseer3DEffect(playId, _rot.x, _rot.y, _rot.z);
		}
	}
}
