/*
*	Date : 2022/05/05(木)
*		Author : がむしるぉ
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
	*	初期化処理
	*/
	static bool Init();

	/*
	* @ brief \
	*	終了処理
	*/
	static void Uninit();
	
	/*
	* @ brief \
	*	更新処理
	*/
	static void Update();

public:
	class Button
	{
	public:
		/*
		* @ brief \
		*	ボタンが押されている
		* @ param \
		*	padNum	: 使用するコントローラー番号
		*	type	: ボタンタイプ
		* @ comment \
		*	type は XINPUT_GAMEPAD_Xのような指定方法と
		*		 PadButton::Xのような指定の仕方があります
		*/
		static bool Press(int padNum, DWORD type);
		
		/*
		* @ brief \
		*	ボタンが押された
		* @ param \
		*	padNum	: 使用するコントローラー番号
		*	type	: ボタンタイプ
		* @ comment \
		*	type は XINPUT_GAMEPAD_Xのような指定方法と
		*		 PadButton::Xのような指定の仕方があります
		*/
		static bool Trigger(int padNum, DWORD type);
		
		/*
		* @ brief \
		*	ボタンが離された
		* @ param \
		*	padNum	: 使用するコントローラー番号
		*	type	: ボタンタイプ
		* @ comment \
		*	type は XINPUT_GAMEPAD_Xのような指定方法と
		*		 PadButton::Xのような指定の仕方があります
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
			*	左スティックの正規化した入力値を取得する
			*/
			static std::array<float, 2> GetVal(int padNum);

			/*
			* @ brief \
			*	左スティックの入力取得する(PRESS)
			* @ param \
			*	padNum	: コントローラーの番号
			*	dir		: 方向
			* @ comment \
			*	dirには0~3までの値を指定する
			*	または PadStick::UPのように指定する
			*/
			static bool Press(int padNum, int dir);

			/*
			* @ brief \
			*	左スティックの入力取得する(TRIGGER)
			* @ param \
			*	padNum	: コントローラーの番号
			*	dir		: 方向
			* @ comment \
			*	dirには0~3までの値を指定する
			*	または PadStick::UPのように指定する	
			*/
			static bool Trigger(int padNum, int dir);

			/*
			* @ brief \
			*	左スティックの入力取得する(Release)
			* @ param \
			*	padNum	: コントローラーの番号
			*	dir		: 方向
			* @ comment \
			*	dirには0~3までの値を指定する
			*	または PadStick::UPのように指定する
			*/
			static bool Release(int padNum, int dir);

		};

		class Right
		{
		public:
			/*
			* @ brief \
			*	左スティックの正規化した入力値を取得する
			* @ return \
			*	正規化された入力データ
			*/
			static std::array<float, 2> GetVal(int padNum);

			/*
			* @ brief \
			*	左スティックの入力取得する(PRESS)
			* @ param \
			*	padNum	: コントローラーの番号
			*	dir		: 方向
			* @ comment \
			*	dirには0~3までの値を指定する
			*	または PadStick::UPのように指定する
			*/
			static bool Press(int padNum, int dir);

			/*
			* @ brief \
			*	左スティックの入力取得する(TRIGGER)
			* @ param \
			*	padNum	: コントローラーの番号
			*	dir		: 方向
			* @ comment \
			*	dirには0~3までの値を指定する
			*	または PadStick::UPのように指定する
			*/
			static bool Trigger(int padNum, int dir);

			/*
			* @ brief \
			*	左スティックの入力取得する(Release)
			* @ param \
			*	padNum	: コントローラーの番号
			*	dir		: 方向
			* @ comment \
			*	dirには0~3までの値を指定する
			*	または PadStick::UPのように指定する
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
		*	バイブレーションの設定
		* @ param \
		*	padNum	: コントローラー番号
		*	valL	: 左モータの深度(最小 : 0, 最大 : 65535)
		* 	valR	: 右モータの深度(最小 : 0, 最大 : 65535)
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
			*	左トリガーの入力値を取得する
			* @ param \
			*	padNum : コントローラー番号
			*/
			static float GetVal(int padNum);

			/*
			* @ brief \
			*	左トリガーの入力を取得する(PRESS)
			* @ param \
			*	padNum : コントローラー番号
			*/
			static bool Press(int padNum);

			/*
			* @ brief \
			*	左トリガーの入力を取得する(TRIGGER)
			* @ param \
			*	padNum : コントローラー番号
			*/
			static bool Trigger(int padNum);

			/*
			* @ brief \
			*	左トリガーの入力を取得する(RELEASE)
			* @ param \
			*	padNum : コントローラー番号
			*/
			static bool Release(int padNum);
		};

		class Right
		{
		public:
			/*
			* @ brief \
			*	トリガーの入力値を取得する
			* @ param \
			*	padNum : コントローラー番号
			*/
			static float GetVal(int padNum);

			/*
			* @ brief \
			*	右トリガーの入力を取得する(PRESS)
			* @ param \
			*	padNum : コントローラー番号
			*/
			static bool Press(int padNum);

			/*
			* @ brief \
			*	右トリガーの入力を取得する(TRIGGER)
			* @ param \
			*	padNum : コントローラー番号
			*/
			static bool Trigger(int padNum);

			/*
			* @ brief \
			*	右トリガーの入力を取得する(RELEASE)
			* @ param \
			*	padNum : コントローラー番号
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

