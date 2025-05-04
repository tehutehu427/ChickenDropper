#include<DxLib.h>
#include"../Utility/Utility.h"
#include"Camera.h"

//�f�t�H���g�R���X�g���N�^
Camera::Camera(void)
{
	pos_ = Utility::VECTOR_ZERO;
	angles_ = Utility::VECTOR_ZERO;
}

//�f�X�g���N�^
Camera::~Camera(void)
{

}

//����������
bool Camera::Init(void)
{
	//�J�����̈ʒu
	pos_ = Utility::VECTOR_ZERO;

	//�J�����̊p�x
	angles_ = { Utility::Deg2RadF(60.0f), 0.0f,0.0f};

	return true;
}

//�X�V����
void Camera::Update(void)
{

}

//�J�����ݒ�(���t���[�����s)
void Camera::SetBeforeDraw(void)
{
	//�N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(10.0f, 30000.0f);

	//�J�����̐ݒ�(�ʒu�Ɗp�x�ɂ�鐧��)
	SetCameraPositionAndAngle(
		pos_
		, angles_.x
		, angles_.y
		, angles_.z
	);

}

//�`�揈��
void Camera::Draw(void)
{

}

//�������(��{�I�ɍŌ�̂P�񂾂����s)
bool Camera::Release(void)
{
	return true;
}

void Camera::SetCamerawork(VECTOR _pos, VECTOR _angles)
{
	pos_ = _pos;
	angles_ = { Utility::Deg2RadF(_angles.x) ,Utility::Deg2RadF(_angles.y)  ,Utility::Deg2RadF(_angles.z) };
}

