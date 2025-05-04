#include<DxLib.h>
#include<cmath>
#include"InputManager.h"

//�V���O���g����
//-----------------------------------
//instance�̏�����
InputManager* InputManager::instance_ = nullptr;

//����������
void InputManager::InitInput(void)
{
	//���͐���̏�����
	for (int i = 0; i < KEY_BUF_LEN; i++)
	{
		currentKeyBuf_[i] = '\0';
		preKeyBuf_[i] = '\0';
	}

	//���͐���̏�����(�R���g���[���[)
	for (int n = 0; n < PAD_NUM; n++)
	{
		currentPadKeyBuf_[n] = 0;
		prePadKeyBuf_[n] = 0;
	
		for (int k = 0; k < PAD_KEY_BUF_LEN; k++)
		{
			pushPadKeyTime_[n][k] = 0;
			pushPadKeyTrg_[n][k] = false;
		}
	}
}

//���䏈��
void InputManager::StepInput(void)
{
	for (int i = 0; i < KEY_BUF_LEN; i++)
	{
		//�O�t���[���̃L�[���ϐ��ɋL�^���Ă���
		preKeyBuf_[i] = currentKeyBuf_[i];
	}

	//���݂̃L�[�����擾
	GetHitKeyStateAll(currentKeyBuf_);
}

//�P���ɉ�����Ă��邩
bool InputManager::IsKeyDown(int _keyCode)
{
	//���t���ŉ�����Ă���(�O�t���͊֌W�Ȃ�)
	if (currentKeyBuf_[_keyCode] == 1)
	{
		return true;
	}

	return false;
}

//�������������ꂽ��
bool InputManager::IsKeyPush(int _keyCode)
{
	//�O�t���ŉ�����Ă��Ȃ��@���@���t���ŉ�����Ă���
	if (preKeyBuf_[_keyCode] == 0 && currentKeyBuf_[_keyCode] == 1)
	{
		return true;
	}

	//������Ă��Ȃ��̂�false
	return false;
}

//�����������Ă��邩
bool InputManager::IsKeyKeep(int _keyCode)
{
	//�O�t���ŉ�����Ă���@���@���t���ŉ�����Ă���
	if (preKeyBuf_[_keyCode] == 1 && currentKeyBuf_[_keyCode] == 1)
	{
		return true;
	}

	//������Ă��Ȃ��̂�false
	return false;
}

//�������������ꂽ��
bool InputManager::IsKeyRelease(int _keyCode)
{
	//�O�t���ŉ�����Ă���@���@���t���ŉ�����Ă��Ȃ�
	if (preKeyBuf_[_keyCode] == 1 && currentKeyBuf_[_keyCode] == 0)
	{
		return true;
	}

	//������Ă��Ȃ��̂�false
	return false;
}

//���䏈��(�R���g���[���[)
void InputManager::StepPadInput(void)
{
	//���ݐڑ�����Ă���R���g���[���[�̐����擾
	int padNumber = GetJoypadNum();

	for (int n = 0; n < PAD_NUM; n++)
	{
		//�O�t���[���̃L�[���ϐ��ɋL�^���Ă���
		prePadKeyBuf_[n] = currentPadKeyBuf_[n];

		//�p�b�h������q�����Ă��Ȃ��Ƃ�
		if (padNumber == 0)
		{
			//�z��O�ɃL�[�{�[�h�̌��݂̃L�[�����擾
			currentPadKeyBuf_[0] = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		}
		else
		{
			//���݂̃L�[�����擾
			currentPadKeyBuf_[n] = GetJoypadInputState(n + 1);
		}
	}

	for (int k = 0; k < PAD_KEY_BUF_LEN; k++)
	{
		for (int n = 0; n < PAD_NUM; n++)
		{
			if (!IsJoypadKeyKeep(n + 1, pow(2,k)))
			{
				pushPadKeyTime_[n][k] = 0;

				pushPadKeyTrg_[n][k] = false;
			}
			else
			{
				pushPadKeyTrg_[n][k] = true;
			}

			if (pushPadKeyTrg_[n][k])
			{
				pushPadKeyTime_[n][k]++;
			}
		}
	}
}

//�P���ɉ�����Ă��邩(�R���g���[���[)
bool InputManager::IsJoypadKeyDown(int _padNum, int _keyCode)
{
	//�R���g���[���[�̔ԍ����킹�p
	int padNumber;

	//�R���g���[���[�ƃL�[�{�[�h�p
	if (_padNum == DX_INPUT_KEY_PAD1)
	{
		//�P�Ԃɍ��킹��
		padNumber = DX_INPUT_PAD1 - 1;
	}
	else
	{
		//�z��p�ɐ��������킹��
		padNumber = _padNum - 1;
	}
	
	//���t���ŉ�����Ă���(�O�t���͊֌W�Ȃ�)
	if ((currentPadKeyBuf_[padNumber] & _keyCode) != 0)
	{
		return true;
	}

	//������Ă��Ȃ��̂�false
	return false;
}

