#include"Vector2F.h"
#include"Vector2.h"

//�f�t�H���g�R���X�g���N�^
Vector2::Vector2(void)
{
	x = 0.0f;
	y = 0.0f;
}

//�R���X�g���N�^
Vector2::Vector2(int ix, int iy)
{
	x = ix;
	y = iy;
}

//�f�X�g���N�^
Vector2::~Vector2(void)
{

}

//Vector2��Vector2F�^�ɕϊ�
Vector2F Vector2::ToVector2F()
{
	Vector2F ret;
	ret.x = static_cast<float>(x);
	ret.y = static_cast<float>(y);
	return ret;
}