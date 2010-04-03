#ifndef TESTREGISTER_H
#define TESTREGISTER_H

#include <vector>

class TestObject;

class TestRegister {
public:
	void registerTest(TestObject * test);
	int executeTests(int argc, char** argv);

	static TestRegister * instance();
private:
	TestRegister();

	typedef std::vector<TestObject*> TestContainer;
	TestContainer m_tests;
};

#define REGISTER_TEST(T) \
	static T T##instance;
 
#endif
