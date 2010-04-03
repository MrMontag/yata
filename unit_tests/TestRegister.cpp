#include "TestRegister.h"
#include "TestObject.h"
#include <QTest>
#include <iostream>

void TestRegister::registerTest(TestObject * test)
{
	m_tests.push_back(test);
}

int TestRegister::executeTests(int argc, char** argv)
{
	int results = 0;
	for(TestContainer::iterator itr = m_tests.begin(); itr != m_tests.end(); ++itr) {
		results += QTest::qExec(*itr, argc, argv);
	}
	std::cout << "Overall result: ";
	if(results) {
		std::cout << "FAIL\n";
	} else {
		std::cout << "SUCCESS\n";
	}
	return results;
}

TestRegister * TestRegister::instance()
{
	static TestRegister test_register;
	return &test_register;
}

TestRegister::TestRegister()
{
}
