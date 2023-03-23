#include <sdk/SexySDK.hpp>
#include <callbacks/callbacks.hpp>
#include <MinHook.h>

static void(__fastcall* Sexy__ThunderballApp_LostFocus_)(Sexy::ThunderballApp*, char*);
void __fastcall Sexy__ThunderballApp_LostFocus(Sexy::ThunderballApp* this_, char* edx)
{
	//return Sexy__ThunderballApp_LostFocus_(this_, edx);
}

void init()
{
	MH_Initialize();

	MH_CreateHook((void*)0x00408470, Sexy__ThunderballApp_LostFocus, (void**)&Sexy__ThunderballApp_LostFocus_);

	MH_EnableHook(MH_ALL_HOOKS);
}

DWORD WINAPI OnAttachImpl(LPVOID lpParameter)
{
	init();
	return 0;
}

DWORD WINAPI OnAttach(LPVOID lpParameter)
{
	__try
	{
		return OnAttachImpl(lpParameter);
	}
	__except (0)
	{
		FreeLibraryAndExitThread((HMODULE)lpParameter, 0xDECEA5ED);
	}

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, OnAttach, hModule, 0, nullptr);
		return true;
	}

	return false;
}
