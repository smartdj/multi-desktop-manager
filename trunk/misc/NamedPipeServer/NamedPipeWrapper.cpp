#include "NamedPipeWrapper.h"

int NamedPipeWrapper::Read(char * buf,unsigned long MaxLength,unsigned long *LengthWritten)
{
	int ret = ReadFile(pipehandle,buf,MaxLength,LengthWritten,NULL);
	if(ret == 0)
	{
		return GetLastError();
	}
	return 0;
}

std::string NamedPipeWrapper::Read()
{
	std::string str = "";
	return str;
}

int NamedPipeWrapper::Write(char * text,unsigned long NumberOfBytesToWritte,unsigned long *NumberOfBytesWritten)
{
	int ret = WriteFile(pipehandle,text,NumberOfBytesToWritte,NumberOfBytesWritten,NULL);
	if(ret == 0)
	{
		return GetLastError();
	}
	return 0;
}

int NamedPipeWrapper::Write(std::string text)
{
	return 0;
}