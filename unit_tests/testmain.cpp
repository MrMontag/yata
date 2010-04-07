#include <QApplication>
#include "TestRegister.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	return TestRegister::instance()->executeTests(argc, argv);
}

