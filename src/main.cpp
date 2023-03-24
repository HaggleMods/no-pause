#include <sdk/SexySDK.hpp>
#include <callbacks/callbacks.hpp>
#include <MinHook.h>
#include <filesystem>
#include <ini_rw.h>

bool keep_audio = true;

static void(__fastcall* Sexy__ThunderballApp__LostFocus_)(Sexy::ThunderballApp*, char*);
void __fastcall Sexy__ThunderballApp__LostFocus(Sexy::ThunderballApp* this_, char* edx)
{
	//Basically just do nothing
	//Sexy__ThunderballApp_LostFocus_(this_, edx);
}

bool get_boolean(const char* bool_text)
{
	return !std::strcmp(bool_text, "true");
}

void load_ini()
{
	ini_t* ini;
	auto ini_path = std::filesystem::current_path() / std::format("mods/{}.ini", MOD_NAME);
	if (!std::filesystem::exists(ini_path))
	{
		const char* ini_default = ""
			"[settings]\n"
			"keep-audio = true";

		ini = ini_create(ini_default, strlen(ini_default));
		ini_save(ini, ini_path.string().c_str());
	}
	else
	{
		ini = ini_load(ini_path.string().c_str());
	}

	keep_audio = get_boolean(ini_get(ini, "settings", "keep-audio"));
}

void init()
{
	//load_ini();

	MH_Initialize();

	MH_CreateHook((void*)0x00408470, Sexy__ThunderballApp__LostFocus, (void**)&Sexy__ThunderballApp__LostFocus_);

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
