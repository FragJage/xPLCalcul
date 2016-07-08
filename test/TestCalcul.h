#include <cassert>
#include "UnitTest/UnitTest.h"

using namespace std;

class TestCalcul : public TestClass<TestCalcul>
{
public:
    TestCalcul();
    ~TestCalcul();

    bool SetCalcul();
    bool SetCache();
    bool SetParser();
};
