#include <cassert>
#include "UnitTest/UnitTest.h"
#ifdef WIN32
    #include "Thread/mingw.thread.h"
#else
    #include <thread>
#endif
#include "../src/xPLCalcul.h"
#include "ControlSockMock.h"

using namespace std;

class TestxPLCalcul : public TestClass<TestxPLCalcul>
{
public:
    TestxPLCalcul();
    ~TestxPLCalcul();

    static void ThreadStart(xPLCalcul* pxPLDev);
    bool Start();
    bool StdConfig();
    bool SetAdvConfig();
    bool CalculationInferior();
    bool CalculationSuperior();
    bool CalculationMultiplication();
    bool CalculationDivision();
    bool CalculationAddition();
    bool CalculationSubtraction();
    bool CalculationAnd();
    bool CalculationOr();
    bool DelAdvConfig();
    bool Stop();
    xPLCalcul xPLDev;
private:
    void SetCalculation(string name, string device, string type, string formula);
    void SetDeviceValue(string device, string current, string type, string source);
};
