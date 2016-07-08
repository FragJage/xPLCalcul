#include "UnitTest/UnitTest.h"
#include "TestCalcul.h"

using namespace std;

int main()
{
    UnitTest unitTest;

	unitTest.addTestClass(new TestCalcul());
	if(!unitTest.run()) return 1;
	return 0;
}
