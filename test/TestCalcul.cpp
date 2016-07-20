#include "TestCalcul.h"
#include "xPLLib/Schemas/SchemaControl.h"
#include "../src/Calcul.h"
#include "../src/CacheDevice.h"
#include "../src/CalculParser.h"

using namespace std;

TestCalcul::TestCalcul() : TestClass("Counter", this)
{
	addTest("SetCalcul", &TestCalcul::SetCalcul);
	addTest("SetCache", &TestCalcul::SetCache);
	addTest("SetParser", &TestCalcul::SetParser);
}

TestCalcul::~TestCalcul()
{
}

bool TestCalcul::SetCalcul()
{
    Calcul myCalculSimple;
    Calcul myCalcul("EvaluateNight", "fragxpl-fictif.default:night", "output", "fragxpl-owfs.default:brightness<10");

    assert("", myCalculSimple.m_Name);
    assert("EvaluateNight"==myCalcul.m_Name);
    assert("fragxpl-fictif.default:night"==myCalcul.m_CanonicDevice);
    assert("output"==myCalcul.m_TypeStr);
    assert("fragxpl-owfs.default:brightness<10"==myCalcul.m_Formula);
    assert("fragxpl-fictif.default"==myCalcul.m_ModuleName);
    assert("night"==myCalcul.m_DeviceName);
    assert(xPL::SchemaControlBasic::output==myCalcul.m_TypeEnum);

    return true;
}

bool TestCalcul::SetCache()
{
    Calcul myCalcul("EvaluateNight", "fragxpl-fictif.default:night", "output", "fragxpl-owfs.default:brightness<10");
    CacheDevice cache1;
    CacheDevice cache2(&myCalcul);

    assert(false==cache1.m_Lu);
    assert(false==cache2.m_Lu);
    return true;
}

bool TestCalcul::SetParser()
{
    CalculParser parser1;
    CalculParser parser2("fragxpl-fictif.default:night", 1, 31);


    assert(""==parser1.m_Device);
    assert("fragxpl-fictif.default:night"==parser2.m_Device);
    return true;
}
