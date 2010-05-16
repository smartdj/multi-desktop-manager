#include "XMLParser.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

long const CXMLNode::_MAGICNUM = 0xf123e123; 

CXMLNode::CXMLNode()
:magicnum(_MAGICNUM)
,tag(L"")
,content(L"")
,ref(0)
,size(0)
{
}
CXMLNode::~CXMLNode()
{
    for( size_t i = 0; i < children.size(); ++i )
        delete children[ i ];

    magicnum = 0;
}

void* CXMLNode::operator new(size_t size)
{
    return malloc(sizeof(CXMLNode));
}
void CXMLNode::operator delete( void* p )
{
    if( p == NULL )
        return;

    ((CXMLNode*)p)->ref -= 1;
    if( ((CXMLNode*)p)->ref == 0 )
    {
        free( (CXMLNode*)p );
        return;
    }
}

CXMLNode* CXMLNode::Create( wstring const& sTag, wstring const& sContent )
{
    CXMLNode* p = new CXMLNode();
    p->SetTag( sTag );
    p->SetContent( sContent );
    return p;
}

void CXMLNode::Destory( CXMLNode*& p )
{
	if( p != NULL && p->magicnum == CXMLNode::_MAGICNUM )
    		delete p;
    	p = NULL;
}

CXMLNode* CXMLNode::Clone( CXMLNode const* node )
{
    if( node == NULL )
        return NULL;

    CXMLNode* p = Create( node->tag, node->content );
    for( size_t i = 0; i < node->children.size(); ++i )
        p->children.push_back( CXMLNode::Clone( node->children[ i ] ) );

    p->props = node->props;

    return p;
}
wstring CXMLNode::GetTag()
{
    return tag;
}
void CXMLNode::SetTag( wstring const& str )
{
    tag = str;
}

wstring CXMLNode::GetContent()
{
    return content;
}

void CXMLNode::GetContent( wchar_t ch, vector<wstring>& vec )
{
    if( ch == L'\0' )
        return;

    wchar_t const* p = content.c_str();
    wstring str = L"";
    while( *p != L'\0' )
    {
        if( *p == ch )
        {
            vec.push_back( str );
            str = L"";
        }
        else
            str += *p;

        ++p;
    }
    if( str != L"" )
        vec.push_back( str );
}

wstring CXMLNode::GetContent( wstring const& tag, int exp, vector<wstring>& vec )
{
	CXMLNode* pChild = GetChild( tag );
	if( pChild == NULL || vec.size() % 2 == 1 )
		return L"";

	if( exp == EXP_OR )
	{
		for( size_t i = 0; i < vec.size(); i+=2 )
		{
			CXMLNode* p = GetChild(vec[ i ]);
			if( p && (p->GetContent() == vec[ i+1 ]) )
				return pChild->GetContent();
		}
	}
	else if( exp == EXP_AND )
	{
		for( size_t i = 0; i < vec.size(); i+=2 )
		{
			CXMLNode* p = GetChild(vec[ i ]);
			if( !p || (p->GetContent() != vec[ i+1 ]) )
				return L"";
		}
		return pChild->GetContent();
	}

	return L"";
}
void CXMLNode::SetContent( wstring const& str )
{
    content = str;
}
void CXMLNode::Output( wstring& mem, long fmt )
{

	wchar_t c0d = 0x000d;
	wchar_t c0a = 0x000a;
	wchar_t c09 = 0x0009;
	wchar_t c20 = 0x0020;

    for( int i = 0; i < fmt; ++i )
        //mem += L"\t";
	mem += c09;

    mem += L"<";
    mem += tag;

    map<wstring,wstring>::iterator it = props.begin();
    while( it != props.end() )
    {
        mem += L" ";
        mem += it->first;
        mem += L"=";
        mem += L"\"";
        mem += it->second;
        mem += L"\" ";
        it++;
    }

    if( content.empty() && children.empty() )
    {
        //mem += L"/>\n";
	mem += L"/>";
	mem += c0d;
	mem += c0a;
        return;
    }

    mem += L">";

    if( !content.empty() )
    {
	for( size_t i = 0; i < content.size(); ++i )
	{
		if( content[ i ] != c0d && content[ i ] != c0a && content[ i ] != c09 && content[ i ] != c20 )
		{
			mem += content;
			break;
		}
	}
	
    }
    if( !children.empty() )
    {
        //mem += L"\n";
	mem += c0d;
	mem += c0a;
        for( size_t i = 0; i < children.size(); ++i )
        {
            children[ i ]->Output( mem, fmt+1 );
            //mem += L"\n";
	    mem += c0d;
	    mem += c0a;
        }

        for( long i = 0; i < fmt; ++i )
            //mem += L"\t";
	    mem += c09;
    }

    mem += L"</";
    mem += tag;
    mem += L">";
}

