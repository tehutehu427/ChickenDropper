#include<Dxlib.h>
#include"../Utility/Utility.h"
#include"Grid.h"

//�f�t�H���g�R���X�g���N�^
Grid::Grid(void)
{

}

//�f�X�g���N�^
Grid::~Grid(void)
{

}

//����������
bool Grid::Init(void)
{
	return true;
}

//�X�V����
void Grid::Update(void)
{

}

//�`�揈��
void Grid::Draw(void)
{
	//X�O���b�h��
	VECTOR sPos = { -HLEN, 0.0f,0.0f };
	VECTOR ePos = { HLEN,0.0f,0.0f };
	for (int gx = -HNUM; gx < HNUM; gx++)
	{
		sPos.z = gx * TERM;
		ePos.z = gx * TERM;
		DrawLine3D(sPos, ePos, Utility::COLOR_RED);
		DrawSphere3D(ePos, RADIUS, DIV_NUM, Utility::COLOR_RED, Utility::COLOR_RED, true);
	}

	//Z�O���b�h��
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

//�������
bool Grid::Release(void)
{
	return true;
}
