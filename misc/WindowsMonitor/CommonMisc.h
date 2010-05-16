//CommonMisc.h
#pragma once
//--------------------------------------------------------------------
// Constant variable
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Type define
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Inline function
//--------------------------------------------------------------------
template<class T> inline unsigned int NumOfArray(T t)
{
	return sizeof( T )/sizeof( T[ 0 ] );
}
template<class T> inline void Safe_Release(T t)
{
	if ( t )
	{
		t->Release();
		t = 0;
	}
}
template<class T> inline void Safe_Delete(T t)
{
	if ( t )
	{
		delete t;
		t = 0;
	}
}
template<class T> inline void Safe_Delete_Array(T t)
{
	if ( t )
	{
		delete [] t;
		t = 0;
	}
}