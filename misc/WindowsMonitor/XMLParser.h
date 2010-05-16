#ifndef XMLPARSER_H_INCLUDED
#define XMLPARSER_H_INCLUDED

#include <vector>
#include <string>
#include <map>
using namespace std;

class CXMLNode
{
	public:
		long magicnum;
		static long const _MAGICNUM;

    private:
        wstring tag;
        wstring content;
        map<wstring,wstring> props;
        vector<CXMLNode*> children;
        typedef long (*PFUN)(CXMLNode*);
        map<wstring,PFUN> exes;
        long ref;
        unsigned long size;	

        CXMLNode();
        ~CXMLNode();
        static void* operator new( size_t );
        static void operator delete( void* );

        CXMLNode( CXMLNode const& );
        CXMLNode& operator = ( CXMLNode const& );



    public:
        static CXMLNode* Create( wstring const& = L"", wstring const& = L""  );
        static void Destory( CXMLNode*& );
        static CXMLNode* Clone( CXMLNode const* );

        wstring GetTag();
        void SetTag( wstring const& );

        wstring GetContent();
        void GetContent( wchar_t, vector<wstring>& );
	
	static int const EXP_OR = 0;
	static int const EXP_AND = 1;
	wstring GetContent( wstring const&, int, vector<wstring>& );//整体模式,以子结点作为content
        void SetContent( wstring const& );

        void Output( wstring&, long fmt=0 );
        bool Parser( wstring const& );

	static int const FIND_ONLY = 0;
	static int const FIND_SUBTREE = 1;
        CXMLNode* GetChild( wstring const& );
        bool GetChild( wstring const&, vector<CXMLNode*>&, long = 0 );
        void InsChild( CXMLNode*, bool = false );
        void DelChild( wstring const& );
        void DelChild( CXMLNode*& );
	long DelChildren( wstring const&, long = 0 );

    public:
        wstring GetProp( wstring const& );
        bool SetProp( wstring const&, wstring const& );
        void InsProp( wstring const&, wstring const& );
        void DelProp( wstring const& );

    public:
	wstring GetAttribute( wstring const& );
        bool SetAttribute( wstring const&, wstring const& );
        void InsAttribute( wstring const&, wstring const& );
        void DelAttribute( wstring const& );

        void RegCall( wstring const&, PFUN );
        void UnregCall( wstring const& );
        long TryExe();

        unsigned long GetSize( bool=false );
	void CheckAndFix();
};


class CXMLParser
{
    private:
        CXMLNode* root;
        wstring filename;

   public:
	static int const E_ERR = 0;
	static int const E_UTF16LE = 1;
	static int const E_UTF16BE = 2;

   private:
	long enc;

    public:
        CXMLParser();
        ~CXMLParser();
        bool LoadFile( wstring const& );
        bool LoadMem( wstring& );

        bool SaveFile( wstring const& = L"" );
        bool SaveMem( wstring& );

	bool InitRoot( CXMLNode* );
	CXMLNode* InitRoot(void);
        CXMLNode* GetRoot();
        unsigned long GetSize( bool=false );
	int SetCode( int=E_ERR );
};

#endif // XMLPARSER_H_INCLUDED
