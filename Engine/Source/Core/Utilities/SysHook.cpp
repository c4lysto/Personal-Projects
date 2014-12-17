#include "SysHook.h"
#include "Debugging\DebugHelp.h"

SysHook::SysHook() : m_pHook(nullptr)
{

}

SysHook::~SysHook()
{
	Shutdown();
}

void SysHook::Init(eHookType hookType, SysHookCallback hookCallback)
{
	if(hookCallback)
	{
		void* pModule = nullptr;

		if(Verify(GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN, nullptr, (HMODULE*)&pModule), "SysHook - Failed To Get Module Handle"))
		{
			m_pHook = SetWindowsHookEx(hookType, ((HOOKPROC)hookCallback), (HMODULE)nullptr/*pModule*/, GetCurrentThreadId());
			Assert(m_pHook, "SysHook - Failed To Create Hook!");
		}
	}
}

void SysHook::Shutdown()
{
	if(m_pHook)
	{
		Assert(UnhookWindowsHookEx((HHOOK)m_pHook), "SysHook - Failed To Unhook Windows Hook!\n Error Code: %d", GetLastError());
		m_pHook = nullptr;
	}
}