/*===================================================================================
*    Date : 2022/11/11(‹à)
*        Author		: Gakuto.S
*        File		: main.cpp
*        Detail		: AEì•iƒvƒŒƒ[ƒ“ƒe[ƒVƒ‡ƒ“ (’ŠÛ‰»ƒŒƒCƒ„[‚ğ—p‚¢‚½•`‰æˆ—)
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
