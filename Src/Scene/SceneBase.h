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
	virtual ~SceneBase(void);			

	//初期化処理
	virtual bool Init(void);					
	//更新処理
	virtual void Update(void);					
	//描画処理
	virtual void Draw(void);					
	//解放処理
	virtual bool Release(void);
};