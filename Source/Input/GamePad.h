/*
*	Date : 2022/05/05(��)
*		Author : ���ނ��邧
*		Update : ----/--/--()
*/
#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <array>

#pragma warning(disable : 4995)
#pragma comment(lib, "xinput.lib")

#define GAMEPAD_MAX 4

#define DEAD_ZONE 100

namespace PadButton
{
	static DWORD X			= XINPUT_GAMEPAD_X;
	static DWORD Y			= XINPUT_GAMEPAD_Y;
	static DWORD A			= XINPUT_GAMEPAD_A;
	static DWORD B			= XINPUT_GAMEPAD_B;
	static DWORD UP			= XINPUT_GAMEPAD_DPAD_UP;
	static DWORD LEFT		= XINPUT_GAMEPAD_DPAD_LEFT;
	static DWORD DOWN		= XINPUT_GAMEPAD_DPAD_DOWN;
	static DWORD RIGHT		= XINPUT_GAMEPAD_DPAD_RIGHT;
	static DWORD START		= XINPUT_GAMEPAD_START;
	static DWORD BACK		= XINPUT_GAMEPAD_BACK;
	static DWORD LTHUMB		= XINPUT_GAMEPAD_LEFT_THUMB;
	static DWORD RTHUMB		= XINPUT_GAMEPAD_RIGHT_THUMB;
	static DWORD LSHOULDER	= XINPUT_GAMEPAD_LEFT_SHOULDER;
	static DWORD RSHOULDER	= XINPUT_GAMEPAD_RIGHT_SHOULDER;
}

namespace PadStick
{
	static int UP	 = 0;
	static int DOWN  = 1;
	static int LEFT  = 2;
	static int RIGHT = 3;
}


class GamePad
{
public:
	struct STATE
	{
		XINPUT_STATE		current;
		XINPUT_STATE		last;
		XINPUT_VIBRATION	vibration;
	};

public:
	/*
	* @ brief \
	*	����������
	*/
	static bool Init();

	/*
	* @ brief \
	*	�I������
	*/
	static void Uninit();
	
	/*
	* @ brief \
	*	�X�V����
	*/
	static void Update();

public:
	class Button
	{
	public:
		/*
		* @ brief \
		*	�{�^����������Ă���
		* @ param \
		*	padNum	: �g�p����R���g���[���[�ԍ�
		*	type	: �{�^���^�C�v
		* @ comment \
		*	type �� XINPUT_GAMEPAD_X�̂悤�Ȏw����@��
		*		 PadButton::X�̂悤�Ȏw��̎d��������܂�
		*/
		static bool Press(int padNum, DWORD type);
		
		/*
		* @ brief \
		*	�{�^���������ꂽ
		* @ param \
		*	padNum	: �g�p����R���g���[���[�ԍ�
		*	type	: �{�^���^�C�v
		* @ comment \
		*	type �� XINPUT_GAMEPAD_X�̂悤�Ȏw����@��
		*		 PadButton::X�̂悤�Ȏw��̎d��������܂�
		*/
		static bool Trigger(int padNum, DWORD type);
		
		/*
		* @ brief \
		*	�{�^���������ꂽ
		* @ param \
		*	padNum	: �g�p����R���g���[���[�ԍ�
		*	type	: �{�^���^�C�v
		* @ comment \
		*	type �� XINPUT_GAMEPAD_X�̂悤�Ȏw����@��
		*		 PadButton::X�̂悤�Ȏw��̎d��������܂�
		*/
		static bool Release(int padNum, DWORD type);
	};

public:
	class Stick
	{
	public:
		class Left
		{
		public:
			/*
			* @ brief \
			*	���X�e�B�b�N�̐��K���������͒l���擾����
			*/
			static std::array<float, 2> GetVal(int padNum);

			/*
			* @ brief \
			*	���X�e�B�b�N�̓��͎擾����(PRESS)
			* @ param \
			*	padNum	: �R���g���[���[�̔ԍ�
			*	dir		: ����
			* @ comment \
			*	dir�ɂ�0~3�܂ł̒l���w�肷��
			*	�܂��� PadStick::UP�̂悤�Ɏw�肷��
			*/
			static bool Press(int padNum, int dir);

			/*
			* @ brief \
			*	���X�e�B�b�N�̓��͎擾����(TRIGGER)
			* @ param \
			*	padNum	: �R���g���[���[�̔ԍ�
			*	dir		: ����
			* @ comment \
			*	dir�ɂ�0~3�܂ł̒l���w�肷��
			*	�܂��� PadStick::UP�̂悤�Ɏw�肷��	
			*/
			static bool Trigger(int padNum, int dir);

