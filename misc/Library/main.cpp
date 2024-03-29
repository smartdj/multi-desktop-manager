//// Algorithms.cpp : Defines the entry point for the console application.
////
//
//#include "Algorythms.h"
//#include "System.h"
//#include <windows.h>
//#include <iostream>
//#include "boost/scoped_ptr.hpp"
//#include <string>
//using namespace std;
////using namespace Algorythms;
//int _tmain(int argc, _TCHAR* argv[])
//{
//	{  
//		boost::scoped_ptr<std::string>   p(new std::string("Use scoped_ptr often."));  
//		// 打印字符串的值  
//		if (p)    std::cout << *p << '\n';      
//		// 获取字符串的大小  
//		size_t i=p->size();  
//		// 给字符串赋新值  
//		*p="Acts just like a pointer";  
//		std::cout << *p << '\n';
//	} // 这里p被销毁，并删除std::string 
//	//int Array[] = {2,3,6,3,64,75,42356,563};
//	//insert_sort(Array,8,true);
//	//Algorythms::Bubble_Sort(Array,8);
//	//Algorythms::Build_Max_Heap(Array,8);
//	/*System::Console::HelloWorld();
//	Algorythms::Heap_Sort(Array,8);
//	Algorythms::print(Array,8);
//	Algorythms::Merge_Sort(Array,0,7);
//	Algorythms::print(Array,8);
//	Algorythms::Reserver(Array,8);
//	Algorythms::print(Array,8);
//	WCHAR Str[3][MAX_PATH];
//	lstrcpyW(Str[0],L"abc");
//	lstrcpyW(Str[1],L"cde");
//	lstrcpyW(Str[2],L"123");
//	Algorythms::print(Str,3);*/
//	//string s = "dsafasdfasdfasd%ldsafdsa";
//	//int a = s.find("%l");
//	System::pause();
//	return 0;
//}
//
//#include <stdio.h>
//#include "log4cpp/Portability.hh"
//#ifdef LOG4CPP_HAVE_UNISTD_H
//#include <unistd.h>
//#endif
//#include <iostream>
//#include "log4cpp/Category.hh"
//#include "log4cpp/Appender.hh"
//#include "log4cpp/FileAppender.hh"
//#include "log4cpp/OstreamAppender.hh"
//#ifdef LOG4CPP_HAVE_SYSLOG
//#include "log4cpp/SyslogAppender.hh"
//#endif
//#include "log4cpp/Layout.hh"
//#include "log4cpp/BasicLayout.hh"
//#include "log4cpp/Priority.hh"
//#include "log4cpp/NDC.hh"
//#pragma comment(lib,"log4cpp.lib")
//int main(int argc, char** argv) {
//	log4cpp::Appender* appender;
//#ifdef LOG4CPP_HAVE_SYSLOG
//	log4cpp::SyslogAppender* syslogAppender;
//
//	syslogAppender = new log4cpp::SyslogAppender("syslog", "log4cpp");
//#else
//	log4cpp::Appender* syslogAppender;
//
//	syslogAppender = new log4cpp::OstreamAppender("syslogdummy", &std::cout);
//#endif
//
//	if (argc < 2) {
//		appender = new log4cpp::OstreamAppender("default", &std::cout);
//	} else {
//		appender = new log4cpp::FileAppender("default", argv[1]);
//	}
//
//	syslogAppender->setLayout(new log4cpp::BasicLayout());
//	appender->setLayout(new log4cpp::BasicLayout());
//
//	log4cpp::Category& root = log4cpp::Category::getRoot();
//	root.addAppender(syslogAppender);
//	root.setPriority(log4cpp::Priority::ERROR);
//
//	log4cpp::Category& sub1 = log4cpp::Category::getInstance(std::string("sub1"));
//	sub1.addAppender(appender);
//
//	log4cpp::Category& sub2 = log4cpp::Category::getInstance(std::string("sub1.sub2"));
//
//	log4cpp::NDC::push(std::string("ndc1"));
//
//	std::cout << " root prio = " << root.getPriority() << std::endl;
//	std::cout << " sub1 prio = " << sub1.getPriority() << std::endl;
//	std::cout << " sub2 prio = " << sub2.getPriority() << std::endl;
//
//	root.error("root error");
//	root.warn("root warn");
//	sub1.error("sub1 error");
//	sub1.warn("sub1 warn");
//	sub2.error("sub2 error");
//	sub2.warn("sub2 warn");
//
//	sub1.setPriority(log4cpp::Priority::INFO);
//	std::cout << " root prio = " << root.getPriority() << std::endl;
//	std::cout << " sub1 prio = " << sub1.getPriority() << std::endl;
//	std::cout << " sub2 prio = " << sub2.getPriority() << std::endl;
//
//	std::cout << "priority info" << std::endl;
//	root.error("root error");
//	root.warn("root warn");
//	sub1.error("sub1 error");
//	sub1.warn("sub1 warn");
//	sub2.error("sub2 error");
//	sub2.warn("sub2 warn");
//
//	sub2.warnStream() << "streamed warn";
//
//	sub2 << log4cpp::Priority::WARN << "warn2" << " warn3" 
//             << log4cpp::eol << " warn4";
//
//	{
//		for(int i = 0; i < 10000; i++) {
//			char ndc2[20];
//			sprintf(ndc2, "i=%d", i);
//			log4cpp::NDC::push(ndc2);
//			sub1.info("%s%d", "i = ", i);
//			if ((i % 10) == 0) {
//				sub1.log(log4cpp::Priority::NOTICE, "reopen log");
//				if (log4cpp::Appender::reopenAll()) {
//					sub1.info("log reopened");
//				} else {
//					sub1.warn("could not reopen log");
//				}
//			}
//#ifndef WIN32
//			sleep(1);
//#endif
//			log4cpp::NDC::pop();
//		}
//	}
//
//	return 0;
//}
//#include <iostream>
//#include "rope"
//using namespace std;
//int main()
//{
//	crope bigstr1("dasfasdfasdfa");
//	crope bigstr2("fdasfasdfasdfa");
//	crope story = bigstr1 + bigstr2;
//	cout << story << endl;
//	return 0;
//}
#include "Algorythms.h"
#include "System.h"
#include "Reflection.h"



