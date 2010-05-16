#include "Process.h"
#include <stdio.h>
#pragma comment(lib,"Advapi32.lib")
#pragma comment(lib,"Kernel32.lib")

void printError( WCHAR * msg )
{
  DWORD eNum;
  WCHAR sysMsg[256];
  WCHAR* p;

  eNum = GetLastError( );
  FormatMessageW(
         FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default lang.
         sysMsg, 256, NULL );

  // Trim the end of the line and terminate it with a null
  p = sysMsg;
  while( ( *p > 31 ) || ( *p == 9 ) )
    ++p;
  do { *p-- = 0; } while( ( p >= sysMsg ) &&
                          ( ( *p == '.' ) || ( *p < 33 ) ) );

  // Display the message
  wprintf( L"\n  WARNING: %s failed with error %d (%s)",
    msg, eNum, sysMsg );
}

bool Process::EnableDebugPrivilege(bool bEnable)
{
	bool fOK = false;
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) //打开进程访问令牌
	{
		//试图修改“调试”特权
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		fOK = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken); 
	} 
	return fOK;
}
HANDLE Process::GetProcessById(unsigned long pid)
{
	EnableDebugPrivilege(true);
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	return handle;
}

bool Process::GetProcessList(unsigned long  pid[],int length,int *need)
{
  HANDLE hProcessSnap;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;
  DWORD dwPriorityClass;

  // Take a snapshot of all processes in the system.
  hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if( hProcessSnap == INVALID_HANDLE_VALUE )
  {
    printError( L"CreateToolhelp32Snapshot (of processes)" );
    return( false );
  }

  // Set the size of the structure before using it.
  pe32.dwSize = sizeof( PROCESSENTRY32 );

  // Retrieve information about the first process,
  // and exit if unsuccessful
  if( !Process32First( hProcessSnap, &pe32 ) )
  {
    printError( L"Process32First" ); // Show cause of failure
    CloseHandle( hProcessSnap );    // Must clean up the
                                    //   snapshot object!
    return( false );
  }

  // Now walk the snapshot of processes, and
  // display information about each process in turn
  int count = 0;
  do
  {
    printf( "\n\n"
      "=====================================================" );
    printf( "\nPROCESS NAME:  %s", pe32.szExeFile );
    printf( "\n"
      "-----------------------------------------------------" );

    // Retrieve the priority class.
    dwPriorityClass = 0;
    hProcess = OpenProcess(
      PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
    if( hProcess == NULL )
      printError( L"OpenProcess" );
    else
    {
      dwPriorityClass = GetPriorityClass( hProcess );
      if( !dwPriorityClass )
        printError( L"GetPriorityClass" );
      CloseHandle( hProcess );
    }
	if(count < length)
	{
		pid[count] = pe32.th32ProcessID;
	}
	
    printf( "\n  process ID        = 0x%08X", pe32.th32ProcessID );
    printf( "\n  thread count      = %d",   pe32.cntThreads );
    printf( "\n  parent process ID = 0x%08X",
      pe32.th32ParentProcessID );
    printf( "\n  Priority Base     = %d", pe32.pcPriClassBase );
    if( dwPriorityClass )
      printf( "\n  Priority Class    = %d", dwPriorityClass );
	count ++;
  } while( Process32Next( hProcessSnap, &pe32 ) );
  *need = count;
  CloseHandle( hProcessSnap );
  return( true );
}

bool Process::ListProcessModules(MODULEENTRY32 modules[],int length,int *need)
{
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
  MODULEENTRY32 me32;
  int count = 0;
  // Take a snapshot of all modules in the specified process.
  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, m_pid );
  if( hModuleSnap == INVALID_HANDLE_VALUE )
  {
    printError( L"CreateToolhelp32Snapshot (of modules)" );
    return( false );
  }

  // Set the size of the structure before using it.
  me32.dwSize = sizeof( MODULEENTRY32 );

  // Retrieve information about the first module,
  // and exit if unsuccessful
  if( !Module32First( hModuleSnap, &me32 ) )
  {
    printError( L"Module32First" ); // Show cause of failure
    CloseHandle( hModuleSnap );    // Must clean up the
                                   //   snapshot object!
    return( false );
  }

  // Now walk the module list of the process,
  // and display information about each module
  do
  {
    printf( "\n\n     MODULE NAME:     %s",
      me32.szModule );
    printf( "\n     executable     = %s",
      me32.szExePath );
    printf( "\n     process ID     = 0x%08X",
      me32.th32ProcessID );
    printf( "\n     ref count (g)  =     0x%04X",
      me32.GlblcntUsage );
    printf( "\n     ref count (p)  =     0x%04X",
      me32.ProccntUsage );
    printf( "\n     base address   = 0x%08X",
      (DWORD) me32.modBaseAddr );
    printf( "\n     base size      = %d",
      me32.modBaseSize );
	if(count < length)
	{
		memcpy(&(modules[count]),&me32,sizeof(MODULEENTRY32));
	}
	count ++;

  } while( Module32Next( hModuleSnap, &me32 ) );
  *need = count;
  CloseHandle( hModuleSnap );
  return( true );
}

bool Process::ListProcessThreads(THREADENTRY32 threads[],int length,int *need) 
{ 
  HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
  THREADENTRY32 te32; 
  int count = 0;
  // Take a snapshot of all running threads  
  hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
  if( hThreadSnap == INVALID_HANDLE_VALUE ) 
    return( false ); 
 
  // Fill in the size of the structure before using it. 
  te32.dwSize = sizeof(THREADENTRY32 ); 
 
  // Retrieve information about the first thread,
  // and exit if unsuccessful
  if( !Thread32First( hThreadSnap, &te32 ) ) 
  {
    printError( L"Thread32First" ); // Show cause of failure
    CloseHandle( hThreadSnap );    // Must clean up the
                                   //   snapshot object!
    return( false );
  }

  // Now walk the thread list of the system,
  // and display information about each thread
  // associated with the specified process
  do 
  { 
    if( te32.th32OwnerProcessID == m_pid )
    {
      printf( "\n\n     THREAD ID      = 0x%08X",
        te32.th32ThreadID ); 
      printf( "\n     base priority  = %d", te32.tpBasePri ); 
      printf( "\n     delta priority = %d", te32.tpDeltaPri );
	  if(count < length)
	  {
		memcpy(&(threads[count]),&te32,sizeof(THREADENTRY32));
	  }
	  count ++;
    }
  } while( Thread32Next(hThreadSnap, &te32 ) ); 
  *need = count;
  CloseHandle( hThreadSnap );
  return( true );
}