			/*
			* @ brief \
			*	���X�e�B�b�N�̓��͎擾����(Release)
			* @ param \
			*	padNum	: �R���g���[���[�̔ԍ�
			*	dir		: ����
			* @ comment \
			*	dir�ɂ�0~3�܂ł̒l���w�肷��
			*	�܂��� PadStick::UP�̂悤�Ɏw�肷��
			*/
			static bool Release(int padNum, int dir);

		};

		class Right
		{
		public:
			/*
			* @ brief \
			*	���X�e�B�b�N�̐��K���������͒l���擾����
			* @ return \
			*	���K�����ꂽ���̓f�[�^
			*/
			static std::array<float, 2> GetVal(int padNum);

			/*
			* @ brief \
			*	���X�e�B�b�N�̓��͎擾����(PRESS)
			* @ param \
			*	padNum	: �R���g���[���[�̔ԍ�
			*	dir		: ����
			* @ comment \
			*	dir�ɂ�0~3�܂ł̒l���w�肷��
			*	�܂��� PadStick::UP�̂悤�Ɏw�肷��
			*/
			static bool Press(int padNum, int dir);

			/*
			* @ brief \
			*	���X�e�B�b�N�̓��͎擾����(TRIGGER)
			* @ param \
			*	padNum	: �R���g���[���[�̔ԍ�
			*	dir		: ����
			* @ comment \
			*	dir�ɂ�0~3�܂ł̒l���w�肷��
			*	�܂��� PadStick::UP�̂悤�Ɏw�肷��
			*/
			static bool Trigger(int padNum, int dir);

			/*
			* @ brief \
			*	���X�e�B�b�N�̓��͎擾����(Release)
			* @ param \
			*	padNum	: �R���g���[���[�̔ԍ�
			*	dir		: ����
			* @ comment \
			*	dir�ɂ�0~3�܂ł̒l���w�肷��
			*	�܂��� PadStick::UP�̂悤�Ɏw�肷��
			*/
			static bool Release(int padNum, int dir);
		};

	private:
		static std::array<float, 2> GetStick(const SHORT& stick_x, const SHORT& stick_y);
		static bool Press(const SHORT& current, int dir);
		static bool Trigger(const SHORT& current, const SHORT& old, int dir);
		static bool Release(const SHORT& current, const SHORT& old, int dir);
	};

	class Vibration
	{
	public:
		/*
		* @ brief \
		*	�o�C�u���[�V�����̐ݒ�
		* @ param \
		*	padNum	: �R���g���[���[�ԍ�
		*	valL	: �����[�^�̐[�x(�ŏ� : 0, �ő� : 65535)
		* 	valR	: �E���[�^�̐[�x(�ŏ� : 0, �ő� : 65535)
		*/
		static void Set(int padNum, WORD valL = 0, WORD valR = 0);
	};

	class PadTrigger
	{
	public:
		class Left
		{
		public:
			/*
			* @ brief \
			*	���g���K�[�̓��͒l���擾����
			* @ param \
			*	padNum : �R���g���[���[�ԍ�
			*/
			static float GetVal(int padNum);

			/*
			* @ brief \
			*	���g���K�[�̓��͂��擾����(PRESS)
			* @ param \
			*	padNum : �R���g���[���[�ԍ�
			*/
			static bool Press(int padNum);

			/*
			* @ brief \
			*	���g���K�[�̓��͂��擾����(TRIGGER)
			* @ param \
			*	padNum : �R���g���[���[�ԍ�
			*/
			static bool Trigger(int padNum);

			/*
			* @ brief \
			*	���g���K�[�̓��͂��擾����(RELEASE)
			* @ param \
			*	padNum : �R���g���[���[�ԍ�
			*/
			static bool Release(int padNum);
		};

		class Right
		{
		public:
			/*
			* @ brief \
			*	�g���K�[�̓��͒l���擾����
			* @ param \
			*	padNum : �R���g���[���[�ԍ�
			*/
			static float GetVal(int padNum);

			/*
			* @ brief \
			*	�E�g���K�[�̓��͂��擾����(PRESS)
			* @ param \
			*	padNum : �R���g���[���[�ԍ�
			*/
			static bool Press(int padNum);

			/*
			* @ brief \
			*	�E�g���K�[�̓��͂��擾����(TRIGGER)
			* @ param \
			*	padNum : �R���g���[���[�ԍ�
			*/
			static bool Trigger(int padNum);

			/*
			* @ brief \
			*	�E�g���K�[�̓��͂��擾����(RELEASE)
			* @ param \
			*	padNum : �R���g���[���[�ԍ�
			*/
			static bool Release(int padNum);
		};

	private:
		static float GetVal(const BYTE& current);
		static bool Press(const BYTE& current);
		static bool Trigger(const BYTE& current, const BYTE& old);
		static bool Release(const BYTE& current, const BYTE& old);
	};


private:
	static STATE m_State[GAMEPAD_MAX];
};

