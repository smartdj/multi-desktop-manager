////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Author: angelipin
///
///
///
///
///
///  This is class is not tested!
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <tlhelp32.h>
class Process
{
public:
	Process(unsigned long pid)
	{
		m_pid = pid;
	}
	~Process()
	{

	}
private:
	unsigned long m_pid;

public:
	HANDLE GetProcessById(unsigned long pid);
	bool EnableDebugPrivilege(bool bEnable);
	bool GetProcessList(unsigned long pid[],int length,int *need);
	bool ListProcessModules(MODULEENTRY32 modules[],int length,int *need);
	bool ListProcessThreads(THREADENTRY32 threads[],int length,int *need);
public:
	unsigned long GetPid(){ return m_pid ;}
};