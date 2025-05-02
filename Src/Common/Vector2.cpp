#include"Vector2F.h"
#include"Vector2.h"

//デフォルトコンストラクタ
Vector2::Vector2(void)
{
	x = 0.0f;
	y = 0.0f;
}

//コンストラクタ
Vector2::Vector2(int ix, int iy)
{
	x = ix;
	y = iy;
}

//デストラクタ
Vector2::~Vector2(void)
{

}

//Vector2をVector2F型に変換
Vector2F Vector2::ToVector2F()
{
	Vector2F ret;
	ret.x = static_cast<float>(x);
	ret.y = static_cast<float>(y);
	return ret;
}