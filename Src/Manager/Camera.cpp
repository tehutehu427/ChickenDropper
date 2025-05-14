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
	//カメラの位置
	pos_ = Utility::VECTOR_ZERO;

	//カメラの角度
	angles_ = Utility::VECTOR_ZERO;

	return true;
}

void Camera::Update(void)
{

}

void Camera::SetBeforeDraw(void)
{
	//クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(10.0f, 30000.0f);

	//カメラの設定(位置と角度による制御)
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
	//座標の設定
	pos_ = _pos;

	//角度の設定
	angles_ = { Utility::Deg2RadF(_angles.x) ,Utility::Deg2RadF(_angles.y)  ,Utility::Deg2RadF(_angles.z) };
}

