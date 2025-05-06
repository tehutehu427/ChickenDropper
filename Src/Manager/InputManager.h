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
	const bool IsKeyDown(const int _keyCode);

	//たった今押されたか
	const bool IsKeyPush(const int _keyCode);

	//押し続けられているか
	const bool IsKeyKeep(const int _keyCode);

	//たった今離されたか
	const bool IsKeyRelease(const int _keyCode);

	//コントローラー
	//---------------------------
	
	//制御処理(コントローラー)
	void StepPadInput(void);

	//単純に押されているか(コントローラー)
	const bool IsJoypadKeyDown(const int _padNum, const int _keyCode);

	//たった今押されたか(コントローラー)
	const bool IsJoypadKeyPush(const int _padNum, const int _keyCode);

	//押し続けられているか(コントローラー)
	const bool IsJoypadKeyKeep(const int _padNum, const int _keyCode);

	//たった今離されたか(コントローラー)
	const bool IsJoypadKeyRelease(const int _padNum, const int _keyCode);

	//何フレーム押し続けられたか(コントローラー)
	const KEEP_FUNCTION HowLongJoypadKeyKeep(const int _padNum, const int _keyCode, const int _keepTime);

	//インスタンスの削除
	void Destroy(void);

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
};