/*****************************************************************************
    MAIN
 *****************************************************************************/


#include <list>
#include <string>
#include <iostream>
using namespace std;
using namespace agm::reflection;


void print_class(const Class &pclass)
{
    cout << "class name = " << pclass.getName() << endl;

    cout << "superclass = ";
    if (pclass.hasSuper()) cout << pclass.getSuper().getName();
    else cout << "none";
    cout << endl;

    cout << "fields:\n";
    for(Class::FieldList::const_iterator itField = pclass.getFields().begin();
        itField != pclass.getFields().end();
        ++itField)
    {
        const Field &field = *itField;
        cout << "    " << field.getAccess() << " " << field.getType() << " " << field.getName() << endl;
    }

    cout << "static fields:\n";
    for(Class::StaticFieldList::const_iterator itStaticField = pclass.getStaticFields().begin();
        itStaticField != pclass.getStaticFields().end();
        ++itStaticField)
    {
        const StaticField &field = *itStaticField;
        cout << "    " << field.getAccess() << " " << field.getType() << " " << field.getName() << endl;
    }

    cout << "methods:\n";
    for(Class::MethodList::const_iterator itMethod = pclass.getMethods().begin();
        itMethod != pclass.getMethods().end();
        ++itMethod)
    {
        const Method &method = *itMethod;
        cout << "    " << method.getAccess();
        if (method.isVirtual()) cout << " " << "virtual";
        cout << " " << method.getType() << " " << method.getName() << method.getArgs() << endl;
    }

    cout << "properties:\n";
    for(Class::PropertyList::const_iterator itProperty = pclass.getProperties().begin();
        itProperty != pclass.getProperties().end();
        ++itProperty)
    {
        const Property &property = *itProperty;
        cout << "    " << property.getType() << " " << property.getName() << endl;
    }

    cout << "\n-----------------------------------------------------------\n";
}


class Foo {
public:
    CLASS(Foo, agm::reflection::NullClass);

    PROPERTY(int, length);

    METHOD(public, int, action, ()) {
        cout << "Foo::action();\n";
		return 0;
    }

    METHOD(public, void, action1, (int i)) {
        cout << "Foo::action1(" << i << ");\n";
    }

    STATIC_METHOD(public, int, get_code, ()) {
        cout << "Foo::get_code();\n";
        return 1;
    }

    STATIC_METHOD(public, int, get_code1, (int i)) {
        cout << "Foo::get_code1(" << i << ");\n";
        return i + 1;
    }

    STATIC_METHOD(public, void, print_code, ()) {
        cout << "Foo::print_code();\n";
    }

    STATIC_METHOD(public, void, print_code1, (int i)) {
        cout << "Foo::print_code1(" << i << ");\n";
    }

    Foo() {
        m_length = 0;
    }

private:
    int m_length;

    int get_length() const {
        cout << "get_length();\n";
        return m_length;
    }

    void set_length(int l) {
        cout << "set_length(" << l << ");\n";
        m_length = l;
    }
};


class Bar {
public:
    CLASS(Bar, agm::reflection::NullClass);
};

#include "libxml.h" 
int main()
{
	TiXmlDocument doc;
	
    Foo foo1;
    const Class &foo_class = foo1.getClass();
    print_class(foo_class);
	int x;
	foo1.getClass().getMethod("action").invoke(x,&foo1);
	foo_class.getMethod("action1").invokeVoid(&foo1,3);
    Algorythms::Template::Graphic G = Algorythms::Template::Graphic();
	G.AddEdge(0,1,4);
	G.AddEdge(0,7,8);
	G.AddEdge(1,2,8);
	G.AddEdge(1,7,11);
	G.AddEdge(2,3,7);
	G.AddEdge(2,5,4);
	G.AddEdge(2,8,2);
	G.AddEdge(3,4,9);
	G.AddEdge(3,5,14);
	G.AddEdge(4,5,10);
	G.AddEdge(5,6,2);
	G.AddEdge(6,8,6);
	G.AddEdge(6,7,1);
	G.AddEdge(7,8,7);
	G.AddNodes(9);
	Algorythms::Template::Graphic mst = Algorythms::Template::MST_KRUSKAL(G);

	System::pause();
    return 0;
}

