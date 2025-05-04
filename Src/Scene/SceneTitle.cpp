#include<DxLib.h>
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Common/CommonData.h"
#include"../Manager/Resource.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/SceneManager.h"
#include"../Object/Grid.h"
#include"SceneTitle.h"

 //デストラクタ
SceneTitle::~SceneTitle(void)
{

}

//初期化処理
bool SceneTitle::Init(void)
{
	//インスタンス取得
	ResourceManager& res = ResourceManager::GetInstance();
	CommonData& common = CommonData::GetInstance();

	//サウンドの初期化
	sound_ = new SoundManager();
	sound_->SoundInit();

	//サウンドのロード
	sound_->BGMLoad(SoundManager::BGM_TYPE::TITLE);
	sound_->SELoad(SoundManager::SE_TYPE::CLICK);

	//BGMの再生
	sound_->PlayBGM(SoundManager::BGM_TYPE::TITLE, DX_PLAYTYPE_BACK);

	//順位の配列を削除しておく
	common.DeleteArray();

	//背景のグラフィック登録
	bgImage_ = res.Load(ResourceManager::SRC::TITLE_BACK).handleId_;
	cloudImage_ = res.Load(ResourceManager::SRC::CLOUD).handleId_;

	//タイトルロゴのグラフィック登録
	logoImage_ = res.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

	//雲の位置
	cloudPos_ = { 0.0f,Application::SCREEN_SIZE_Y / 2 };

	return true;
}

//更新処理
void SceneTitle::Update(void)
{
	//インスタンス取得
	InputManager& input = InputManager::GetInstance();

	if (input.IsJoypadKeyPush(DX_INPUT_KEY_PAD1,PAD_INPUT_1) == 1)
	{
		sound_->PlaySE(SoundManager::SE_TYPE::CLICK,DX_PLAYTYPE_BACK, CLICK_VOLUME);

		//次のシーンに遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
	}

	//雲の動き
	cloudPos_.x--;
	if (cloudPos_.x < -Application::SCREEN_SIZE_X)
	{
		//初期位置に戻す
		cloudPos_.x = 0;
	}
}

//描画処理
void SceneTitle::Draw(void)
{
	//フォント
	int font = CreateFontToHandle(NULL, FONT_SIZE, FONT_TICKNESS, DX_FONTTYPE_EDGE);
	unsigned int color;

	//フォントの色
	color = ((static_cast<int>(cloudPos_.x) / FONT_BLINKING_INTERVAL) % 2 == 0) ? Utility::COLOR_WHITE : Utility::COLOR_GRAY;

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

	//文字(パッド接続によって変わる)
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