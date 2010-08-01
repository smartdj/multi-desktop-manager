#include <string>
#include <vector>
class Command
{
private:
	std::string MethodName;
	std::vector<std::string> Prameters;
	int NumOfPrameters;
public:
	std::string GetMethodName()
	{
		return MethodName;
	}
	void SetMethodName(std::string MethodName)
	{
		this->MethodName = MethodName;
	}
	void GetPrameters()
	{
		return Prameters;
	}
};