//�������������ꂽ��(�R���g���[���[)
bool InputManager::IsJoypadKeyPush(int _padNum, int _keyCode)
{
	//�R���g���[���[�̔ԍ����킹�p
	int padNumber;

	//�R���g���[���[�ƃL�[�{�[�h�p
	if (_padNum == DX_INPUT_KEY_PAD1)
	{
		//�P�Ԃɍ��킹��
		padNumber = DX_INPUT_PAD1 - 1;
	}
	else
	{
		//�z��p�ɐ��������킹��
		padNumber = _padNum - 1;
	}

	//�O�t���ŉ�����Ă��Ȃ��@���@���t���ŉ�����Ă���
	if ((prePadKeyBuf_[padNumber] & _keyCode) == 0 && (currentPadKeyBuf_[padNumber] & _keyCode) != 0)
	{
		return true;
	}

	//������Ă��Ȃ��̂�false
	return false;
}

//�����������Ă��邩(�R���g���[���[)
bool InputManager::IsJoypadKeyKeep(int _padNum, int _keyCode)
{
	//�R���g���[���[�̔ԍ����킹�p
	int padNumber;

	//�R���g���[���[�ƃL�[�{�[�h�p
	if (_padNum == DX_INPUT_KEY_PAD1)
	{
		//�P�Ԃɍ��킹��
		padNumber = DX_INPUT_PAD1 - 1;
	}
	else
	{
		//�z��p�ɐ��������킹��
		padNumber = _padNum - 1;
	}

	//�O�t���ŉ�����Ă���@���@���t���ŉ�����Ă���
	if ((prePadKeyBuf_[padNumber] & _keyCode) != 0 && (currentPadKeyBuf_[padNumber] & _keyCode) != 0)
	{
		return true;
	}

	//������Ă��Ȃ��̂�false
	return false;
}

//�������������ꂽ��(�R���g���[���[)
bool InputManager::IsJoypadKeyRelease(int _padNum, int _keyCode)
{
	//�R���g���[���[�̔ԍ����킹�p
	int padNumber;

	//�R���g���[���[�ƃL�[�{�[�h�p
	if (_padNum == DX_INPUT_KEY_PAD1)
	{
		//�P�Ԃɍ��킹��
		padNumber = DX_INPUT_PAD1 - 1;
	}
	else
	{
		//�z��p�ɐ��������킹��
		padNumber = _padNum - 1;
	}

	//�O�t���ŉ�����Ă���@���@���t���ŉ�����Ă��Ȃ�
	if ((prePadKeyBuf_[padNumber] & _keyCode) != 0 && (currentPadKeyBuf_[padNumber] & _keyCode) == 0)
	{
		return true;
	}

	//������Ă��Ȃ��̂�false
	return false;
}

InputManager::KEEP_FUNCTION InputManager::HowLongJoypadKeyKeep(int _padNum, int _keyCode, int _keepTime)
{
	//�R���g���[���[�̔ԍ����킹�p
	int padNumber;

	int codeArray = 0;

	int keyCode = _keyCode;

	while (keyCode / 2 != 0)
	{
		codeArray++;

		keyCode /= 2;
	}

	//�R���g���[���[�ƃL�[�{�[�h�p
	if (_padNum == DX_INPUT_KEY_PAD1)
	{
		//�P�Ԃɍ��킹��
		padNumber = DX_INPUT_PAD1 - 1;
	}
	else
	{
		//�z��p�ɐ��������킹��
		padNumber = _padNum - 1;
	}

	if (pushPadKeyTime_[padNumber][codeArray] < 1)
	{
		return KEEP_FUNCTION::NONE;
	}
	else if (pushPadKeyTime_[padNumber][codeArray] >= _keepTime)
	{
		//������
		pushPadKeyTime_[padNumber][codeArray] = 0;

		return KEEP_FUNCTION::KEEP_TRUE;
	}
	else
	{
		//������
		pushPadKeyTime_[padNumber][codeArray] = 0;

		return KEEP_FUNCTION::KEEP_FALSE;
	}
}

//�V���O���g����
//--------------------------
//�O������ÓI�ɃC���X�^���X�̐���������
void InputManager::CreateInstance(void)
{
	//instance_��nullptr�̏ꍇ�͏��������ꂽ�̂������������s��
	if (instance_ == nullptr)
	{
		instance_ = new InputManager();
	}

	instance_->InitInput();
}

//�O���ŃC���X�^���X�𗘗p�\�Ƃ��邽�߂�
//�C���X�^���X��Ԃ��֐������(�Q�ƌ^�ł��A�|�C���^�^�ł��悢)
InputManager& InputManager::GetInstance(void)
{
	return *instance_;
}

//�C���X�^���X�̔j��
void InputManager::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}