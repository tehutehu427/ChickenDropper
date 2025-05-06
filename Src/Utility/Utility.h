#pragma once

#include<Dxlib.h>
#include<string>
#include<vector>

//クラスの前方宣言
class Vector2;
class Vector2F;

class Utility
{
public:
	//列挙型
	//---------------------
	//方向
	enum class DIR_2D	//2D用の方向
	{
		UP			//上方向
		,RIGHT		//右方向
		,DOWN		//下方向
		,LEFT		//左方向
		,MAX
	};

	enum class DIR_3D	//3D用の方向
	{
		FRONT		//前方向
		, RIGHT		//右方向
		, BACK		//後ろ方向
		, LEFT		//左方向
		, MAX
	};

	//定数
	//----------------------------------------------------
	static constexpr float DEG2RAD = DX_PI_F / 180.0f;		//デグリーのラジアン変換用
	static constexpr float RAD2DEG = 180.0f / DX_PI_F;		//ラジアンのデグリー変換用

	static constexpr VECTOR VECTOR_ZERO = { 0.0f,0.0f,0.0f };		//VECTOR型の０の初期化用
	static constexpr VECTOR VECTOR_ONE = { 1.0f,1.0f,1.0f };		//VECTOR型の１の初期化用

	//デルタタイム
	static constexpr float DEFAULT_FPS = 60.0f;					//FPS
	static constexpr float DELTA_TIME = 1.0f / DEFAULT_FPS;		//デルタタイム

	//色
	static constexpr int COLOR_RED = 0xff0000;		//赤
	static constexpr int COLOR_GREEN = 0x00ff00;	//緑
	static constexpr int COLOR_BLUE = 0x0000ff;		//青
	static constexpr int COLOR_YELLOW = 0xffff00;	//黄
	static constexpr int COLOR_WHITE = 0xffffff;	//白
	static constexpr int COLOR_BLACK = 0x000000;	//黒
	static constexpr int COLOR_GRAY = 0xaaaaaa;		//灰

	//関数
	//----------------------------------------------------

	//文字列分割の機能
	static std::vector <std::string> Split(const std::string& line, const char delimiter);

	//四捨五入用の関数(float)
	static const int Round(const float value);

	//オーバーロード関数
	static const Vector2 Round(const Vector2F value);

	//DegreeのRadian変換
	static const float Deg2RadF(const float _deg);

	//RadianのDegree変換
	static const float Rad2DegF(const float _rad);
};