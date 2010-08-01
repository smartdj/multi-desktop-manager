#include <string>
#include <windows.h>
#include <strsafe.h>
class NamedPipeClient
{
private:
	std::string pipeName;
	HANDLE hPipe;
public:
	NamedPipeClient(std::string pipeName)
	{
		this->pipeName = pipeName;
	}
public:
	unsigned long Connect()
	{
		this->hPipe = CreateFileA(this->pipeName.c_str(),GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,0);
	}
	std::string Read()
	{
		//ReadFile(this->hPipe,
	}
};