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
    Calcul myCalcul("EvaluateNight", "fragxpl-fictif.default:night", "output", "fragxpl-owfs.default:brightness<10");

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
    CacheDevice cache1();
    CacheDevice cache2(&myCalcul);
    return true;
}

bool TestCalcul::SetParser()
{
    CalculParser parser1();
    CalculParser parser2("fragxpl-fictif.default:night", 1, 31);
    return true;
}
