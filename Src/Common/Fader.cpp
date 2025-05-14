#include <DxLib.h>
#include "../Utility/Utility.h"
#include "../Application.h"
#include "Fader.h"

Fader::Fader(void)
{
	state_ = STATE::NONE;
	alpha_ = 0.0f;
	isPreEnd_ = true;
	isEnd_ = true;
}

Fader::~Fader(void)
{
}

void Fader::Init(void)
{
	state_ = STATE::NONE;
	alpha_ = 0.0f;
	isPreEnd_ = true;
	isEnd_ = true;
}

void Fader::Update(void)
{
	//フェードの終了判定
	if (isEnd_)
	{
		// フェード処理が終了していたら何もしない
		return;
	}
	
	if (isPreEnd_)
	{
		// 1フレーム後(Draw後)に終了とする
		isEnd_ = true;
		return;
	}

	//フェードの状態
	switch (state_)
	{
	case STATE::NONE:
		//何もしない
		return;

	case STATE::FADE_OUT:
		//フェードアウト
		alpha_ += SPEED_ALPHA;
		if (alpha_ > ALPHA_MAX)
		{
			// フェード終了
			alpha_ = ALPHA_MAX;
			isPreEnd_ = true;
		}
		break;

	case STATE::FADE_IN:
		//フェードイン
		alpha_ -= SPEED_ALPHA;
		if (alpha_ < 0)
		{
			// フェード終了
			alpha_ = 0;
			isPreEnd_ = true;
		}
		break;

	default:
		return;

	}

}

void Fader::Draw(void)
{

	switch (state_)
	{
	case STATE::NONE:
		//何もしない
		break;
	case STATE::FADE_OUT:		
	case STATE::FADE_IN:
		//アルファブレンド
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
		DrawBox(
			0, 0,
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y,
			Utility::COLOR_BLACK, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}

}

const Fader::STATE Fader::GetState(void)
{
	return state_;
}

const bool Fader::IsEnd(void)
{
	return isEnd_;
}

void Fader::SetFade(const STATE state)
{
	//状態
	state_ = state;
	
	//通常状態
	if (state_ != STATE::NONE)
	{
		//何もしない
		isPreEnd_ = false;
		isEnd_ = false;
	}
	//フェードインの前処理
	if (state_ == STATE::FADE_IN)
	{
		//真っ暗にするためアルファ値を最大に
		alpha_ = ALPHA_MAX;
	}
	//フェードアウトの前処理
	else if (state_ == STATE::FADE_OUT)
	{
		//真っ暗からだんだん明るくするためにアルファ値を0にする
		alpha_ = 0.0f;
	}
}
