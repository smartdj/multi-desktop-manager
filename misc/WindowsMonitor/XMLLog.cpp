//XMLLog.cpp
#include "XMLLog.h"
#include "XMLParser.h"
#include "CommonMisc.h"


XMLLog::XMLLog( const wstring& filename, int log_num, bool autosave )
:
m_strFilename( filename ),
m_bAutoSave( autosave ),
m_uiMaxLog( log_num ),
m_uiCount( 0 )
{
	m_xmlLog = new CXMLParser;

	m_xndRoot = m_xmlLog->GetRoot();

	m_xndRoot->SetTag( L"XMLLog" );
}

XMLLog::~XMLLog()
{
	Clean();
}

void XMLLog::Clean()
{
	if ( m_bAutoSave )
	{
		SaveLog( m_strFilename );
	}
	Safe_Delete( m_xmlLog );
}

bool XMLLog::AddLog(const wstring& tag,
					const wstring& info,
					Extra_Info flag)
{
	CXMLNode* xml_info = CXMLNode::Create( tag );

	if ( xml_info ) 
	{
		xml_info->SetContent(info);
	}
	else
	{
		return false;
	}
	m_Logs.push_back( xml_info );
	if ( ++m_uiCounter > m_uiMaxLog )
	{
		--m_uiCounter;

		m_Logs.erase( m_Logs.begin() );
	}
	if ( m_bAutoSave )
	{
		m_xndRoot->InsChild( xml_info );
		SaveLog( m_strFilename );
	}
	if (flag == no_extra_info)
	{
		return true;
	}
	if (flag && system_time)
	{
		CXMLNode* xml_info = CXMLNode::Create(L"SystemTime");
		if ( xml_info == NULL )
		{
			return false;
		}
		time_t theTime;
		time( &theTime ); 
		tm *t = localtime( &theTime );
		wostringstream ss;
		ss << asctime( t );
		xml_info->SetContent(ss.str());
		m_xndRoot->InsChild( xml_info );
	}
	return true;
}

bool XMLLog::SaveLog(const wstring& path)
{
	//list<CXMLNode*>::iterator iter = m_Logs.begin();

	//for ( ; iter != m_Logs.end(); ++iter )
	//{
	//	m_xndRoot->InsChild( *iter );
	//}
	return m_xmlLog->SaveFile( path );
}