#pragma once

class Grid
{
public:
	//定数
	//--------------------------
	//線の長さ
	static constexpr float LEN = 1200.0f;

	//線の長さの半分
	static constexpr float HLEN = LEN / 2.0f;

	//線の間隔
	static constexpr float TERM = 50.0f;

	//線の数
	static constexpr int NUM = static_cast<int>(LEN / TERM);

	//線の数の半分
	static constexpr int HNUM = NUM / 2;

	//球の半径
	static constexpr float RADIUS = 15.0f;

	//球の面数
	static constexpr int DIV_NUM = 10;

	//メンバー関数
	//--------------------------
	
	//デフォルトコンストラクタ
	Grid(void);						
	//デストラクタ
	~Grid(void);					

	//初期化処理
	bool Init(void);
	//更新処理
	void Update(void);
	//描画処理
	void Draw(void);
	//解放処理
	bool Release(void);

};