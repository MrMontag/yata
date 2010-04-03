#include "TestObject.h"
#include "TestRegister.h"

TestObject::TestObject()
{
	TestRegister::instance()->registerTest(this);
}
