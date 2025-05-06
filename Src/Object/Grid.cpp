#include<Dxlib.h>
#include"../Utility/Utility.h"
#include"Grid.h"

//デフォルトコンストラクタ
Grid::Grid(void)
{

}

//デストラクタ
Grid::~Grid(void)
{

}

//初期化処理
bool Grid::Init(void)
{
	return true;
}

//更新処理
void Grid::Update(void)
{

}

//描画処理
void Grid::Draw(void)
{
	//Xグリッド線
	VECTOR sPos = { -HLEN, 0.0f,0.0f };
	VECTOR ePos = { HLEN,0.0f,0.0f };
	for (int gx = -HNUM; gx < HNUM; gx++)
	{
		sPos.z = gx * TERM;
		ePos.z = gx * TERM;
		DrawLine3D(sPos, ePos, Utility::COLOR_RED);
		DrawSphere3D(ePos, RADIUS, DIV_NUM, Utility::COLOR_RED, Utility::COLOR_RED, true);
	}

	//Zグリッド線
	sPos = { 0.0f,0.0f,-HLEN };
	ePos = { 0.0f,0.0f,HLEN };
	for (int gz = -HNUM; gz < HNUM; gz++)
	{
		sPos.x = gz * TERM;
		ePos.x = gz * TERM;
		DrawLine3D(sPos, ePos, Utility::COLOR_BLUE);
		DrawSphere3D(ePos, RADIUS, DIV_NUM, Utility::COLOR_BLUE, Utility::COLOR_BLUE, true);
	}
}

//解放処理
bool Grid::Release(void)
{
	return true;
}
