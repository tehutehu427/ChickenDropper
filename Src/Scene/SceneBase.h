#pragma once

#include<DxLib.h>

class SceneBase
{
public:
	//メンバー関数
	//-------------------------
	
	//デフォルトコンストラクタ
	SceneBase(void);					
	//デストラクタ
	virtual ~SceneBase(void) = 0;			

	//初期化処理
	virtual bool Init(void) = 0;					
	//更新処理
	virtual void Update(void) = 0;					
	//描画処理
	virtual void Draw(void) = 0;					
	//解放処理
	virtual bool Release(void) = 0;
};