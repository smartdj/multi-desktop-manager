//WindowMonitor.cpp
//--------------------------------------------------------------------
// File include
//--------------------------------------------------------------------
#include "WindowMonitor.h"
#include "XMLParser.h"
#include "CommonWindows.h"
#include <cassert>
#include <vector>
CRITICAL_SECTION CriticalSection;
CRITICAL_SECTION cs;
WindowMonitor* g_pThis = 0;
//--------------------------------------------------------------------
// Constructor and destructor
//--------------------------------------------------------------------
WindowMonitor::WindowMonitor()
:
m_uiEnumInterval( 5000 )
{
}
WindowMonitor::~WindowMonitor()
{
	Clean();
}

WCHAR black_list[256][256];
WCHAR black_process[256][256];
typedef struct HWNDLIST
{
	HWND hwnd;
	int count; 
}HWNDL;

HWNDL hwndlist[4096];

int list_len = 0;
int process_len = 0;
static int hwndlist_len = 0;

FILE * fq;
//--------------------------------------------------------------------
// Methods
//--------------------------------------------------------------------
bool WindowMonitor::Initialize()
{
	g_pThis = this;

	m_Log.SetFilename( L"WindowMonitorLog.xml" );

	m_strPipename =  L"\\\\.\\pipe\\ShadoWin.WindowsMon";

	m_hWindowStation = GetProcessWindowStation();
	if ( m_hWindowStation == 0 )
	{
		return false;
	}
	EnumDesktop();

	//fq = fopen("windows_monitor.log.txt","a");
	//fwprintf(fq,L"Start.\n");
	//fclose(fq);
    FILE * fp = fopen("black_list.config.txt","r");
	char t [256];
	WCHAR wt [256];

	int count = 0;
    while(true)
	{
		memset(t,0,256);
		if(fgets(t,512,fp) == NULL)
		{
			break;
		}
		if(t[strlen(t) -1 ] == 10)
		{
			t[strlen(t) -1 ]  = 0;
		}
		memset(wt,0,256 * sizeof(WCHAR));
		MultiByteToWideChar(CP_ACP,   0, t ,   strlen(t), wt  ,   256);   

		count ++;
		if(lstrcmp(wt,L"[End]") == 0)
		{
			break;
		}
		if(count > 256)
		{
			break;
		}
		lstrcpy(black_list[count - 1],wt);
	}
	list_len = count;
	count = 0;
	while(true)
	{
		memset(t,0,256);
		if(fgets(t,512,fp) == NULL)
		{
			break;
		}
		if(t[strlen(t) -1 ] == 10)
		{
			t[strlen(t) -1 ]  = 0;
		}
		MultiByteToWideChar(CP_ACP,   0, t ,   strlen(t) + 1, black_process[count]  ,   256);   
		count ++;
		if(count > 256)
		{
			break;
		}
	}
	process_len = count;
	fclose(fp);
    
	for(int i = 0 ; i < 4096 ; i ++)
	{
		hwndlist[i].count = 0;
		hwndlist[i].hwnd = NULL;
	}
	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 
        0x80000400) ) 
        return false;

	if (!InitializeCriticalSectionAndSpinCount(&cs, 
        0x80000400) ) 
        return false;
	
	HANDLE hMonitor = 
	CreateThread(	0,
					0,
					( LPTHREAD_START_ROUTINE )WindowMonitor::MonitorThread,
					0,
					0,
					0 );
	CloseHandle( hMonitor );
	return true;
}
void WindowMonitor::Clean()
{
	DeleteCriticalSection(&CriticalSection);
}

typedef struct MsgData {
	const WCHAR desktop[128];
	const WCHAR title[1024];
} MSGDATA, *PMSGDATA;


