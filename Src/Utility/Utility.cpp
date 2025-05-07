#include<DxLib.h>
#include<sstream>
#include"../Common/Vector2.h"
#include"../Common/Vector2F.h"
#include"Utility.h"

//四捨五入用の関数(float)
const int Utility::Round(const float value)
{
	return static_cast <int>(roundf(value));
}

//オーバーロード関数(引数と戻り値を変更している関数)
//四捨五入用の関数(Vector2)
const Vector2 Utility::Round(const Vector2F value)
{
	Vector2 ret;
	ret.x = static_cast<int>(round(value.x));
	ret.y = static_cast<int>(round(value.y));
	return ret;
}

const float Utility::Deg2RadF(const float _deg)
{
	return _deg * DEG2RAD;
}

const float Utility::Rad2DegF(const float _rad)
{
	return _rad * RAD2DEG;
}
