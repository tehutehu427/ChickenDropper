#include<DxLib.h>
#include"../Application.h"
#include "SoundManager.h"

void SoundManager::SoundInit(void)
{
	BGMInit();
	SEInit();
}

void SoundManager::BGMInit(void)
{
	//BGMのパス
	bgmPass_[BGM_TYPE::TITLE] = "title.mp3";
	bgmPass_[BGM_TYPE::SELECT] = "Select.wav";
	bgmPass_[BGM_TYPE::BATTLE_FIRST_HALF] = "BattleFirstHalf.mp3";
	bgmPass_[BGM_TYPE::BATTLE_SECOND_HALF] = "BattleSecondHalf.mp3";
	bgmPass_[BGM_TYPE::RESULT] = "Result2.mp3";
}

void SoundManager::BGMLoad(const BGM_TYPE _bgm)
{
	//指定したBGMをロード
	bgm_[_bgm] = LoadSoundMem((Application::PATH_BGM + bgmPass_[_bgm]).c_str());
}

void SoundManager::SEInit(void)
{
	//SEのパス
	sePass_[SE_TYPE::CLICK] = "Click.mp3";
	sePass_[SE_TYPE::CANCEL] = "Cancel.mp3";
	sePass_[SE_TYPE::FALL ] = "Fall.mp3";
	sePass_[SE_TYPE::ATTACK ] = "Attack.mp3";
	sePass_[SE_TYPE::TILEBREAK] = "TileBreak.mp3";
	sePass_[SE_TYPE::CURTAIN] = "CurtainOpen.mp3";
}

void SoundManager::SELoad(const SE_TYPE _se)
{
	////指定したSEをロード
	se_[_se] = LoadSoundMem((Application::PATH_SE + sePass_[_se]).c_str());
}

void SoundManager::PlayBGM(const BGM_TYPE _bgm, const int _playType, const int _volumePar, const bool _topPositionFlag)
{
	ChangeVolumeSoundMem((MAX_VOLUME * _volumePar / MAX_PERCENT), bgm_[_bgm]);
	PlaySoundMem(bgm_[_bgm], _playType, _topPositionFlag);
}

void SoundManager::PlaySE(const SE_TYPE _se, const int _playType, const int _volumePar, const bool _topPositionFlag)
{
	ChangeVolumeSoundMem((MAX_VOLUME * _volumePar / MAX_PERCENT), se_[_se]);
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
	for (int b = 0; b < static_cast<int>(BGM_TYPE::MAX); b++)
	{
		DeleteSoundMem(bgm_[static_cast<BGM_TYPE>(b)]);
	}

	for (int s = 0; s < static_cast<int>(SE_TYPE::MAX); s++)
	{
		DeleteSoundMem(se_[static_cast<SE_TYPE>(s)]);
	}
}
