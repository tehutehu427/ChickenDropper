#pragma once

class Vector2;

class Vector2F
{
public:
	//メンパー関数
	//-------------------------
	
	//デフォルトコンストラクタ
	Vector2F(void);

	//コンストラクタ
	Vector2F(float ix, float iy);

	//デストラクタ
	~Vector2F(void);

	//Vector2F型からVector2型に変更
	Vector2 ToVector2(void)const;

	//メンバー変数
	float x;
	float y;

private:

};