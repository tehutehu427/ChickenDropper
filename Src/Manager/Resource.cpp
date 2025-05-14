#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Resource.h"

Resource::Resource(void)
{
	resType_ = TYPE::NONE;
	path_ = "";

	numX_ = -1;
	numY_ = -1;
	sizeX_ = -1;
	sizeY_ = -1;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::Resource(TYPE type, const std::string& path)
{
	resType_ = type;
	path_ = path;

	numX_ = -1;
	numY_ = -1;
	sizeX_ = -1;
	sizeY_ = -1;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY)
{
	resType_ = type;
	path_ = path;
	numX_ = numX;
	numY_ = numY;
	sizeX_ = sizeX;
	sizeY_ = sizeY;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::~Resource(void)
{
}

void Resource::Load(void)
{

	switch (resType_)
	{
	case Resource::TYPE::IMG:
		// 画像読み込み
		handleId_ = LoadGraph(path_.c_str());
		break;

	case Resource::TYPE::IMGS:
		// 分割画像読み込み
		handleIds_ = new int[numX_ * numY_];
		LoadDivGraph(
			path_.c_str(),
			numX_ * numY_,
			numX_, numY_,
			sizeX_, sizeY_,
			&handleIds_[0]);
		break;

	case Resource::TYPE::MASK:
		//マスク画像読み込み
		handleId_ = LoadMask(path_.c_str());
		break;

	case Resource::TYPE::MODEL:
		// モデル読み込み
		handleId_ = MV1LoadModel(path_.c_str());
		break;

	case Resource::TYPE::EFFEKSEER:
		//エフェクト読み込み
		handleId_ = LoadEffekseerEffect(path_.c_str());
		break;

	case Resource::TYPE::SOUND:
		//サウンド読み込み
		handleId_ = LoadSoundMem(path_.c_str());
		break;
	}

}

void Resource::Release(void)
{

	switch (resType_)
	{
	case Resource::TYPE::IMG:
		//画像削除
		DeleteGraph(handleId_);
		break;

	case Resource::TYPE::IMGS:
	{
		//分割画像削除
		int num = numX_ * numY_;
		for (int i = 0; i < num; i++)
		{
			DeleteGraph(handleIds_[i]);
		}
		delete[] handleIds_;
	}
		break;

	case Resource::TYPE::MASK:
		//マスク削除
		DeleteMask(handleId_);
		break;

	case Resource::TYPE::MODEL:
	{
		//モデル削除
		MV1DeleteModel(handleId_);
		auto ids = duplicateModelIds_;
		for (auto id : ids)
		{
			MV1DeleteModel(id);
		}
	}
		break;

	case Resource::TYPE::EFFEKSEER:
		//エフェクト削除
		DeleteEffekseerEffect(handleId_);
		break;

	case Resource::TYPE::SOUND:
		//サウンド削除
		DeleteSoundMem(handleId_);
		break;
	}

}

void Resource::CopyHandle(int* imgs)
{
	//ハンドルがないなら
	if (handleIds_ == nullptr)
	{
		//何もしない
		return;
	}

	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		//ハンドルコピー
		imgs[i] = handleIds_[i];
	}

}
