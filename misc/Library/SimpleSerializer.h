#include <string>
#include <vector>
namespace Serializer
{
	typedef std::map <std::string,std::string> Dictionary;
	class Command
	{
	private:
		std::string ClassName;
		std::string MethodName;
		std::string NameSpace;
		Dictionary Prameters;

	public:
		std::string GetClass()
		{
			return this->ClassName;
		}
		std::string GetMethod()
		{
			return this->Method;
		}
		std::string GetNameSpace()
		{
			return this->NameSpace;
		}
		
		Dictionary GetPrameters()
		{
			return this->Prameters;
		}
		std::string Serialize(BaseClass & obj)
		{

		}
		static void DeSerialize(std::string xml,Command & obj);
	};
}