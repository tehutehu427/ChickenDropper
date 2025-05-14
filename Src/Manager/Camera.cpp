#include<DxLib.h>
#include"../Utility/Utility.h"
#include"Camera.h"

Camera::Camera(void)
{
	pos_ = Utility::VECTOR_ZERO;
	angles_ = Utility::VECTOR_ZERO;
}

Camera::~Camera(void)
{

}

bool Camera::Init(void)
{
	//�J�����̈ʒu
	pos_ = Utility::VECTOR_ZERO;

	//�J�����̊p�x
	angles_ = Utility::VECTOR_ZERO;

	return true;
}

void Camera::Update(void)
{

}

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

void Camera::Draw(void)
{

}

void Camera::SetCamerawork(const VECTOR _pos, const VECTOR _angles)
{
	//���W�̐ݒ�
	pos_ = _pos;

	//�p�x�̐ݒ�
	angles_ = { Utility::Deg2RadF(_angles.x) ,Utility::Deg2RadF(_angles.y)  ,Utility::Deg2RadF(_angles.z) };
}

