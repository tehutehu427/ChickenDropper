#pragma once

class Fader
{

public:

	// フェードが進む速さ
	static constexpr float SPEED_ALPHA = 3.0f;

	//アルファ値の最大
	static constexpr float ALPHA_MAX = 255.0f;

	// 状態
	enum class STATE
	{
		NONE,		// 通常状態
		FADE_OUT,	// 徐々に暗転
		FADE_IN		// 徐々に明転
	};

	// コンストラクタ
	Fader(void);

	// デストラクタ
	~Fader(void);

	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);

	// 状態の取得
	const STATE GetState(void);

	// フェード処理が終了しているか
	const bool IsEnd(void);

	// 指定フェードを開始する
	void SetFade(const STATE state);

private:

	// 状態
	STATE state_;

	// 透明度
	float alpha_;

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;

};
