#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include"Utility/NodyUtility.h"
#include"Manager/InputManager.h"
#include"Manager/EffectManager.h"
#include"Manager/SceneManager.h"
#include"Common/CommonData.h"
#include"Application.h"

//シングルトン化
//---------------------------
//instanceの初期化
Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_BGM = "Data/Sound/BGM/";
const std::string Application::PATH_SE = "Data/Sound/SE/";

//設定フレームレート(60分の1秒)
constexpr float FRAME_RATE (1000 / 60);

//コンストラクタ
Application::Application(void)
{

}

//デストラクタ
Application::~Application(void)
{

}

//初期化処理
bool Application::Init(void)
{
	//システム処理
	//------------------------
	SetWindowText("チキンドロッパー");					//ゲームウィンドウのタイトル
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);		//ゲームウィンドウのサイズと色モード
	ChangeWindowMode(true);								//ゲームウィンドウの表示設定(false:フルスクリーン)
	
	//DxLibの初期化
	if (DxLib_Init() == 1)								
	{
		OutputDebugString("DxLibの初期化失敗\n");
		return false;		//DxLibの初期化失敗
	}

	//インスタンスの用意
	//------------------------
	//コモンデータのインスタンス生成(シングルトン化)
	CommonData::CreateInstance();

	//シーンマネージャのインスタンス生成(シングルトン化)
	SceneManager::CreateInstance();

	//インプットマネージャのインスタンス生成(シングルトン化)
	InputManager::CreateInstance();

	return true;
}

//ゲームループ処理
void Application::Run(void)
{
	//ゲームループ
	//-------------------------
	while (ProcessMessage() == 0)
	{
		Sleep(1);		//システムに処理を返す

		//現在の時刻を取得
		currentTime_ = GetNowCount();

		//現在の時刻が、前回のフレーム時より
		//1/60秒経過していたら処理を実行する
		if (currentTime_ - lastFrameTime_ >= FRAME_RATE)
		{
			//フレーム実行時の時間を更新
			lastFrameTime_ = currentTime_;

			//フレーム数のカウント
			frameCnt_++;

			//ESCAPEキーが押されたら終了
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			{
				return;
			}

			//ゲームのメイン処理
			//-------------------------
			//シーンマネージャの更新処理(シングルトン化)
			SceneManager::GetInstance().Update();

			//描画処理
			//-------------------------
			SetDrawScreen(DX_SCREEN_BACK);	//描画する画面を裏の画面に設定
			ClearDrawScreen();				//描画する画面の内容を消去

			//シーンマネージャの描画処理(シングルトン化)
			SceneManager::GetInstance().Draw();

			//キー状態の取得処理(シングルトン化)
			InputManager::GetInstance().StepInput();

			//コントローラーの入力状態の取得処理(シングルトン化)
			InputManager::GetInstance().StepPadInput();


			//フレームレート計算
			CalcFrameRate();

			//フレームレートの表示(デバッグ)
			DrawFrameRate();

			//裏の画面を表の画面にコピー
			ScreenFlip();
		}
	}
}

//解放処理
bool Application::Release(void)
{
	//キーマネージャの解放処理
	InputManager::GetInstance().InputRelease();

	//管理マネージャの解放処理
	//--------------------------------
	//シーンマネージャの解放処理(シングルトン化)
	SceneManager::GetInstance().Release();

	//コモンデータの解放処理
	CommonData::GetInstance().Release();

	Destroy();			//インスタンスの破棄

	//システムの終了
	//----------------------
	DxLib_End();		//DXライブラリの終了

	return true;		//ゲーム終了
}

//シングルトン化
//---------------------------
//外部から静的にインスタンスの生成をする
void Application::CreateInstance(void)
{
	//instance_がnullptrの場合は初期化されたのち生成処理を行う
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}

	instance_->Init();
}

//外部でインスタンスを利用可能とするために、
//インスタンスを返す関数を作る
Application& Application::GetInstance(void)
{
	return *instance_;
}

//インスタンスの破棄
void Application::Destroy(void)
{
	delete instance_;		//インスタンスの削除
	instance_ = nullptr;	//インスタンスの格納領域を初期化

	//DxLib終了
	DxLib_End();
}

void Application::CalcFrameRate()
{
	//前回のフレームレート更新からの経過時間を求める
	int nDifTime = currentTime_ - updateFrameRateTime_;

	//前回のフレームレートを更新から
	//1秒以上経過していたらフレームレートを更新する
	if (nDifTime > ONE_SEC_TO_FRAME)
	{
		//フレーム回数を1ミリ秒に合わせる
		//小数まで出したのでfloatにキャスト
		float fFrameCnt = (float)(frameCnt_ * ONE_SEC_TO_FRAME);

		//フレームレートを求める
		//理想通りなら 60000 / 1000 で 60 になる
		frameRate_ = fFrameCnt / nDifTime;

		//フレームカウントをクリア
		frameCnt_ = 0;

		//フレームレート更新時間を更新
		updateFrameRateTime_ = currentTime_;
	}
}

void Application::DrawFrameRate()
{
	int fontHandle;
	int fontSize = 20;
	int fontTickness = 8;

	//フォント作成
	fontHandle = CreateFontToHandle(NULL, fontSize, fontTickness, NULL);

	if (InputManager::GetInstance().IsKeyPush(KEY_INPUT_TAB))
	{
		//フレームレートを描画する
		isDrawFrameRate_ = isDrawFrameRate_ ? false : true;
	}

	if (isDrawFrameRate_)
	{
		DrawFormatStringToHandle(SCREEN_SIZE_X + DRAW_FPS_SHIFT_POS_X, 2, NodyUtility::COLOR_YELLOW, fontHandle, "FPS[%.2f]", frameRate_);
	}

	//フォント解放
	DeleteFontToHandle(fontHandle);
}
