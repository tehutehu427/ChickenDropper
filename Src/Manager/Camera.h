#pragma once
#include<DxLib.h>

class Camera
{
public:
	//メンバー関数
	//-------------------
	
	//デフォルトコンストラクタ
	Camera(void);					
	//デストラクタ
	~Camera(void);					

	//初期化処理
	bool Init(void);					
	//更新処理
	void Update(void);					

	//カメラ設定(毎フレーム実行)
	void SetBeforeDraw(void);			
	//描画処理
	void Draw(void);					
	//解放処理(基本的に最後の１回だけ実行)
	bool Release(void);					

	/// <summary>
	/// カメラワークの設定
	/// </summary>
	/// <param name="_pos">カメラ位置</param>
	/// <param name="_angles">カメラ角度(ラジアン変換不要)</param>
	void SetCamerawork(const VECTOR _pos, const VECTOR _angles);

private:
	//カメラの位置
	VECTOR pos_;

	//カメラの角度
	VECTOR angles_;
};