unsigned int __stdcall WindowMonitor::ShowPopupWindowThread(LPVOID lpParam)
{
	EnterCriticalSection(&CriticalSection);
	PMSGDATA p= (PMSGDATA)lpParam;


	HDESK dsk = OpenInputDesktop(0,true,DESKTOP_JOURNALRECORD | DESKTOP_JOURNALPLAYBACK | DESKTOP_CREATEWINDOW | DESKTOP_ENUMERATE | DESKTOP_WRITEOBJECTS | DESKTOP_SWITCHDESKTOP | DESKTOP_CREATEMENU | DESKTOP_HOOKCONTROL | DESKTOP_READOBJECTS);
    SetThreadDesktop(dsk);
	PVOID desktopname = NULL;
	DWORD len = 0;

	GetUserObjectInformation(dsk,UOI_NAME,desktopname,len,&len);
	desktopname = (PVOID) malloc((len + 1) * sizeof(char));
	GetUserObjectInformation(dsk,UOI_NAME,desktopname,len + 1,&len);
	//Ìí¼ÓºÚÃûµ¥
	
	if(lstrcmp(L"Shadowin2010 (Running) - Microsoft Visual Studio",p->title)==0 ||
		lstrcmp(L"Shadowin2010 (Debugging) - Microsoft Visual Studio",p->title)==0 || 
		lstrcmp(L"ShadowinPopUpMsg",p->title)==0 ||
		lstrcmp(L"Default&Debug",p->title)==0 ||  
		lstrcmp(L"SysFader",p->title)==0 //||
		//lstrcmp(L"",p->title)==0 || StrStr(p->title,L"Shadowin") != NULL )
		)
	{
		return 0; 
	}
	WCHAR cmd[2046];
	/*
	lstrcpy(cmd,L"");
	lstrcat(cmd, L"E:\\ppp\\Release\\ShadowinPopUpMsg.exe \"");
	lstrcat(cmd,p->title);
	lstrcat(cmd,L"\" ");
	lstrcat(cmd,p->desktop);
	cmd = 0x00feef80 "ShadowinPopUpMsg.exe "SysFader" Default"
	*/
	WCHAR title[1024];
	lstrcpy(title,p->title);

	StrTrim(title,L"\"!@#$%^&*()_+|-=\\?/><'");
	title[20] = 0;
	wsprintf(cmd,L"ShadowinPopUpMsg.exe \"%s...\" %s",title,p->desktop);
	/*FILE * f = fopen("windows_monitor.log.txt","a");
	fwprintf(f,L"Title is %s. Desktop is %s\n",title,p->desktop);*/
	
	if(lstrcmp(p->desktop,(LPWSTR)desktopname) != 0 && lstrcmp(p->desktop,L"Default") == 0)
	{
		STARTUPINFO si; 
		PROCESS_INFORMATION pi;
		ZeroMemory( &si, sizeof(si)); 
		si.lpDesktop = (LPWSTR)desktopname;
	
		si.cb = sizeof(si); 
		ZeroMemory( &pi, sizeof(pi)); 
	
		int ret = CreateProcess(NULL,cmd,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);	
	}
	//free(lpParam);
	/*fclose(f);*/
	CloseDesktop(dsk);
	LeaveCriticalSection(&CriticalSection);
	return 0;
}

int WindowMonitor::Transport2Server()
{
	wstring strBuffer;
	
	CXMLNode* node_root = CXMLNode::Create( L"root" );

	if ( node_root == 0 )
	{
		return 0;
	}
	for ( size_t i = 0; i < m_WindowAttributeVec.size(); ++i )
	{
		CXMLNode* node_desktop = 
		CXMLNode::Create( L"desktop", m_WindowAttributeVec[ i ].strAttachDesktop );
		CXMLNode* node_title = 
		CXMLNode::Create( L"title", m_WindowAttributeVec[ i ].strWindowTitle );
		MSGDATA * msg = (PMSGDATA)malloc(sizeof(MSGDATA));
		lstrcpy((LPWSTR)(msg->desktop),(LPCWSTR)m_WindowAttributeVec[ i ].strAttachDesktop.c_str());
		lstrcpy((LPWSTR)(msg->title),(LPCWSTR)m_WindowAttributeVec[ i ].strWindowTitle.c_str());
		
		/*for( int i = 0 ; i < count ; i ++ )
		{
		}*/
		
		bool isInBlack(false);
		for( int i = 0 ; i < list_len ; i ++)
		{
			if(lstrcmp(msg->title,black_list[i]) == 0)
			{
				isInBlack = true;
				break;
			}
		}
		if(!isInBlack)
		{
			DWORD tid;
			CreateThread(NULL,0,( LPTHREAD_START_ROUTINE )WindowMonitor::ShowPopupWindowThread,msg,0,&tid);
		}
		node_root->InsChild( node_desktop );
		node_desktop->InsChild( node_title );
	}
	node_root->Output( strBuffer );

#ifdef _DEBUG
	//m_Log.InitRoot( node_root );
	//m_Log.SaveFile(
	//::GetMainDirectory() + L"windowmonitorlog.xml" );
#endif
    /*
	{"Type":0,"NameSpace":"Shadowin.VEManager","ClassName":"VEManager","MethodName":"Switch","Prameters":["b46756c5-0db3-4540-8d54-56839ac36d9a"]}

	const int iBufferSize = 512;
	
	DWORD dwRead = 0;

	char* pRecvBuffer = new char[ iBufferSize ];
	BOOL bReturn = 
	CallNamedPipe(	m_strPipename.c_str(),
					( LPVOID )strBuffer.c_str(),
					( strBuffer.size() + 1 )*sizeof( wchar_t ) ,
					pRecvBuffer,
					iBufferSize,
					&dwRead,
					NMPWAIT_NOWAIT );
					//NMPWAIT_WAIT_FOREVER );
	Safe_Delete_Array( pRecvBuffer );
    */
	m_WindowAttributeVec.clear();
	return 0;
}
int WindowMonitor::EnumNewWindows(	const wstring& name,
									TitleMap& cur, 
									TitleMap& pre )
{
	if ( pre.empty() )
	{
		pre = cur;
		return 0;
	}
	TitleMap::iterator cur_iter = cur.begin();
	TitleMap::iterator pre_iter = pre.end();
	
	for ( ; cur_iter != cur.end(); ++cur_iter )
	{
		if ( pre.find( cur_iter->first ) == pre.end() )
		{
			WindowAttribute waTemp;
			waTemp.strAttachDesktop = name;
			waTemp.strWindowTitle =  cur_iter->first;
			m_WindowAttributeVec.push_back( waTemp );
		}
	}
	pre.clear();
	pre = cur;
	cur.clear();
	return 0;
}
int WindowMonitor::EnumDesktop()
{
	EnumDesktops(	m_hWindowStation,
					( DESKTOPENUMPROC )EnumDesktopCallback,
					0 );
	return 0;
}

