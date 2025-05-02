#include<DxLib.h>
#include"../Application.h"
#include"../Common/CommonData.h"
#include"../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/SceneManager.h"
#include"../Object/Grid.h"
#include"SceneTitle.h"

//コンストラクタ
//SceneTitle::SceneTitle(void)
//{
//	bgImage_ = 0;
//}

// デストラクタ
//SceneTitle::~SceneTitle(void)
//{
//
//}

//初期化処理
bool SceneTitle::Init(void)
{
	//サウンドの初期化
	sound_ = new SoundManager();
	sound_->SoundInit();

	//サウンドのロード
	sound_->BGMLoad(SoundManager::BGM_TYPE::TITLE);
	sound_->SELoad(SoundManager::SE_TYPE::CLICK);

	//BGMの再生
	sound_->PlayBGM(SoundManager::BGM_TYPE::TITLE, DX_PLAYTYPE_BACK);

	//順位の配列を削除しておく
	CommonData::GetInstance().DeleteArray();

	//背景のグラフィック登録
	bgImage_ = LoadGraph((Application::PATH_IMAGE + "TitleBack.png").c_str());
	cloudImage_ = LoadGraph((Application::PATH_IMAGE + "cloud.png").c_str());

	cloudPos_ = { 0.0f,Application::SCREEN_SIZE_Y / 2 };

	//タイトルロゴのグラフィック登録
	logoImage_ = LoadGraph((Application::PATH_IMAGE + "TitleLogo.png").c_str());

	return true;
}

//更新処理
void SceneTitle::Update(void)
{
	if (InputManager::GetInstance().IsJoypadKeyPush(DX_INPUT_KEY_PAD1,PAD_INPUT_1) == 1)
	{
		sound_->PlaySE(SoundManager::SE_TYPE::CLICK,DX_PLAYTYPE_BACK, CLICK_VOLUME);

		//次のシーンに遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
	}

	//雲の動き
	cloudPos_.x--;
	if (cloudPos_.x < -Application::SCREEN_SIZE_X)
	{
		cloudPos_.x = 0;
	}
}

//描画処理
void SceneTitle::Draw(void)
{
	//フォント
	int fontSize = 45;
	int fontTick = 5;
	auto font = CreateFontToHandle(NULL, fontSize, fontTick, DX_FONTTYPE_EDGE);
	unsigned int color = 0;

	//背景画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImage_, true,false);
	DrawRotaGraph(cloudPos_.x
				, cloudPos_.y
				, 1.0
				, 0.0, cloudImage_, true,false);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X
				, cloudPos_.y
				, 1.0
				, 0.0, cloudImage_, true,false);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X * 2
				, cloudPos_.y
				, 1.0
				, 0.0, cloudImage_, true,false);

	//ロゴ
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 3, 1.0, 0.0, logoImage_, true);

	//フォントの色
	color = ((static_cast<int>(cloudPos_.x) / 20) % 2 == 0) ? 0xffffff : 0xaaaaaa;

	//文字
	DrawFormatStringToHandle(Application::SCREEN_SIZE_X / 2 - CHAR_POS_X
		, Application::SCREEN_SIZE_Y / 2 + CHAR_POS_Y
		, color
		, font
		, GetJoypadNum() >= 1 ? " Aボタンを押してください" : " Zキーを押してください");

	//フォント解放
	DeleteFontToHandle(font);
}

//解放処理
bool SceneTitle::Release(void)
{
	//サウンドの解放
	sound_->SoundRelease();
	delete sound_;
	sound_ = nullptr;

	//タイトル画像の解放
	DeleteGraph(bgImage_);
	DeleteGraph(cloudImage_);
	DeleteGraph(logoImage_);

	return true;
}