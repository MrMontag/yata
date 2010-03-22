#include <QTest>
#include <iostream>
#include "HtmlConverterTest.h"
#include "TailViewTest.h"

class Tester {
public:
	Tester(int argc_, char ** argv_)
		: m_result(0)
		, argc(argc_)
		, argv(argv_)
	{}

	template <typename TestClass>
	void execTest()
	{
		TestClass * testClass = new TestClass;
		m_result += QTest::qExec(testClass, argc, argv);
		delete testClass;
	}

	int m_result;
	int argc;
	char ** argv;
};

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	Tester tester(argc, argv);
	tester.execTest<HtmlConverterTest>();
	tester.execTest<TailViewTest>();
	
	std::cout << "Overall result: ";
	if(tester.m_result) {
		std::cout << "FAIL\n";
	} else {
		std::cout << "SUCCESS\n";
	}
	

	return tester.m_result;
}

