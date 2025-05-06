#pragma once

//クラスの前方宣言
class Vector2F;

class Vector2
{
public:
	//メンパー関数
	//-------------------------
	
	//デフォルトコンストラクタ
	Vector2(void);

	//コンストラクタ
	Vector2(int ix, int iy);

	//デストラクタ
	~Vector2(void);

	//Vector2をVector2F型に変換
	Vector2F ToVector2F(void)const;

	//メンバー変数
	int x;
	int y;

private:

};