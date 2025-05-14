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
		// �摜�ǂݍ���
		handleId_ = LoadGraph(path_.c_str());
		break;

	case Resource::TYPE::IMGS:
		// �����摜�ǂݍ���
		handleIds_ = new int[numX_ * numY_];
		LoadDivGraph(
			path_.c_str(),
			numX_ * numY_,
			numX_, numY_,
			sizeX_, sizeY_,
			&handleIds_[0]);
		break;

	case Resource::TYPE::MASK:
		//�}�X�N�摜�ǂݍ���
		handleId_ = LoadMask(path_.c_str());
		break;

	case Resource::TYPE::MODEL:
		// ���f���ǂݍ���
		handleId_ = MV1LoadModel(path_.c_str());
		break;

	case Resource::TYPE::EFFEKSEER:
		//�G�t�F�N�g�ǂݍ���
		handleId_ = LoadEffekseerEffect(path_.c_str());
		break;

	case Resource::TYPE::SOUND:
		//�T�E���h�ǂݍ���
		handleId_ = LoadSoundMem(path_.c_str());
		break;
	}

}

void Resource::Release(void)
{

	switch (resType_)
	{
	case Resource::TYPE::IMG:
		//�摜�폜
		DeleteGraph(handleId_);
		break;

	case Resource::TYPE::IMGS:
	{
		//�����摜�폜
		int num = numX_ * numY_;
		for (int i = 0; i < num; i++)
		{
			DeleteGraph(handleIds_[i]);
		}
		delete[] handleIds_;
	}
		break;

	case Resource::TYPE::MASK:
		//�}�X�N�폜
		DeleteMask(handleId_);
		break;

	case Resource::TYPE::MODEL:
	{
		//���f���폜
		MV1DeleteModel(handleId_);
		auto ids = duplicateModelIds_;
		for (auto id : ids)
		{
			MV1DeleteModel(id);
		}
	}
		break;

	case Resource::TYPE::EFFEKSEER:
		//�G�t�F�N�g�폜
		DeleteEffekseerEffect(handleId_);
		break;

	case Resource::TYPE::SOUND:
		//�T�E���h�폜
		DeleteSoundMem(handleId_);
		break;
	}

}

void Resource::CopyHandle(int* imgs)
{
	//�n���h�����Ȃ��Ȃ�
	if (handleIds_ == nullptr)
	{
		//�������Ȃ�
		return;
	}

	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		//�n���h���R�s�[
		imgs[i] = handleIds_[i];
	}

}
