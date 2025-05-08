#include"../Application.h"
#include"../Utility/Utility.h"
#include"..//Common/Easing.h"
#include"../Manager/Resource.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/SceneManager.h"
#include "SceneResult.h"

bool SceneResult::Init(void)
{
	//インスタンス取得
	ResourceManager& res = ResourceManager::GetInstance();

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
	bgImage_ = res.Load(ResourceManager::SRC::RESULT_BACK).handleId_;

	//背景の舞台幕画像の読み込み
	bgCurtainImage_ = res.Load(ResourceManager::SRC::CURTAIN_BACK).handleId_;
	
	//舞台幕画像
	curtainImage_ = res.Load(ResourceManager::SRC::CURTAIN).handleId_;
	curtainPos_ = VSub(CURTAIN_INIT_POS, VGet(CURTAIN_SHIFT_POS, 0.0f, 0.0f));

	//モデルの読み込み
	pModel_[static_cast<int>(CommonData::TYPE::P1) - 1] = res.LoadModelDuplicate(ResourceManager::SRC::CHICKEN_BLUE);
	pModel_[static_cast<int>(CommonData::TYPE::P2) - 1] = res.LoadModelDuplicate(ResourceManager::SRC::CHICKEN_RED);
	pModel_[static_cast<int>(CommonData::TYPE::P3) - 1] = res.LoadModelDuplicate(ResourceManager::SRC::CHICKEN_GREEN);
	pModel_[static_cast<int>(CommonData::TYPE::P4) - 1] = res.LoadModelDuplicate(ResourceManager::SRC::CHICKEN_YELLOW);

	pScl_ = VScale(Utility::VECTOR_ONE, P_SCALE);
	pRot_ = Utility::VECTOR_ZERO;
	pPos_[0] = Utility::VECTOR_ZERO;
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
			rankImage_[p] = res.Load(ResourceManager::SRC::RANK_1ST).handleId_;
			break;
	
		case 2:
			animAttachNo_[p] = MV1AttachAnim(pModel_[p], static_cast<int>(ANIM_NUM::RANK2ND));
			rankImage_[p] = res.Load(ResourceManager::SRC::RANK_2ND).handleId_;
			break;
		
		case 3:
			animAttachNo_[p] = MV1AttachAnim(pModel_[p], static_cast<int>(ANIM_NUM::RANK3RD));
			rankImage_[p] = res.Load(ResourceManager::SRC::RANK_3RD).handleId_;
			break;
		
		case 4:
			animAttachNo_[p] = MV1AttachAnim(pModel_[p], static_cast<int>(ANIM_NUM::RANK4TH));
			rankImage_[p] = res.Load(ResourceManager::SRC::RANK_4TH).handleId_;
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
	auto& ins = InputManager::GetInstance();
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
		stepAnim_[pl] += (speedAnim_[pl] * Utility::DELTA_TIME);
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
	DrawBillboard3D(curtainPos_, BILLBOURD_POS, BILLBOURD_POS, CURTAIN_SIZE, 0.0f, curtainImage_, true);
	DrawBillboard3D(VGet(-curtainPos_.x, curtainPos_.y, curtainPos_.z), BILLBOURD_POS, BILLBOURD_POS, CURTAIN_SIZE, 0.0f, curtainImage_, true);

	//背景舞台幕の画像
	DrawBillboard3D(CURTAIN_INIT_POS, BILLBOURD_POS, BILLBOURD_POS, CURTAIN_SIZE, 0.0f, bgCurtainImage_, true);

	for (int p = 0; p < size; p++)
	{
		//プレイヤモデル
		MV1DrawModel(pModel_[p]);
		
		//順位
		DrawBillboard3D({ pPos_[p].x,pPos_[p].y - RANK_DIS_Y ,pPos_[p].z - RANK_DIS_Z }, BILLBOURD_POS, BILLBOURD_POS, RANK_SIZE, 0.0f, rankImage_[p], true);
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

