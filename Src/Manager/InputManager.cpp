#include<DxLib.h>
#include<cmath>
#include"InputManager.h"

//シングルトン化
//-----------------------------------
//instanceの初期化
InputManager* InputManager::instance_ = nullptr;

//初期化処理
void InputManager::InitInput(void)
{
	//入力制御の初期化
	for (int i = 0; i < KEY_BUF_LEN; i++)
	{
		currentKeyBuf_[i] = '\0';
		preKeyBuf_[i] = '\0';
	}

	//入力制御の初期化(コントローラー)
	for (int n = 0; n < PAD_NUM; n++)
	{
		currentPadKeyBuf_[n] = 0;
		prePadKeyBuf_[n] = 0;
	
		for (int k = 0; k < PAD_KEY_BUF_LEN; k++)
		{
			pushPadKeyTime_[n][k] = 0;
			pushPadKeyTrg_[n][k] = false;
		}
	}
}

//制御処理
void InputManager::StepInput(void)
{
	for (int i = 0; i < KEY_BUF_LEN; i++)
	{
		//前フレームのキー情報変数に記録しておく
		preKeyBuf_[i] = currentKeyBuf_[i];
	}

	//現在のキー情報を取得
	GetHitKeyStateAll(currentKeyBuf_);
}

//単純に押されているか
bool InputManager::IsKeyDown(int _keyCode)
{
	//現フレで押されている(前フレは関係ない)
	if (currentKeyBuf_[_keyCode] == 1)
	{
		return true;
	}

	return false;
}

//たった今押されたか
bool InputManager::IsKeyPush(int _keyCode)
{
	//前フレで押されていない　かつ　現フレで押されている
	if (preKeyBuf_[_keyCode] == 0 && currentKeyBuf_[_keyCode] == 1)
	{
		return true;
	}

	//押されていないのでfalse
	return false;
}

//押し続けられているか
bool InputManager::IsKeyKeep(int _keyCode)
{
	//前フレで押されている　かつ　現フレで押されている
	if (preKeyBuf_[_keyCode] == 1 && currentKeyBuf_[_keyCode] == 1)
	{
		return true;
	}

	//押されていないのでfalse
	return false;
}

//たった今離されたか
bool InputManager::IsKeyRelease(int _keyCode)
{
	//前フレで押されている　かつ　現フレで押されていない
	if (preKeyBuf_[_keyCode] == 1 && currentKeyBuf_[_keyCode] == 0)
	{
		return true;
	}

	//押されていないのでfalse
	return false;
}

//制御処理(コントローラー)
void InputManager::StepPadInput(void)
{
	//現在接続されているコントローラーの数を取得
	int padNumber = GetJoypadNum();

	for (int n = 0; n < PAD_NUM; n++)
	{
		//前フレームのキー情報変数に記録しておく
		prePadKeyBuf_[n] = currentPadKeyBuf_[n];

		//パッドが一つも繋がっていないとき
		if (padNumber == 0)
		{
			//配列０にキーボードの現在のキー情報を取得
			currentPadKeyBuf_[0] = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		}
		else
		{
			//現在のキー情報を取得
			currentPadKeyBuf_[n] = GetJoypadInputState(n + 1);
		}
	}

	for (int k = 0; k < PAD_KEY_BUF_LEN; k++)
	{
		for (int n = 0; n < PAD_NUM; n++)
		{
			if (!IsJoypadKeyKeep(n + 1, pow(2,k)))
			{
				pushPadKeyTime_[n][k] = 0;

				pushPadKeyTrg_[n][k] = false;
			}
			else
			{
				pushPadKeyTrg_[n][k] = true;
			}

			if (pushPadKeyTrg_[n][k])
			{
				pushPadKeyTime_[n][k]++;
			}
		}
	}
}

//単純に押されているか(コントローラー)
bool InputManager::IsJoypadKeyDown(int _padNum, int _keyCode)
{
	//コントローラーの番号合わせ用
	int padNumber;

	//コントローラーとキーボード用
	if (_padNum == DX_INPUT_KEY_PAD1)
	{
		//１番に合わせる
		padNumber = DX_INPUT_PAD1 - 1;
	}
	else
	{
		//配列用に数字を合わせる
		padNumber = _padNum - 1;
	}
	
	//現フレで押されている(前フレは関係ない)
	if ((currentPadKeyBuf_[padNumber] & _keyCode) != 0)
	{
		return true;
	}

	//押されていないのでfalse
	return false;
}

