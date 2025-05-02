#include"../Application.h"
#include"../Utility/NodyUtility.h"
#include"..//Common/Easing.h"
#include"../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/SceneManager.h"
#include "SceneResult.h"

bool SceneResult::Init(void)
{
	//サウンドのインスタンス
	sound_ = new SoundManager();

	//サウンドの読み込み
	sound_->SoundInit();
	sound_->BGMLoad(SoundManager::BGM_TYPE::RESULT);

	sound_->SELoad(SoundManager::SE_TYPE::CLICK);
	sound_->SELoad(SoundManager::SE_TYPE::CURTAIN);

	//BGMの再生
	sound_->PlayBGM(SoundManager::BGM_TYPE::RESULT, DX_PLAYTYPE_LOOP);

	//背景画像の読み込み
	bgImage_ = LoadGraph((Application::PATH_IMAGE + "ResultBack.png").c_str());

	//背景の舞台幕画像の読み込み
	bgCurtainImage_ = LoadGraph((Application::PATH_IMAGE + "ResultBackCurtain.png").c_str());
	
	//舞台幕画像
	curtainImage_ = LoadGraph((Application::PATH_IMAGE + "Curtain.png").c_str());
	curtainPos_ = VSub(CURTAIN_INIT_POS, VGet(CURTAIN_SHIFT_POS, 0.0f, 0.0f));

	//モデルの読み込み
	pModel_[static_cast<int>(CommonData::TYPE::P1) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenBlue.mv1").c_str());
	pModel_[static_cast<int>(CommonData::TYPE::P2) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenRed.mv1").c_str());
	pModel_[static_cast<int>(CommonData::TYPE::P3) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenGreen.mv1").c_str());
	pModel_[static_cast<int>(CommonData::TYPE::P4) - 1] = MV1LoadModel((Application::PATH_MODEL + "chickenYellow.mv1").c_str());

	pScl_ = VScale(NodyUtility::VECTOR_ONE, P_SCALE);
	pRot_ = NodyUtility::VECTOR_ZERO;
	pPos_[0] = NodyUtility::VECTOR_ZERO;
	pPos_[0].x -= PL_DIS * 1.5f;

	for (int p = 0; p < static_cast<int>(CommonData::GetInstance().GetPlayerNum()) + static_cast<int>(CommonData::GetInstance().GetCPUNum()); p++)
	{
		//順位取得
		rank_.push_back(CommonData::GetInstance().GetRank(p));
	}

	//人数
	size_t size = CommonData::GetInstance().GetRank().size();

	for (int p = 0; p < size; p++)
	{
		pPos_[p] = VAdd(pPos_[0], { PL_DIS * p });

		//モデルの位置、大きさ、角度
		MV1SetScale(pModel_[p], pScl_);
		MV1SetRotationXYZ(pModel_[p], pRot_);
		MV1SetPosition(pModel_[p], pPos_[p]);

		auto rank = rank_[p];

		switch (rank)
		{
		case 1:
			animAttachNo_[p] = MV1AttachAnim(pModel_[p], static_cast<int>(ANIM_NUM::RANK1ST));
			rankImage_[p] = LoadGraph((Application::PATH_IMAGE + "1stImage.png").c_str());
			break;
	
		case 2:
			animAttachNo_[p] = MV1AttachAnim(pModel_[p], static_cast<int>(ANIM_NUM::RANK2ND));
			rankImage_[p] = LoadGraph((Application::PATH_IMAGE + "2ndImage.png").c_str());
			break;
		
		case 3:
			animAttachNo_[p] = MV1AttachAnim(pModel_[p], static_cast<int>(ANIM_NUM::RANK3RD));
			rankImage_[p] = LoadGraph((Application::PATH_IMAGE + "3rdImage.png").c_str());
			break;
		
		case 4:
			animAttachNo_[p] = MV1AttachAnim(pModel_[p], static_cast<int>(ANIM_NUM::RANK4TH));
			rankImage_[p] = LoadGraph((Application::PATH_IMAGE + "4thImage.png").c_str());
			break;
		}
		
		animTotalTime_[p] = MV1GetAttachAnimTotalTime(pModel_[p], animAttachNo_[p]);
		stepAnim_[p] = 0.0f;
	}

	curtainFlag_ = false;

	return true;
}

void SceneResult::Update(void)
{
	auto ins = InputManager::GetInstance();
	auto& cIns = CommonData::GetInstance();

	//人数
	size_t size = CommonData::GetInstance().GetRank().size();

	//舞台幕を開く
	if (curtainFeedBackTime_ < CURTAIN_FEEDBACK_TIME)
	{
		if (!curtainFlag_)
		{
			//カーテン音
			sound_->PlaySE(SoundManager::SE_TYPE::CURTAIN, DX_PLAYTYPE_BACK, CURTAIN_VOLUME, false);
			curtainFlag_ = true;
		}

		//カウント
		curtainFeedBackTime_++;

		//ずらす
		curtainPos_.x = CubicIn(curtainFeedBackTime_
			, CURTAIN_FEEDBACK_TIME
			, CURTAIN_INIT_POS.x - CURTAIN_SHIFT_POS
			, -Application::SCREEN_SIZE_X / 2);
	}
	//舞台幕が開ききったら
	else
	{
		//シーン遷移処理
		//キーボードのZ　又は　パッドのAが押されたか
		if (ins.IsJoypadKeyPush(DX_INPUT_KEY_PAD1, PAD_INPUT_A))
		{
			sound_->PlaySE(SoundManager::SE_TYPE::CLICK, DX_PLAYTYPE_BACK, CLICK_VOLUME, false);
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
		}
	}

	for (int pl = 0; pl < size; pl++)
	{
		//モデルの位置、大きさ、角度
		MV1SetScale(pModel_[pl], pScl_);
		MV1SetRotationXYZ(pModel_[pl], pRot_);
		MV1SetPosition(pModel_[pl], pPos_[pl]);

		//アニメーション再生
		//-------------------------------------------

		//アニメーション時間の進行
		stepAnim_[pl] += (speedAnim_[pl] * NodyUtility::DELTA_TIME);
		if (stepAnim_[pl] > animTotalTime_[pl])
		{
			//ループ再生
			stepAnim_[pl] = 0.0f;
		}

		//再生するアニメーション時間の設定
		MV1SetAttachAnimTime(pModel_[pl], animAttachNo_[pl], stepAnim_[pl]);

		//アニメーション速度
		speedAnim_[pl] = SPEED_ANIM;
	}
}

void SceneResult::Draw(void)
{
	//人数
	size_t size = CommonData::GetInstance().GetRank().size();

	//背景画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImage_, true);

	//舞台幕の画像
	DrawBillboard3D(curtainPos_, 0.5f, 0.5f, 100.0f, 0.0f, curtainImage_, true);
	DrawBillboard3D(VGet(-curtainPos_.x, curtainPos_.y, curtainPos_.z), 0.5f, 0.5f, 100.0f, 0.0f, curtainImage_, true);

	//背景舞台幕の画像
	DrawBillboard3D(CURTAIN_INIT_POS, 0.5f, 0.5f, 100.0f, 0.0f, bgCurtainImage_, true);

	for (int p = 0; p < size; p++)
	{
		//プレイヤモデル
		MV1DrawModel(pModel_[p]);
		
		//順位
		DrawBillboard3D({ pPos_[p].x,pPos_[p].y - RANK_DIS_Y ,pPos_[p].z - RANK_DIS_Z }, 0.5f, 0.5f, RANK_SIZE, 0.0f, rankImage_[p], true);
	}
}

bool SceneResult::Release(void)
{
	//SEの解放
	sound_->SoundRelease();
	delete sound_;
	sound_ = nullptr;

	//人数
	size_t size = CommonData::GetInstance().GetRank().size();

	for (int p = 0; p < size; p++)
	{
		//モデルの解放
		MV1DeleteModel(pModel_[p]);

		//順位
		DeleteGraph(rankImage_[p]);
	}

	DeleteGraph(bgImage_);
	DeleteGraph(bgCurtainImage_);
	DeleteGraph(curtainImage_);

	return true;
}

