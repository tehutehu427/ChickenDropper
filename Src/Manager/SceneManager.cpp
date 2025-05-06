#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include"Camera.h"
#include"../Common/Fader.h"
#include"../Scene/SceneTitle.h"
#include"../Scene/SceneSelect.h"
#include"../Scene/SceneGame.h"
#include"../Scene/SceneResult.h"
#include"Resource.h"
#include"ResourceManager.h"
#include"SceneManager.h"

//シングルトン化
//----------------------------
//instanceの初期化
SceneManager* SceneManager::instance_ = nullptr;

//デフォルトコンストラクタ
SceneManager::SceneManager(void)
{
	sceneID_ = SCENE_ID::NONE;
	waitSceneID_ = SCENE_ID::NONE;
	isSceneChanging_ = false;
	camera_ = nullptr;
	fader_ = nullptr;
	scene_ = nullptr;
}

// デストラクタ
SceneManager::~SceneManager(void)
{

}

//初期化処理
bool SceneManager::Init(void)
{
	//３Ｄの初期設定
	Init3D();

	//変数の初期化
	//---------------------------
	//フェードの設定
	fader_ = new Fader();
	fader_->Init();

	//カメラの設定
	camera_ = new Camera();
	if (camera_->Init() == false)
	{
		OutputDebugString("Camera.cppの初期化失敗");
		return false;
	}

	//シーンの生成処理
	sceneID_ = SCENE_ID::NONE;
	waitSceneID_ = FIRST_SCENE;		//最初のシーン
	DoChangeScene();

	//最初のシーンをフェードインさせる
	fader_->SetFade(Fader::STATE::FADE_IN);
	isSceneChanging_ = true;

	return true;
}

//3Dに関する初期設定
void SceneManager::Init3D(void)
{
	//背景色設定
	SetBackgroundColor(0, 0, 0);

	//Zバッファを有効にする
	SetUseZBuffer3D(true);

	//Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	//バックカリングを有効にする
	SetUseBackCulling(true);

	//ライトを有効化
	SetUseLighting(true);

	//ディレクショナルライトの方向の設定(正規化されていなくてもよい)
	//正面から斜め下に向かったライト
	ChangeLightTypeDir({ 0.00f,-1.00f,1.00f });
}

//更新処理
void SceneManager::Update(void)
{
	//フェードの処理
	//フェードが行われていないなら処理が起きない
	fader_->Update();

	//フェード中
	if (isSceneChanging_ == true)
	{
		Fade();
	}
	//フェード終了
	else
	{
		//各シーンの更新処理
		scene_->Update();

		//カメラの更新処理
		camera_->Update();
	}
}

//描画処理
void SceneManager::Draw(void)
{
	//カメラ設定
	camera_->SetBeforeDraw();

	//カメラの描画
	camera_->Draw();

	//Dxライブラリのカメラとエフェクシアのカメラを同期する
	Effekseer_Sync3DSetting();

	//順番厳守!!カメラ処理の後!!
	//Effekseerにより再生中のエフェクトを更新
	UpdateEffekseer3D();

	//各シーンの描画処理
	scene_->Draw();

	//エフェクトの描画
	DrawEffekseer3D();

	//フェード(必ず最後)
	fader_->Draw();
}

//シーン切り替えを依頼する関数
//isToFadeがtrueならばフェードアウト
void SceneManager::ChangeScene(const SCENE_ID nextID, const bool isToFade)
{
	//フェードの処理が終わってからもシーンを変える必要があるため
	//遷移先シーンをメンバ変数に保持する
	waitSceneID_ = nextID;

	if (isToFade)
	{
		//フェードを実施し、シーン遷移をする
		fader_->SetFade(Fader::STATE::FADE_OUT);
		isSceneChanging_ = true;
	}
	else
	{
		//フェードを実施せず、シーン遷移をする
		DoChangeScene();
	}
}

//シーン切り替える
void SceneManager::DoChangeScene(void)
{
	//リソースマネージャ取得
	ResourceManager& res = ResourceManager::GetInstance();

	//現在のシーン()を解放する
	ReleaseScene();

	//次に移動するシーン(waitSceneID_)を生成する
	//---------------------------------------------------
	//現在のシーンを次のシーンに入れ替える
	sceneID_ = waitSceneID_;

	//各シーンの初期化処理
	switch (sceneID_)
	{
	case SceneManager::SCENE_ID::TITLE:
		//タイトル用リソース取得
		res.InitTitle();

		//タイトルシーンのインスタンス生成
		scene_ = new SceneTitle();
		break;

	case SceneManager::SCENE_ID::SELECT:
		//セレクトシーン用リソース取得
		res.InitSelect();

		//セレクトシーンのインスタンス生成
		scene_ = new SceneSelect();

		//セレクトシーン用のカメラワーク設定
		camera_->SetCamerawork(SELECT_CAMERA_POS, SELECT_CAMERA_ANGLE);
		break;

	case SceneManager::SCENE_ID::GAME:
		//ゲームシーン用リソース取得
		res.InitGame();

		//ゲームシーンのインスタンス
		scene_ = new SceneGame();

		//ゲームシーン用のカメラワーク設定
		camera_->SetCamerawork(GAME_CAMERA_POS, GAME_CAMERA_ANGLE);
		break;

	case SceneManager::SCENE_ID::RESULT:
		//リザルトシーン用リソース取得
		res.InitResult();

		//リザルトシーンのインスタンス
		scene_ = new SceneResult();

		//リザルトシーン用のカメラワーク設定
		camera_->SetCamerawork(RESULT_CAMERA_POS, RESULT_CAMERA_ANGLE);
		break;

	default:
		break;
	}
	
	//対応シーンの初期化
	scene_->Init();

	//シーンの繊維が終了したので、次のシーンをクリアする
	waitSceneID_ = SCENE_ID::NONE;
}

//フェード実施用関数
void SceneManager::Fade(void)
{
	//現在のフェード設定を取得
	Fader::STATE fState = fader_->GetState();

	//フェード処理
	switch (fState)
	{
	case Fader::STATE::FADE_OUT:
		if (fader_->IsEnd() == true)
		{
			//シーンの切り替え
			DoChangeScene();

			//フェードで明るくする
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;

	case Fader::STATE::FADE_IN:
		if (fader_->IsEnd() == true)
		{
			//フェードの終了処理
			fader_->SetFade(Fader::STATE::NONE);

			//シーンの切り替えの終了
			isSceneChanging_ = false;
		}
		break;

	default:
		break;
	}
}

//指定したシーンの解放用
void SceneManager::ReleaseScene(void)
{
	//各シーンの解放処理
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
		scene_ = nullptr;
	}
}

//シングルトン化
//--------------------------
//外部から静的にインスタンスの生成をする
void SceneManager::CreateInstance(void)
{
	//instance_がnullptrの場合は初期化されたのち生成処理を行う
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}

	instance_->Init();
}

//外部でインスタンスを利用可能とするために
//インスタンスを返す関数を作る(参照型でも、ポインタ型でもよい)
SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

//インスタンスの破棄
void SceneManager::Destroy(void)
{
	//シーンの解放
	ReleaseScene();

	//カメラの解放
	camera_->Release();
	delete camera_;
	camera_ = nullptr;

	//フェードの解放
	delete fader_;
	fader_ = nullptr;

	//インスタンスの削除
	delete instance_;
	instance_ = nullptr;
}