//たった今押されたか(コントローラー)
bool InputManager::IsJoypadKeyPush(int _padNum, int _keyCode)
{
	//コントローラーの番号合わせ用
	int padNumber;

	//コントローラーとキーボード用
	if (_padNum == DX_INPUT_KEY_PAD1)
	{
		//１番に合わせる
		padNumber = DX_INPUT_PAD1 - 1;
	}
	else
	{
		//配列用に数字を合わせる
		padNumber = _padNum - 1;
	}

	//前フレで押されていない　かつ　現フレで押されている
	if ((prePadKeyBuf_[padNumber] & _keyCode) == 0 && (currentPadKeyBuf_[padNumber] & _keyCode) != 0)
	{
		return true;
	}

	//押されていないのでfalse
	return false;
}

//押し続けられているか(コントローラー)
bool InputManager::IsJoypadKeyKeep(int _padNum, int _keyCode)
{
	//コントローラーの番号合わせ用
	int padNumber;

	//コントローラーとキーボード用
	if (_padNum == DX_INPUT_KEY_PAD1)
	{
		//１番に合わせる
		padNumber = DX_INPUT_PAD1 - 1;
	}
	else
	{
		//配列用に数字を合わせる
		padNumber = _padNum - 1;
	}

	//前フレで押されている　かつ　現フレで押されている
	if ((prePadKeyBuf_[padNumber] & _keyCode) != 0 && (currentPadKeyBuf_[padNumber] & _keyCode) != 0)
	{
		return true;
	}

	//押されていないのでfalse
	return false;
}

//たった今離されたか(コントローラー)
bool InputManager::IsJoypadKeyRelease(int _padNum, int _keyCode)
{
	//コントローラーの番号合わせ用
	int padNumber;

	//コントローラーとキーボード用
	if (_padNum == DX_INPUT_KEY_PAD1)
	{
		//１番に合わせる
		padNumber = DX_INPUT_PAD1 - 1;
	}
	else
	{
		//配列用に数字を合わせる
		padNumber = _padNum - 1;
	}

	//前フレで押されている　かつ　現フレで押されていない
	if ((prePadKeyBuf_[padNumber] & _keyCode) != 0 && (currentPadKeyBuf_[padNumber] & _keyCode) == 0)
	{
		return true;
	}

	//押されていないのでfalse
	return false;
}

InputManager::KEEP_FUNCTION InputManager::HowLongJoypadKeyKeep(int _padNum, int _keyCode, int _keepTime)
{
	//コントローラーの番号合わせ用
	int padNumber;

	int codeArray = 0;

	int keyCode = _keyCode;

	while (keyCode / 2 != 0)
	{
		codeArray++;

		keyCode /= 2;
	}

	//コントローラーとキーボード用
	if (_padNum == DX_INPUT_KEY_PAD1)
	{
		//１番に合わせる
		padNumber = DX_INPUT_PAD1 - 1;
	}
	else
	{
		//配列用に数字を合わせる
		padNumber = _padNum - 1;
	}

	if (pushPadKeyTime_[padNumber][codeArray] < 1)
	{
		return KEEP_FUNCTION::NONE;
	}
	else if (pushPadKeyTime_[padNumber][codeArray] >= _keepTime)
	{
		//初期化
		pushPadKeyTime_[padNumber][codeArray] = 0;

		return KEEP_FUNCTION::KEEP_TRUE;
	}
	else
	{
		//初期化
		pushPadKeyTime_[padNumber][codeArray] = 0;

		return KEEP_FUNCTION::KEEP_FALSE;
	}
}

//シングルトン化
//--------------------------
//外部から静的にインスタンスの生成をする
void InputManager::CreateInstance(void)
{
	//instance_がnullptrの場合は初期化されたのち生成処理を行う
	if (instance_ == nullptr)
	{
		instance_ = new InputManager();
	}

	instance_->InitInput();
}

//外部でインスタンスを利用可能とするために
//インスタンスを返す関数を作る(参照型でも、ポインタ型でもよい)
InputManager& InputManager::GetInstance(void)
{
	return *instance_;
}

//インスタンスの破棄
void InputManager::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}