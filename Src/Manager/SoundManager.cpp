#include<DxLib.h>
#include"../Application.h"
#include "Resource.h"
#include "ResourceManager.h"
#include "SoundManager.h"

void SoundManager::SoundInit(void)
{
	BGMInit();
	SEInit();
}

void SoundManager::BGMInit(void)
{
	//リソースマネージャーのインスタンス取得
	ResourceManager& res = ResourceManager::GetInstance();

	//BGMのリソース
	res.ResourceBGM();

	//BGMのパス

	//タイトルBGM
	bgmPass_[BGM_TYPE::TITLE] = res.Load(ResourceManager::SRC::TITLE_BGM).path_;
	//セレクトBGM
	bgmPass_[BGM_TYPE::SELECT] = res.Load(ResourceManager::SRC::SELECT_BGM).path_;
	//バトル前半BGM
	bgmPass_[BGM_TYPE::BATTLE_FIRST_HALF] = res.Load(ResourceManager::SRC::BATTLE_FIRST_HALF_BGM).path_;
	//バトル後半BGM
	bgmPass_[BGM_TYPE::BATTLE_SECOND_HALF] = res.Load(ResourceManager::SRC::BATTLE_SECOND_HALF_BGM).path_;
	//リザルトBGM
	bgmPass_[BGM_TYPE::RESULT] = res.Load(ResourceManager::SRC::RESULT_BGM).path_;
}

void SoundManager::BGMLoad(const BGM_TYPE _bgm)
{
	//指定したBGMをロード
	bgm_[_bgm] = LoadSoundMem(bgmPass_[_bgm].c_str());
}

void SoundManager::SEInit(void)
{
	//リソースマネージャーのインスタンス取得
	ResourceManager& res = ResourceManager::GetInstance();

	//SEのリソース
	res.ResourceSE();

	//SEのパス
	sePass_[SE_TYPE::CLICK] = res.Load(ResourceManager::SRC::CLICK_SE).path_;
	sePass_[SE_TYPE::CANCEL] = res.Load(ResourceManager::SRC::CANCEL_SE).path_;
	sePass_[SE_TYPE::FALL ] = res.Load(ResourceManager::SRC::FALL_SE).path_;
	sePass_[SE_TYPE::ATTACK ] = res.Load(ResourceManager::SRC::ATTACK_SE).path_;
	sePass_[SE_TYPE::TILEBREAK] = res.Load(ResourceManager::SRC::TILEBREAK_SE).path_;
	sePass_[SE_TYPE::CURTAIN] = res.Load(ResourceManager::SRC::CURTAIN_SE).path_;
}

void SoundManager::SELoad(const SE_TYPE _se)
{
	//指定したSEをロード
	se_[_se] = LoadSoundMem(sePass_[_se].c_str());
}

void SoundManager::PlayBGM(const BGM_TYPE _bgm, const int _playType, const int _volumePar, const bool _topPositionFlag)
{
	//音量設定
	ChangeVolumeSoundMem((MAX_VOLUME * _volumePar / MAX_PERCENT), bgm_[_bgm]);
	
	//BGM再生
	PlaySoundMem(bgm_[_bgm], _playType, _topPositionFlag);
}

void SoundManager::PlaySE(const SE_TYPE _se, const int _playType, const int _volumePar, const bool _topPositionFlag)
{
	//音量設定
	ChangeVolumeSoundMem((MAX_VOLUME * _volumePar / MAX_PERCENT), se_[_se]);
	
	//SE再生
	PlaySoundMem(se_[_se], _playType, _topPositionFlag);
}

bool SoundManager::CheckBGMPlay(const BGM_TYPE _bgm)
{
	return CheckSoundMem(bgm_[_bgm]);
}

bool SoundManager::CheckSEPlay(const SE_TYPE _se)
{
	return CheckSoundMem(se_[_se]);
}

void SoundManager::StopBGM(const BGM_TYPE _bgm)
{
	StopSoundMem(bgm_[_bgm]);
}

void SoundManager::StopSE(const SE_TYPE _se)
{
	StopSoundMem(se_[_se]);
}

void SoundManager::SoundRelease(void)
{
	//サウンド解放
	for (auto& bgm : bgm_)
	{
		if (bgm.second == -1)continue;
		DeleteSoundMem(bgm.second);
	}

	for (auto& se : se_)
	{
		if (se.second == -1)continue;
		DeleteSoundMem(se.second);
	}
}
