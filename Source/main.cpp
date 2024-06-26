/*===================================================================================
*    Date : 2022/11/11(金)
*        Author		: Gakuto.S
*        File		: main.cpp
*        Detail		: 就職作品プレゼンテーション (抽象化レイヤーを用いた描画処理)
===================================================================================*/
#include "Application.h"

#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif // _DEBUG
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	if (Application::Get()->Init())
		while (Application::Get()->Run());

	Application::Get()->Uninit();

	return 0;
}
