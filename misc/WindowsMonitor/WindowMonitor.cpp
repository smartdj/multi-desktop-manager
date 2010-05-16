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

WindowMonitor* g_pThis = 0;
//--------------------------------------------------------------------
// Constructor and destructor
//--------------------------------------------------------------------
WindowMonitor::WindowMonitor()
:
m_uiEnumInterval( 2000 )
{
}
WindowMonitor::~WindowMonitor()
{
	Clean();
}

WCHAR black_list[256][256];
WCHAR black_process[256][256];


int list_len = 0;
int process_len = 0;


//--------------------------------------------------------------------
// Methods
//--------------------------------------------------------------------
bool WindowMonitor::Initialize()
{
	g_pThis = this;

	m_Log.SetFilename( L"WindowMonitorLog.xml" );

	m_hWindowStation = GetProcessWindowStation();
	if ( m_hWindowStation == 0 )
	{
		return false;
	}
	EnumDesktop();

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
    

	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 
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
	
	if(lstrcmp(L"ShadowinPopUpMsg",p->title)==0 ||
		lstrcmp(L"Default&Debug",p->title)==0 ||  
		lstrcmp(L"SysFader",p->title)==0 
		)
	{
		return 0; 
	}
	WCHAR cmd[2046];
	WCHAR title[1024];
	lstrcpy(title,p->title);

	StrTrim(title,L"\"!@#$%^&*()_+|-=\\?/><'");
	title[20] = 0;
	wsprintf(cmd,L"ShadowinPopUpMsg.exe \"%s...\" %s",title,p->desktop);
	
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
	CloseDesktop(dsk);
	LeaveCriticalSection(&CriticalSection);
	return 0;
}

int WindowMonitor::Transport2Server()
{
	wstring strBuffer;
	
	//CXMLNode* node_root = CXMLNode::Create( L"root" );

	//if ( node_root == 0 )
	//{
	//	return 0;
	//}
	for ( size_t i = 0; i < m_WindowAttributeVec.size(); ++i )
	{
		//CXMLNode* node_desktop = 
		//CXMLNode::Create( L"desktop", m_WindowAttributeVec[ i ].strAttachDesktop );
		////CXMLNode* node_title = 
		//CXMLNode::Create( L"title", m_WindowAttributeVec[ i ].strWindowTitle );
		MSGDATA * msg = (PMSGDATA)malloc(sizeof(MSGDATA));
		lstrcpy((LPWSTR)(msg->desktop),(LPCWSTR)m_WindowAttributeVec[ i ].strAttachDesktop.c_str());
		lstrcpy((LPWSTR)(msg->title),(LPCWSTR)m_WindowAttributeVec[ i ].strWindowTitle.c_str());
		
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
		//node_root->InsChild( node_desktop );
		//node_desktop->InsChild( node_title );
	}
	//node_root->Output( strBuffer );

	m_WindowAttributeVec.clear();
	return 0;
}
int WindowMonitor::EnumNewWindows(	const wstring& name,
									TitleMap& cur, 
									TitleMap& pre )
{
	static const unsigned int uiBufferSize = 256;
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
			wchar_t strWindowText[ uiBufferSize ];
			ZeroMemory( strWindowText, sizeof( wchar_t )*uiBufferSize );
			int ret = GetWindowText( cur_iter->first, strWindowText, uiBufferSize );
			if(!ret)
			{
				
				WindowAttribute waTemp;
				waTemp.strAttachDesktop = name;
				waTemp.strWindowTitle = strWindowText;
				m_WindowAttributeVec.push_back( waTemp );
			}
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
	/*wchar_t strWindowText[ uiBufferSize ];

	ZeroMemory( strWindowText, sizeof( wchar_t )*uiBufferSize );

	GetWindowText( hwnd, strWindowText, uiBufferSize );

	std::wstring strText( strWindowText );	*/


	/*if ( strText.size() )*/
	{
		vector< DesktopAttribute >& rDesktopVec = g_pThis->GetDesktopVec();

		if ( !rDesktopVec[ uiDesktopIndex ].PreviousTitles[ hwnd ] )
		{
			rDesktopVec[ uiDesktopIndex ].PreviousTitles[ hwnd ] = true;
		}
	}
	return WindowMonitor::c_uiEnumFunctionReturn;
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
