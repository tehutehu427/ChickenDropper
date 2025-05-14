#pragma once
#include <string>
#include <vector>

class Resource
{

public:
	
	// ���\�[�X�^�C�v
	enum class TYPE
	{
		NONE,			
		IMG,			//�摜
		IMGS,			//�����摜
		MASK,			//�}�X�N�摜
		MODEL,			//���f��
		EFFEKSEER,		//�G�t�F�N�g
		SOUND			//�T�E���h
	};

	// �R���X�g���N�^
	Resource(void);
	// �R���X�g���N�^
	Resource(TYPE type, const std::string& path);
	// �R���X�g���N�^(IMGS�p)
	Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY);

	// �f�X�g���N�^
	~Resource(void);

	// �ǂݍ���
	void Load(void);

	// ���
	void Release(void);

	// �����摜�n���h����ʔz��ɃR�s�[
	void CopyHandle(int* imgs);

	// ���\�[�X�^�C�v
	TYPE resType_;

	// ���\�[�X�̓ǂݍ��ݐ�
	std::string path_;

	// �摜�ƃ��f���̃n���h��ID
	int handleId_;

	// IMGS::LoadDivGraph�p
	int* handleIds_;	//�n���h���̐擪�A�h���X
	int numX_;			//��������
	int numY_;			//�c������
	int sizeX_;			//�摜��̉��̑傫��
	int sizeY_;			//�摜��̏c�̑傫��

	// ���f�������p
	std::vector<int> duplicateModelIds_;

};

