//#pragma once

#include"../Common/Vector2F.h"
#include"SceneBase.h"

class SoundManager;

class SceneTitle : public SceneBase
{
public:
	//定数
	//-------------------

	//音量(SE)
	//-----------------

	//決定音
	static constexpr int CLICK_VOLUME = 70;

	//文字の位置用
	static constexpr int CHAR_POS_X = 300;	//文字のX移動用
	static constexpr int CHAR_POS_Y = 200;	//文字のY移動用

	//メンバー関数
	//-------------------

	//初期化処理
	bool Init(void) override;		
	//更新処理
	void Update(void) override;		
	//描画処理
	void Draw(void) override;		
	//解放処理
	bool Release(void) override;	

private:
	//ポインタ
	//--------------------------

	//サウンドのインスタンス
	SoundManager* sound_;

	//メンバー変数
	//--------------------------
	//背景画像の格納領域
	int bgImage_;

	//雲画像
	int cloudImage_;

	//雲画像の座標
	Vector2F cloudPos_;

	//タイトルロゴ
	int logoImage_;
};