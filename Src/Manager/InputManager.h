#pragma once

class InputManager
{
public:
	//列挙型
	//-----------------------

	//キーの押し続けられたかどうかの機能
	enum class KEEP_FUNCTION
	{
		NONE			//そもそも押されていない
		,KEEP_TRUE		//押し続けられた
		,KEEP_FALSE		//途中で離された
	};

	//定数
	//-----------------------
	static constexpr int KEY_BUF_LEN = 256;		//キーボードのキーの数

	static constexpr int PAD_KEY_BUF_LEN = 32;	//ゲームパッドの入力領域数

	static constexpr int PAD_NUM = 4;			//コントローラーの最大接続数

	//初期化処理
	void InitInput(void);

	//制御処理
	void StepInput(void);

	//単純に押されているか
	bool IsKeyDown(int _keyCode);

	//たった今押されたか
	bool IsKeyPush(int _keyCode);

	//押し続けられているか
	bool IsKeyKeep(int _keyCode);

	//たった今離されたか
	bool IsKeyRelease(int _keyCode);

	//コントローラー
	//---------------------------
	
	//制御処理(コントローラー)
	void StepPadInput(void);

	//単純に押されているか(コントローラー)
	bool IsJoypadKeyDown(int _padNum, int _keyCode);

	//たった今押されたか(コントローラー)
	bool IsJoypadKeyPush(int _padNum, int _keyCode);

	//押し続けられているか(コントローラー)
	bool IsJoypadKeyKeep(int _padNum, int _keyCode);

	//たった今離されたか(コントローラー)
	bool IsJoypadKeyRelease(int _padNum, int _keyCode);

	//何フレーム押し続けられたか(コントローラー)
	KEEP_FUNCTION HowLongJoypadKeyKeep(int _padNum, int _keyCode, int _keepTime);

	//解放処理
	void InputRelease(void);

	//シングルトン化
	//----------------------------
	
	//外部から静的にインスタンスの生成をする
	static void CreateInstance(void);

	//外部でインスタンスを利用可能とするために
	//インスタンスを返す関数を作る(参照型でも、ポインタ型でもよい)
	static InputManager& GetInstance(void);

private:
	//メンバ変数
	//-----------------------
	//現在フレームのキー情報
	char currentKeyBuf_[KEY_BUF_LEN] = { 0 };

	//前フレームのキー情報
	char preKeyBuf_[KEY_BUF_LEN] = { 0 };

	//現在フレームのキー情報(コントローラー)
	int currentPadKeyBuf_[PAD_NUM];

	//前フレームのキー情報(コントローラー)
	int prePadKeyBuf_[PAD_NUM];

	//キーの押された時間(コントローラー)
	int pushPadKeyTime_[PAD_NUM][PAD_KEY_BUF_LEN];

	//キーの押されたかのトリガー(コントローラー)
	bool pushPadKeyTrg_[PAD_NUM][PAD_KEY_BUF_LEN];

	//静的なインスタンス格納領域
	static InputManager* instance_;

	//シングルトン化
	//--------------------------------

	void Destroy(void);
};