bool CXMLNode::Parser( wstring const& str )
{
    size_t i = str.find( L'<' );
    if( wstring::npos == i )
        return false;
    ++i;
    while( i < wstring::npos && str[ i ] != L'/' && str[ i ] != L'>' && str[ i ] != L' ' )
    {
        tag += str[ i ];
        ++i;
    }
    while( i < wstring::npos && str[ i ] == L' ' )
        ++i;

    wstring tk = L"";
    wstring tv = L"";
    while( i < wstring::npos && str[ i ] != L'/' && str[ i ] != L'>' )
    {
        if( str[ i ] == L'=' )
        {
            while( str[ i ] != L'\"' )
            {
                if( i >= wstring::npos )
                    return false;
                ++i;
            }
            ++i;
            if( i < wstring::npos )
            {
                while( str[ i ] != L'\"' )
                {
                    if( i == wstring::npos )
                        return false;
                        tv += str[ i ];
                        ++i;
                }
            }
            InsProp( tk, tv );
            tk = L"";
            tv = L"";
        }
        else if( str[ i ] != L' ' )
            tk += str[ i ];

        ++i;
    }
    if( i+1 < wstring::npos )
    {
        if( str[ i ] == L'/' )
        {
            if( i+1 >= wstring::npos )
                return false;
            else if( str[ i+1 ] == L'>' )
                return true;
            else
                return false;
        }
        else
        {
            ++i;
            long count = 0;
            size_t end = str.rfind( L"</" );
            if( end >= wstring::npos )
                return false;
            wstring tmp = L"";
            while( i < end && count >= 0 )
            {
                if( str[ i ] == L'<' && str[ i+1 ] != L'/' )
                {
                    ++count;
                }
                else if( (str[ i ] == L'<' && str[ i+1 ] == L'/' )
                || ( str[ i ] == L'/' && str[ i+1 ] == L'>' ) )
                {
                    --count;
                    if( count == 0 )
                    {
                        while( i < end && str[ i ] != L'>' )
                        {
                            tmp += str[ i ];
                            ++i;
                        }
                        tmp += str[ i ];
                        CXMLNode* p = CXMLNode::Create();
                        if( !p->Parser( tmp ) )
                            return false;
                        InsChild( p );
                        tmp = L"";

                    }
                    else if( count < 0 )
                        return false;

                }
                if( i < end && str[ i ] != L'\n' && str[ i ] != L'\t' )
                {
                    if( count > 0 )
                        tmp += str[ i ];
                    if( count == 0 && str[ i ] != L'>' )//&& str[ i ] != L' ' )
                        content += str[ i ];
                }
                ++i;
            }
            if( count != 0 )
                return false;

            if( i == end )
                return true;
        }
    }

    return false;
}

CXMLNode* CXMLNode::GetChild( wstring const& str )
{
    for( size_t i = 0; i < children.size(); ++i )
        if( children[ i ]->tag == str )
            return children[ i ];

    return NULL;
}
bool CXMLNode::GetChild( wstring const& str, vector<CXMLNode*>& vec, long flag )
{
    for( size_t i = 0; i < children.size(); ++i )
    {
        if( children[ i ]->tag == str )
            vec.push_back( children[ i ] );

        if( flag & FIND_SUBTREE )
            children[ i ]->GetChild( str, vec, flag );

    }
    return vec.size()>0;
}
void CXMLNode::InsChild( CXMLNode* node, bool rh )
{
    if( node == NULL )
        return;

    if( node->ref > 0 && !rh )
    {
        InsChild( CXMLNode::Clone( node ) );
    }
    else
    {
        node->ref += 1;
        children.push_back( node );
    }
}
void CXMLNode::DelChild( wstring const& str )
{
    vector<CXMLNode*>::iterator it = children.begin();
    vector<CXMLNode*>::iterator dst = children.end();
    while( it != dst )
    {
        if( (*it)->tag == str )
        {
            delete *it;
            swap( *it, *(dst-1) );
            --dst;
        }
        else
            ++it;
    }
    children.erase( dst, children.end() );
}

