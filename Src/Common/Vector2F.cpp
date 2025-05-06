#include<tgmath.h>
#include"Vector2.h"
#include "Vector2F.h"

//デフォルトコンストラクタ
Vector2F::Vector2F(void)
{
	x = 0;
	y = 0;
}

//コンストラクタ
Vector2F::Vector2F(float ix, float iy)
{
	x = ix;
	y = iy;
}

//デストラクタ
Vector2F::~Vector2F(void)
{

}

//Vector2F型からVector2型に変更
Vector2 Vector2F::ToVector2(void)const
{
	Vector2 ret;
	ret.x = static_cast<int>(roundf(x));
	ret.y = static_cast<int>(roundf(y));
	return ret;
}