//XMLlog.h
#pragma once

#define _CRT_SECURE_NO_WARNINGS
//--------------------------------------------------------------------
// File include
//--------------------------------------------------------------------
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <list>

using namespace std;
//--------------------------------------------------------------------
// Class declare
//--------------------------------------------------------------------
class CXMLParser;
class CXMLNode;
//--------------------------------------------------------------------
// Class XMLLog define
//--------------------------------------------------------------------
class XMLLog
{
public:
	//Flag
	enum Extra_Info
	{
		no_extra_info	= 0x0001,
		current_line	= 0x0002,
		current_file	= 0x0004,
		system_time		= 0x0008
	};

	enum Output_Mode
	{

	};

	enum
	{
		max_log = 100
	};
public:
	//Constructor/Destructor
	XMLLog(	const wstring& filename = L"log.xml",
			int log_num = max_log, 
			bool autosave = true );
	virtual ~XMLLog();
public:
	//Methods
	bool AddLog(	const wstring&, 
					const wstring&, 
					Extra_Info = no_extra_info );

	bool SaveLog( const wstring& );
	void ClearLog();
private:
	//Internals
	void Clean();

public:
	//Set/Get
	void SetFilename(const wstring& filename)	{ m_strFilename = filename; } 

	const wstring& GetFilename() const			{ return m_strFilename; } 
private:
	//Data members
	CXMLParser*		m_xmlLog;
	CXMLNode*		m_xndRoot;
	
	wstring			m_strFilename;
	unsigned int	m_uiCounter;
	unsigned int	m_uiMaxLog;
	unsigned int	m_uiCount;

	bool			m_bAutoSave;

	list<CXMLNode*>	m_Logs;
};