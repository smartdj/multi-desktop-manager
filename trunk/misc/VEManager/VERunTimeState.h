#include <string>
#include <map>
typedef enum
{
	VE_OPEN;
	VE_CLOSED;
	VE_ACTIVE;
}VESTATE;
class DateTime
{
private:
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
public:
	int GetYear()
	{
		return year;
	}
	int GetMonth()
	{
		return month;
	}
	int GetDay()
	{
		return day;
	}
	int GetHour()
	{
		return hour;
	}
	int GetMinute()
	{
		return minute;
	}
	int GetSecond()
	{
		return second;
	}
	std::string ToString()
	{
		char datetime[128];
		sprintf(datetime,"%d-%d-%d %d:%d:%d",year,month,day,hour,minute,second);
		std::string ret = datetime;
		return ret;
	}
};
typedef enum
{
	XP;
	WIN7;
	BOTH;
}OS;
class Storage
{
private:
	std::string Name;
	std::string Id;
	std::string Des;
	std::string Version;
	DateTime CreateTime;
	DateTime LastModified;
	int ShowIndex;
	std::string Owner;
	OS Os;
	bool IsPassWord;
	std::string PassWord;
public:
	std::string GetName()
	{
		return Name;
	}
	std::string GetId()
	{
		return Id;
	}
	std::string GetDes()
	{
		return Des;
	}
	std::string GetVersion()
	{
		return Version;
	}
	DateTime GetCreateTime()
	{
		return CreateTime;
	}
	DateTime GetLastModifyTime()
	{
		return LastModified;
	}
	int GetShowIndex()
	{
		return ShowIndex;
	}
	std::string GetOwner()
	{
		return Owner;
	}
	OS GetSupportOS()
	{
		return Os;
	}
	bool GetIsPassWord()
	{
		return IsPassWord;
	}
	std::string CheckPassWord(std::string pwd)
	{
		return PassWord.compare(pwd) == 0 ;
	}
	//Save Read Write Load Method
	//All this is to write the message to the file or read from file.

};
class VE
{
public:
	std::string Name;
	std::string Id;
	VESTATE State;
	std::string DesktopName;
	int Index;
};
class VERunTimeState
{
private:
	std::string currentVE;
	std::map<std::string,VE> ALLVE;
public:
	std::string GetCurrentVE()
	{
		return this->currentVE;
	}
	VE GetVeInfo(std::string Id)
	{
		return ALLVE[Id];
	}
};