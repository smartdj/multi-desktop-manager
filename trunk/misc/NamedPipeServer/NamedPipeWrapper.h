#include <windows.h> 
#include <string>
class NamedPipeWrapper
{
	NamedPipeWrapper(HANDLE hpipe)
	{
		this->pipehandle = hpipe;
	}
	~NamedPipeWrapper()
	{
		CloseHandle(this->pipehandle);
	}
private:
	HANDLE pipehandle;
public:
	std::string Read();
	int Read(char * buf,unsigned long MaxLength,unsigned long *LengthWritten);
	int Write(char * text,unsigned long NumberOfBytesWritte,unsigned long *NumberOfBytesWritten);
	int Write(std::string text);
};