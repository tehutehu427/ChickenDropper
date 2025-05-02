//#pragma once

#include<string>

class Application
{
public:
	//定数
	//-------------------------

	//画面サイズ

	//拡大率
	static constexpr float MAGNIFICATION = 4.0f / 5.0f;
	
	//フルスクリーン 
	static constexpr int SCREEN_SIZE_X = 1920 * MAGNIFICATION;
	static constexpr int SCREEN_SIZE_Y = 1080 * MAGNIFICATION;

	//static constexpr int SCREEN_SIZE_X = 1200;
	//static constexpr int SCREEN_SIZE_Y = 800;

	//フレームレート関係
	static constexpr int ONE_SEC_TO_FRAME = 1000;		//1秒をミリ秒変換
	static constexpr int DRAW_FPS_SHIFT_POS_X = -110;	//フレームレート描画用

	//データパス
	static const std::string PATH_IMAGE;	//画像のパス
	static const std::string PATH_MODEL;	//モデルのパス
	static const std::string PATH_EFFECT;	//エフェクトのパス
	static const std::string PATH_BGM;		//BGMのパス
	static const std::string PATH_SE;		//SEのパス


	//メンバー関数
	//-----------------------------
	
	//初期化処理
	bool Init(void);	
	//ゲームループ処理
	void Run(void);		
	//解放処理
	bool Release(void);	

	//シングルトン化
	//--------------------------
	//外部から静的にインスタンスの生成をする
	static void CreateInstance(void);

	//外部でインスタンスを利用可能とするために、
	//インスタンスを返す関数を作る
	static Application& GetInstance(void);

private:
	//シングルトン化
	//---------------------------
	
	//コンストラクタ
	Application(void);		
	//デストラクタ
	~Application(void);		

	//コピーコンストラクタを利用できないようにする
	Application(const Application& ins);

	//インスタンスの破棄
	void Destroy(void);

	//メンバー変数
	//--------------------------
	//静的なインスタンス格納領域
	static Application* instance_;

	//フレームレート関連
	//---------------------------------
	int currentTime_ = 0;			//現在時間
	int lastFrameTime_ = 0;			//前回のフレーム実行時の時間

	int frameCnt_ = 0;				//フレームカウント用
	int updateFrameRateTime_ = 0;	//フレームレートを更新した時間

	float frameRate_ = 0.f;			//フレームレート(表示用)

	bool isDrawFrameRate_ = false;	//フレームレートの表示フラグ(true:表示)

	//フレームレート計算
	void CalcFrameRate();

	//フレームレート表示(デバッグ用)
	void DrawFrameRate();
};