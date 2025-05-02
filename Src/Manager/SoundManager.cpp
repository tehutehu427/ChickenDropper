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
	//BGM�̃p�X
	bgmPass_[BGM_TYPE::TITLE] = "title.mp3";
	bgmPass_[BGM_TYPE::SELECT] = "Select.wav";
	bgmPass_[BGM_TYPE::BATTLE_FIRST_HALF] = "BattleFirstHalf.mp3";
	bgmPass_[BGM_TYPE::BATTLE_SECOND_HALF] = "BattleSecondHalf.mp3";
	bgmPass_[BGM_TYPE::RESULT] = "Result2.mp3";
}

void SoundManager::BGMLoad(BGM_TYPE _bgm)
{
	//�w�肵��BGM�����[�h
	bgm_[_bgm] = LoadSoundMem((Application::PATH_BGM + bgmPass_[_bgm]).c_str());
}

void SoundManager::SEInit(void)
{
	//SE�̃p�X
	sePass_[SE_TYPE::CLICK] = "Click.mp3";
	sePass_[SE_TYPE::CANCEL] = "Cancel.mp3";
	sePass_[SE_TYPE::FALL ] = "Fall.mp3";
	sePass_[SE_TYPE::ATTACK ] = "Attack.mp3";
	sePass_[SE_TYPE::TILEBREAK] = "TileBreak.mp3";
	sePass_[SE_TYPE::CURTAIN] = "CurtainOpen.mp3";
}

void SoundManager::SELoad(SE_TYPE _se)
{
	////�w�肵��SE�����[�h
	se_[_se] = LoadSoundMem((Application::PATH_SE + sePass_[_se]).c_str());
}

void SoundManager::PlayBGM(BGM_TYPE _bgm, int _playType, int _volumePar, bool _topPositionFlag)
{
	ChangeVolumeSoundMem((255 * _volumePar / 100), bgm_[_bgm]);
	PlaySoundMem(bgm_[_bgm], _playType, _topPositionFlag);
}

void SoundManager::PlaySE(SE_TYPE _se, int _playType, int _volumePar, bool _topPositionFlag)
{
	ChangeVolumeSoundMem((255 * _volumePar / 100), se_[_se]);
	PlaySoundMem(se_[_se], _playType, _topPositionFlag);
}

bool SoundManager::CheckBGMPlay(BGM_TYPE _bgm)
{
	return CheckSoundMem(bgm_[_bgm]);
}

bool SoundManager::CheckSEPlay(SE_TYPE _se)
{
	return CheckSoundMem(se_[_se]);
}

void SoundManager::StopBGM(BGM_TYPE _bgm)
{
	StopSoundMem(bgm_[_bgm]);
}

void SoundManager::StopSE(SE_TYPE _se)
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