void CXMLNode::DelChild( CXMLNode*& p )
{
    if( p == NULL )
        return;

    vector<CXMLNode*>::iterator it = children.begin();
    vector<CXMLNode*>::iterator dst = children.end();
    while( it != dst )
    {
        if( (*it) == p )
        {
            delete *it;
            swap( *it, *(dst-1) );
            --dst;
        }
        else
            ++it;
    }
    children.erase( dst, children.end() );
    p = NULL;
}

long CXMLNode::DelChildren( wstring const& str, long flag )
{
	vector<CXMLNode*> vec;
	GetChild( str, vec, flag );
	for( size_t i=0; i < vec.size(); ++i )
		Destory( vec[ i ] );
	CheckAndFix();
	return vec.size();	
}
wstring CXMLNode::GetProp( wstring const& str )
{
    map<wstring,wstring>::iterator it = props.find( str );
    if( it != props.end() )
        return it->second;
    return L"";
}

bool CXMLNode::SetProp( wstring const& pk, wstring const& pv )
{
    map<wstring,wstring>::iterator it = props.find( pk );
    if( it == props.end() )
        return false;

    it->second = pv;
    return true;
}

void CXMLNode::InsProp( wstring const& pk, wstring const& pv )
{
    props[ pk ] = pv;
}

void CXMLNode::DelProp( wstring const& str  )
{
    props.erase( props.find(str) );
}

wstring CXMLNode::GetAttribute( wstring const& str )
{
	return GetProp( str );
}

bool CXMLNode::SetAttribute( wstring const& pk, wstring const& pv )
{
	return SetProp( pk, pv );
}

void CXMLNode::InsAttribute( wstring const& pk, wstring const& pv )
{
	return InsProp( pk, pv );
}

void CXMLNode::DelAttribute( wstring const& str  )
{
	return DelProp( str );
}

void CXMLNode::CheckAndFix()
{
	vector<CXMLNode*>::iterator it = children.begin();
	vector<CXMLNode*>::iterator dst = children.end();
	while( it != dst )
	{
	    if( (*it)->magicnum != _MAGICNUM )
	    {
		    swap( *it, *(dst-1) );
		    --dst;
	    }
	    else
	    {
		    (*it)->CheckAndFix();
		    ++it;
	    }
    	}
	children.erase( dst, children.end() );
}

void CXMLNode::RegCall( wstring const& str, PFUN pf )
{
    exes[ str ] = pf;
}
void CXMLNode::UnregCall( wstring const& str )
{
    exes.erase( exes.find( str ) );
}
long CXMLNode::TryExe()
{
    long ret = 0;
    vector<CXMLNode*> vec;
    map<wstring,PFUN>::iterator it = exes.begin();
    while( it != exes.end() )
    {
        vec.clear();
        GetChild( it->first, vec );
        for( size_t i = 0; i < vec.size(); ++i )
            ret = (it->second)( vec[ i ] );
        ++it;
    }  
    return ret;
}

unsigned long CXMLNode::GetSize( bool flush )
{
    if( size && !flush )
        return size;

    size = sizeof( CXMLNode );
    size += tag.length()*sizeof(wchar_t) ;
    size += content.length()*sizeof(wchar_t) ;
    {
        vector<CXMLNode*>::iterator it = children.begin();
        vector<CXMLNode*>::iterator dst = children.end();
        while( it != dst )
        {
            size += (*it)->GetSize();
            ++it;
        }
    }

    {
        map<wstring,wstring>::iterator it = props.begin();
        map<wstring,wstring>::iterator dst = props.end();
        while( it != dst )
        {
            size += it->first.length()*sizeof(wchar_t) ;
            size += it->second.length()*sizeof(wchar_t) ;
            ++it;
        }
    }
    return size;
}

CXMLNode* CXMLParser::GetRoot()
{
    return root;
}

bool CXMLParser::InitRoot( CXMLNode* p )
{
	if( p == NULL || p->magicnum != CXMLNode::_MAGICNUM )
		return false;
	CXMLNode::Destory( root );
	root = p;
	return true;
}

