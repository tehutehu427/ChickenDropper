#include<DxLib.h>
#include"../Utility/Utility.h"
#include"Camera.h"

//デフォルトコンストラクタ
Camera::Camera(void)
{
	pos_ = Utility::VECTOR_ZERO;
	angles_ = Utility::VECTOR_ZERO;
}

//デストラクタ
Camera::~Camera(void)
{

}

//初期化処理
bool Camera::Init(void)
{
	//カメラの位置
	pos_ = Utility::VECTOR_ZERO;

	//カメラの角度
	angles_ = Utility::VECTOR_ZERO;

	return true;
}

//更新処理
void Camera::Update(void)
{

}

//カメラ設定(毎フレーム実行)
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

//描画処理
void Camera::Draw(void)
{

}

void Camera::SetCamerawork(const VECTOR _pos, const VECTOR _angles)
{
	pos_ = _pos;
	angles_ = { Utility::Deg2RadF(_angles.x) ,Utility::Deg2RadF(_angles.y)  ,Utility::Deg2RadF(_angles.z) };
}

