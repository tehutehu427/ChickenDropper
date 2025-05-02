#include<DxLib.h>
#include<sstream>
#include"../Common/Vector2.h"
#include"../Common/Vector2F.h"
#include"NodyUtility.h"

//文字列分割の機能
std::vector <std::string> NodyUtility::Split(std::string& line, char delimiter)
{
	//動的配列の戻り値格納領域の確保
	std::vector<std::string> result;

	//文字列操作用のクラスに置き換え
	std::istringstream lineStream(line);

	//データの格納領域
	std::string field;

	//streamを順番に読み込み
	//getlineを使い
	//第3引数で指定したデリミタ記号までのデータを
	//第2引数の変数に格納する
	while (getline(lineStream, field, delimiter))
	{
		//動的配列に追加する
		result.push_back(field);
	}

	return result;
}

//四捨五入用の関数(float)
int NodyUtility::Round(float value)
{
	return static_cast <int>(roundf(value));
}

//オーバーロード関数(引数と戻り値を変更している関数)
//四捨五入用の関数(Vector2)
Vector2 NodyUtility::Round(Vector2F value)
{
	Vector2 ret;
	ret.x = static_cast<int>(round(value.x));
	ret.y = static_cast<int>(round(value.y));
	return ret;
}

float NodyUtility::Deg2RadF(const float _deg)
{
	return _deg * DEG2RAD;
}

float NodyUtility::Rad2DegF(const float _rad)
{
	return _rad * RAD2DEG;
}