unsigned int __stdcall WindowMonitor::EnumNewDesktopCallback( wchar_t* name, unsigned int param )
{
	if ( name == 0 || g_pThis == 0 ) return 0;

	HDESK hDesk = OpenDesktop( name, 0, 0, GENERIC_ALL );
	if ( hDesk )
	{
		DesktopAttribute daTemp;

		daTemp.hDesktop = hDesk;
		daTemp.strDesktopName = name;
		bool find = false;
		for ( size_t i = 0; i < g_pThis->GetDesktopVec().size(); ++i )
		{
			if(g_pThis->GetDesktopVec()[i].strDesktopName.compare(name) == 0)
			{
				find = true;
				break;
			}
		}
		if(!find)
		{
			g_pThis->GetDesktopVec().push_back( daTemp );
		}
	}
	return WindowMonitor::c_uiEnumFunctionReturn;
}

int WindowMonitor::EnumNewDesktop()
{
	EnumDesktops(	m_hWindowStation,
					( DESKTOPENUMPROC )EnumNewDesktopCallback,
					0 );
	return 0;
}



int WindowMonitor::EnumWindowByDesktop()
{
	for ( size_t i = 0; i < m_DesktopVec.size(); ++i )
	{

		EnumDesktopWindows(	m_DesktopVec[ i ].hDesktop, 
							( WNDENUMPROC )EnumDesktopWindowsCallback, 
							i );
		EnumNewWindows(	m_DesktopVec[ i ].strDesktopName,
						m_DesktopVec[ i ].PreviousTitles,
						m_DesktopVec[ i ].CurrentTitles );
		if ( !m_WindowAttributeVec.empty() )
		{
			Transport2Server();
			m_WindowAttributeVec.clear();
		}
	}

	return 0;
}
int WindowMonitor::RefreshWindowMap()
{
	return 0;
}
//--------------------------------------------------------------------
// Core thread functions
//--------------------------------------------------------------------
unsigned int __stdcall WindowMonitor::MonitorThread( void* param )
{
	assert( g_pThis );
	SetTimer( 0, 0, g_pThis->GetEnumInterval(), ( TIMERPROC )( TimerCallback ) );
	SetTimer( 0, 0, 30 * 1000, ( TIMERPROC )CleanHwnd  );
	MSG msg;
	while ( GetMessage( &msg, 0, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return 0;
}
//--------------------------------------------------------------------
// System callback functions
//--------------------------------------------------------------------
unsigned int __stdcall WindowMonitor::EnumStationCallback( wchar_t *name, unsigned int param )
{
	assert( g_pThis );

	return WindowMonitor::c_uiEnumFunctionReturn;
}
unsigned int __stdcall WindowMonitor::EnumDesktopCallback( wchar_t* name, unsigned int param )
{
	if ( name == 0 || g_pThis == 0 ) return 0;

	HDESK hDesk = OpenDesktop( name, 0, 0, GENERIC_ALL );
	if ( hDesk )
	{
		DesktopAttribute daTemp;

		daTemp.hDesktop = hDesk;
		daTemp.strDesktopName = name;

		g_pThis->GetDesktopVec().push_back( daTemp );
#ifdef _DEBUG
		g_pThis->GetLog().AddLog( L"Desktop", name );
#endif
	}
	return WindowMonitor::c_uiEnumFunctionReturn;
}

unsigned int __stdcall WindowMonitor::EnumDesktopWindowsCallback( HWND hwnd, unsigned int param )
{
	assert( g_pThis );

	static const unsigned int uiBufferSize = 256;
	
	unsigned int uiDesktopIndex = param;

	if ( !IsWindowVisible( hwnd ) )
	{
		return WindowMonitor::c_uiEnumFunctionReturn;
	}
	//check the process
	//HwndList
	/*if(HwndList.)
	{
		return  WindowMonitor::c_uiEnumFunctionReturn;
	}
	HwndList.insert(pair<HWND, bool>(hwnd, true));*/
	//hwndlist

	DWORD pid;
	GetWindowThreadProcessId(hwnd,&pid);
	if(pid)
	{
		HMODULE hMods[1024];
		DWORD cbNeeded;
		unsigned int i;

		HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE,pid);
		//ExplorerPID = pid;
		DWORD ret = EnumProcessModules(handle, hMods, sizeof(hMods), &cbNeeded);
		DWORD lstError = GetLastError();

		if( ret )
		{
			for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
			{
				//TCHAR szModName[MAX_PATH];
				TCHAR szProcessName[MAX_PATH];
				// Get the full path to the module's file.szProcessName = 0x0012e2d0 ""


				if(GetModuleBaseName( handle, hMods[i], szProcessName, sizeof(szProcessName)/sizeof(TCHAR) ))
				{
					for(int j = 0 ; j < process_len ; j ++)
					{
						if(lstrcmp(szProcessName,black_process[j]) == 0)
						{
							return  WindowMonitor::c_uiEnumFunctionReturn;
						}
					}
				}
			}
		}
		CloseHandle( handle );
	}
	wchar_t strWindowText[ uiBufferSize ];

	ZeroMemory( strWindowText, sizeof( wchar_t )*uiBufferSize );

	GetWindowText( hwnd, strWindowText, uiBufferSize );

	std::wstring strText( strWindowText );	

 //   EnterCriticalSection(&cs);
	//for(int i = 0 ; i < hwndlist_len ; i ++ )
	//{
	//	if(hwndlist[i].hwnd == hwnd)
	//	{
	//		if( hwndlist[i].count > 1)//hlc
	//		{
	//			return WindowMonitor::c_uiEnumFunctionReturn;
	//		}
	//		hwndlist[i].count ++;
	//	}
	//}

	//if(hwndlist_len > 4096 )
	//{
	//	hwndlist_len = 4095;
	//}
	//hwndlist[hwndlist_len].hwnd = hwnd;
	//hwndlist_len ++;
	//LeaveCriticalSection(&cs);

	if ( strText.size() )
	{
		vector< DesktopAttribute >& rDesktopVec = g_pThis->GetDesktopVec();

		if ( !rDesktopVec[ uiDesktopIndex ].PreviousTitles[ strText ] )
		{
			rDesktopVec[ uiDesktopIndex ].PreviousTitles[ strText ] = true;
		}
	}
	return WindowMonitor::c_uiEnumFunctionReturn;
}
void __stdcall WindowMonitor::CleanHwnd( HWND, UINT, UINT_PTR, DWORD )
{
	EnterCriticalSection(&cs);
	for(int i = 0 ; i < 4096 ; i ++)
	{
		hwndlist[i].count = 0;
		hwndlist[i].hwnd = NULL;
	}
	hwndlist_len = 0;
	LeaveCriticalSection(&cs);
}
void __stdcall WindowMonitor::TimerCallback( HWND, UINT, UINT_PTR, DWORD )
{
	assert( g_pThis );
	//g_pThis->GetDesktopVec().clear();
	//g_pThis->EnumDesktop();
	//g_pThis->EnumNewDesktop();
	g_pThis->EnumWindowByDesktop();
	g_pThis->RefreshWindowMap();
//	if ( GetNewWindow( m_CurrentTitles, m_PrevTitles, m_NewTitles ) )
//	{
//#ifdef _DEBUG
//		//m_Log.AddLog( L"new", oss.str().c_str() );
//		for ( size_t i = 0; i < m_NewTitles.size(); ++i )
//		{
//			m_Log.AddLog( L"New", m_NewTitles[ i ] );
//		}
//#endif
//		SendInfo2Host( m_NewTitles );
//	}
//	m_PrevTitles = m_CurrentTitles;
//	m_CurrentTitles.clear();
//	m_NewTitles.clear();
}