CXMLNode* CXMLParser::InitRoot()
{
	if( root != NULL )
		CXMLNode::Destory( root );
	root = CXMLNode::Create();
	return root;
}

CXMLParser::CXMLParser()
:root(NULL)
,enc( E_ERR )
{
	InitRoot();
}

CXMLParser::~CXMLParser()
{
    CXMLNode::Destory(root);
}
bool CXMLParser::LoadMem( wstring& mem )
{
    if( root != NULL )
        CXMLNode::Destory(root);

    wstring bstr = L"<?";
    wstring estr = L"?>";
    for( size_t i = mem.find(bstr); i != wstring::npos; i = mem.find(bstr) )
    {
        size_t j = mem.find( estr );
        if( wstring::npos == j )
            return false;
        mem.erase( mem.begin()+i, mem.begin()+j+estr.size() );
    }

    bstr = L"<!--";
    estr = L"-->";
    for( size_t i = mem.find(bstr); i != wstring::npos; i = mem.find(bstr) )
    {
        size_t j = mem.find( estr );
        if( wstring::npos == j )
            return false;
        mem.erase( mem.begin()+i, mem.begin()+j+estr.size() );
    }

    InitRoot();
    if( !root->Parser( mem ) )
    {
        CXMLNode::Destory( root );
        return false;
    }
    return true;
}
bool CXMLParser::LoadFile( wstring const& path )
{
    if( path.empty() )
        return false;

    char fname[ 1024 ] = {0};
	setlocale( LC_ALL, ".ACP" );
    wcstombs( fname, path.c_str(), 1024 );   
    FILE* fp = fopen( fname, "rb" );
    if( fp == NULL )
        return false;

    unsigned short flag = 0;
    fread( &flag, sizeof(wchar_t), 1, fp );
    if( flag == 0xfeff )
	    enc = E_UTF16LE;
    else if( flag == 0xfffe )
	    enc = E_UTF16BE;
    else
    {
        fclose(fp);
        return false;
    }
    fseek( fp, 0, SEEK_END );
    size_t len = ftell( fp );
    len /= 2;
    wchar_t* wsRawData = new wchar_t[ len ];
    fseek( fp, 2, SEEK_SET );
    fread( wsRawData, sizeof(wchar_t), len, fp );
    
    if( enc == E_UTF16BE )//require transfer 
    {
	    for( size_t i = 0; i < len-1; ++i )
	    {
   		    unsigned char* p = (unsigned char*)&wsRawData[ i ];
		    unsigned char tmp = *p;
		    *p = *(p+1);
		    *(p+1) = tmp;
	    }
    }
    wsRawData[ len-1 ] = 0;
    wstring mem( wsRawData );
    delete [] wsRawData;
    fclose( fp );

    if( LoadMem( mem ) )
    {
        filename = path;
        return true;
    }
    return false;
}

bool CXMLParser::SaveMem( wstring& mem  )
{
    if( !root )
        return false;
    root->Output( mem );
    return true;
}

bool CXMLParser::SaveFile( wstring const& path )
{

    if( !path.empty() )
        filename = path;
    wstring mem;
    if( !SaveMem( mem ) )
        return false;

    char fname[ 1024 ] = {0};
    wcstombs( fname, filename.c_str(), 1024 );
    FILE* fp = fopen( fname, "wb" );
    unsigned long flag = 0;
    switch( enc )
    {
	    case E_UTF16LE:
		    flag = 0xfeff;
		    fwrite( &flag, 2, 1, fp );
		    break;
	    case E_UTF16BE:
		    flag = 0xfffe;
		    fwrite( &flag, 2, 1, fp );
		    for( size_t i = 0; i < mem.size(); ++i )
		    {
			    unsigned char* p = (unsigned char*)&mem[ i ];
			    unsigned char tmp = *p;
			    *p = *(p+1);
			    *(p+1) = tmp;
		    }
		    break;
	    default:
		    fclose(fp);
		    return false;
    }
    fwrite( (wchar_t const*)mem.c_str(), 2, mem.size(), fp );
    fclose(fp);
    return true;
}

int CXMLParser::SetCode( int c )
{
	int old = enc;
	enc = c;
	return old;
}

unsigned long CXMLParser::GetSize( bool flush )
{
    return sizeof(CXMLParser) + filename.length()*sizeof(wchar_t) + (!root?0:root->GetSize(flush));
}
