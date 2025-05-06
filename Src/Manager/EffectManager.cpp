#include<EffekseerForDXLib.h>
#include"../Application.h"
#include "EffectManager.h"

void EffectManager::EffectInit(void)
{
	//�G�t�F�N�V�A�̏�����
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
	//�G�t�F�N�V�A�̏I��
	Effkseer_End();
}

void EffectManager::EffectLoad(void)
{
	EFFDATA data;

	//�G�t�F�N�g�̃��[�h
	data.effectResId_ = LoadEffekseerEffect((Application::PATH_EFFECT + "Basic/Start.efkefc").c_str());
	effDatas_.emplace(EFF_TYPE::START, data);

	data.effectResId_ = LoadEffekseerEffect((Application::PATH_EFFECT + "Basic/Revival.efkefc").c_str());
	effDatas_.emplace(EFF_TYPE::REVIVAL,data);

	data.effectResId_ = LoadEffekseerEffect((Application::PATH_EFFECT + "Pierre/Attack2.efkefc").c_str());
	effDatas_.emplace(EFF_TYPE::ATTACK,data);
}

void EffectManager::EffectPlay(const EFF_TYPE _effect, const VECTOR _pos, const VECTOR _scale, const VECTOR _rot)
{
	//�G�t�F�N�g�n���h��
	int effHandle = PlayEffekseer3DEffect(effDatas_[_effect].effectResId_);

	//�G�t�F�N�g�̍Đ�
	effDatas_[_effect].effectPlayId_.emplace_back(effHandle);

	//�ȍ~�A�Đ�����G�t�F�N�g�̐���͕K���v���C�n���h��ID���g�p���邱��
	//�G�t�F�N�g�̑傫���ݒ�(XYZ)
	SetScalePlayingEffekseer3DEffect(effHandle, _scale.x, _scale.y, _scale.z);

	//�G�t�F�N�g�̊p�x��ݒ�
	SetRotationPlayingEffekseer3DEffect(effHandle, _rot.x, _rot.y, _rot.z);

	//�G�t�F�N�g�̈ʒu��ݒ�
	SetPosPlayingEffekseer3DEffect(effHandle, _pos.x, _pos.y, _pos.z);
}

void EffectManager::EffectStop(const EFF_TYPE _effect)
{
	for (auto playId : effDatas_[_effect].effectPlayId_)
	{
		StopEffekseer3DEffect(playId);
	}
}

void EffectManager::EffectChangePos(const EFF_TYPE _effect, const VECTOR _pos)
{
	for (auto playId : effDatas_[_effect].effectPlayId_)
	{
		if (IsEffekseer3DEffectPlaying(playId))
		{
			SetPosPlayingEffekseer3DEffect(playId, _pos.x, _pos.y, _pos.z);
		}
	}
}

void EffectManager::EffectChangeScl(const EFF_TYPE _effect, const VECTOR _scl)
{
	for (auto playId : effDatas_[_effect].effectPlayId_)
	{
		if (IsEffekseer3DEffectPlaying(playId))
		{
			SetScalePlayingEffekseer3DEffect(playId, _scl.x, _scl.y, _scl.z);
		}
	}
}

void EffectManager::EffectChangeRot(const EFF_TYPE _effect, const VECTOR _rot)
{
	for (auto playId : effDatas_[_effect].effectPlayId_)
	{
		if (IsEffekseer3DEffectPlaying(playId))
		{
			SetRotationPlayingEffekseer3DEffect(playId, _rot.x, _rot.y, _rot.z);
		}
	}
}
