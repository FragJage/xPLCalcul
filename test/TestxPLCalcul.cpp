#include "TestxPLCalcul.h"
#include "Plateforms/Plateforms.h"
#include "xPLLib/Schemas/SchemaObject.h"
#include "../src/Calcul.h"
#include "../src/CacheDevice.h"
#include "../src/CalculParser.h"

using namespace std;

TestxPLCalcul::TestxPLCalcul() : TestClass("Counter", this)
{
	addTest("Start", &TestxPLCalcul::Start);
	addTest("StdConfig", &TestxPLCalcul::StdConfig);
	addTest("SetAdvConfig", &TestxPLCalcul::SetAdvConfig);
	addTest("CalculationInferior", &TestxPLCalcul::CalculationInferior);
	addTest("CalculationSuperior", &TestxPLCalcul::CalculationSuperior);
	addTest("CalculationMultiplication", &TestxPLCalcul::CalculationMultiplication);
	addTest("CalculationDivision", &TestxPLCalcul::CalculationDivision);
	addTest("CalculationAddition", &TestxPLCalcul::CalculationAddition);
	addTest("CalculationSubtraction", &TestxPLCalcul::CalculationSubtraction);
    addTest("CalculationAnd", &TestxPLCalcul::CalculationAnd);
    addTest("CalculationOr", &TestxPLCalcul::CalculationOr);
    addTest("DelAdvConfig", &TestxPLCalcul::DelAdvConfig);
	addTest("Stop", &TestxPLCalcul::Stop);
}

TestxPLCalcul::~TestxPLCalcul()
{
}

void TestxPLCalcul::ThreadStart(xPLCalcul* pxPLDev)
{
    char exeName[] = "test";
    char confName[] = "config";
    char* argv[2];

    argv[0]= exeName;
    argv[1]= confName;
    pxPLDev->ServiceStart(2, argv);
}

bool TestxPLCalcul::Start()
{
    string msg;
    xPL::SchemaObject sch;

    remove("config");

    thread integrationTest(ThreadStart, &xPLDev);
    integrationTest.detach();

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("xPL Calcul"==sch.GetValue("appname"));

    return true;
}

bool TestxPLCalcul::StdConfig()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schCfg(xPL::SchemaObject::cmnd, "config", "response");

    schCfg.SetValue("interval", "25");
    schCfg.SetValue("newconf", "test");
    msg = schCfg.ToMessage("fragxpl-test.default", "fragxpl-calcul.default");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);     //Pass Hbeat message
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("25"==sch.GetValue("interval"));
    assert("fragxpl-calcul.test"==sch.GetSource());

    return true;
}

bool TestxPLCalcul::SetAdvConfig()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schAdvCfg(xPL::ISchema::cmnd, "advanceconfig", "request");

    schAdvCfg.SetValue("configname", "EvaluateNight");
    schAdvCfg.SetValue("output", "fragxpl-fictif.default:night");
    schAdvCfg.SetValue("type", "output");
    schAdvCfg.SetValue("formula", "fragxpl-owfs.default:brightness<10");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-calcul.test");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("fragxpl-owfs.default"==sch.TargetAddress.ToString());
    assert("sensor"==sch.GetClass());
    assert("request"==sch.GetType());
    assert("current"==sch.GetValue("request"));
    assert("brightness"==sch.GetValue("device"));

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("fragxpl-fictif.default"==sch.TargetAddress.ToString());
    assert("sensor"==sch.GetClass());
    assert("request"==sch.GetType());
    assert("current"==sch.GetValue("request"));
    assert("night"==sch.GetValue("device"));

    return true;
}

void TestxPLCalcul::SetCalculation(string name, string device, string type, string formula)
{
    string msg;
    xPL::SchemaObject schAdvCfg(xPL::ISchema::cmnd, "advanceconfig", "request");

    schAdvCfg.SetValue("configname", name);
    schAdvCfg.SetValue("output", device);
    schAdvCfg.SetValue("type", type);
    schAdvCfg.SetValue("formula", formula);
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-calcul.test");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);     //Pass output device request
    msg = ControlSockMock::GetLastSend(10);     //Pass formula device request
}

void TestxPLCalcul::SetDeviceValue(string device, string current, string type, string source)
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schSensor(xPL::ISchema::trig, "sensor", "basic");


    schSensor.SetValue("device", device);
    schSensor.SetValue("current", current);
    schSensor.SetValue("type", type);
    msg = schSensor.ToMessage(source, "*");
    ControlSockMock::SetNextRecv(msg);
}

bool TestxPLCalcul::CalculationInferior()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schSensor(xPL::ISchema::trig, "sensor", "basic");


    SetDeviceValue("night", "LOW", "output", "fragxpl-fictif.default");
    Plateforms::delay(100);

    SetDeviceValue("brightness", "9", "illuminance", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("control"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("night"==sch.GetValue("device"));
    assert("high"==sch.GetValue("current"));

    SetDeviceValue("brightness", "11", "illuminance", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("low"==sch.GetValue("current"));

    return true;
}

bool TestxPLCalcul::CalculationSuperior()
{
    string msg;
    xPL::SchemaObject sch;

    SetCalculation("SuperiorTest", "fragxpl-owfs.default:hotflag", "flag", "fragxpl-owfs.default:temperature>50");
    SetDeviceValue("hotflag", "clear", "flag", "fragxpl-owfs.default");
    Plateforms::delay(100);

    SetDeviceValue("temperature", "51", "temp", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("control"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("hotflag"==sch.GetValue("device"));
    assert("set"==sch.GetValue("current"));

    SetDeviceValue("temperature", "49", "temp", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("neutral"==sch.GetValue("current"));

    return true;
}

bool TestxPLCalcul::CalculationMultiplication()
{
    string msg;
    xPL::SchemaObject sch;

    SetCalculation("MultiplicationTest", "fragxpl-owfs.default:urgency", "input", "fragxpl-owfs.default:stress*2");
    SetDeviceValue("urgency", "disable", "input", "fragxpl-owfs.default");
    Plateforms::delay(100);

    SetDeviceValue("stress", "5", "temp", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("control"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("urgency"==sch.GetValue("device"));
    assert("enable"==sch.GetValue("current"));

    SetDeviceValue("stress", "-1", "temp", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("disable"==sch.GetValue("current"));

    return true;
}

bool TestxPLCalcul::CalculationDivision()
{
    string msg;
    xPL::SchemaObject sch;

    SetCalculation("DivisionTest", "fragxpl-owfs.default:macro", "macro", "fragxpl-owfs.default:tempmacro/2");
    SetDeviceValue("macro", "disable", "macro", "fragxpl-owfs.default");
    Plateforms::delay(100);

    SetDeviceValue("tempmacro", "5", "temp", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("control"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("macro"==sch.GetValue("device"));
    assert("enable"==sch.GetValue("current"));

    SetDeviceValue("tempmacro", "-1", "temp", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("disable"==sch.GetValue("current"));

    SetDeviceValue("tempmacro", "0", "temp", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("do"==sch.GetValue("current"));

    return true;
}

bool TestxPLCalcul::CalculationAddition()
{
    string msg;
    xPL::SchemaObject sch;


    SetCalculation("OperationTest", "fragxpl-owfs.default:mute", "mute", "fragxpl-owfs.default:speed+10");
    SetDeviceValue("mute", "no", "mute", "fragxpl-owfs.default");
    Plateforms::delay(100);

    SetDeviceValue("speed", "5", "speed", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("control"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("mute"==sch.GetValue("device"));
    assert("yes"==sch.GetValue("current"));

    SetDeviceValue("speed", "-11", "temp", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("no"==sch.GetValue("current"));

    return true;
}

bool TestxPLCalcul::CalculationSubtraction()
{
    string msg;
    xPL::SchemaObject sch;

    SetCalculation("OperationTest", "fragxpl-owfs.default:periodic", "periodic", "fragxpl-owfs.default:value-2");
    SetDeviceValue("periodic", "disable", "periodic", "fragxpl-owfs.default");
    Plateforms::delay(100);

    SetDeviceValue("value", "30", "generic", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("control"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("periodic"==sch.GetValue("device"));
    assert("enable"==sch.GetValue("current"));

    SetDeviceValue("value", "2", "generic", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("started"==sch.GetValue("current"));

    SetDeviceValue("value", "1", "generic", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("disable"==sch.GetValue("current"));

    return true;
}

bool TestxPLCalcul::CalculationAnd()
{
    string msg;
    xPL::SchemaObject sch;

    SetCalculation("OperatorTest", "fragxpl-owfs.default:sched", "scheduled", "fragxpl-owfs.default:startone&fragxpl-owfs.default:starttwo");
    msg = ControlSockMock::GetLastSend(10);     //Pass the second request device value
    SetDeviceValue("sched", "disable", "scheduled", "fragxpl-owfs.default");
    Plateforms::delay(100);

    SetDeviceValue("startone", "high", "output", "fragxpl-owfs.default");
    Plateforms::delay(500);
    SetDeviceValue("starttwo", "high", "output", "fragxpl-owfs.default");
    Plateforms::delay(500);
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("control"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("sched"==sch.GetValue("device"));
    assert("enable"==sch.GetValue("current"));

    SetDeviceValue("startone", "low", "output", "fragxpl-owfs.default");
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("started"==sch.GetValue("current"));

    return true;
}

bool TestxPLCalcul::CalculationOr()
{
    //|   timer
    string msg;
    xPL::SchemaObject sch;

    SetCalculation("OperatorTest", "fragxpl-owfs.default:timer", "timer", "fragxpl-owfs.default:devalpha&fragxpl-owfs.default:devbeta");
    msg = ControlSockMock::GetLastSend(10);     //Pass the second request device value
    SetDeviceValue("timer", "stop", "timer", "fragxpl-owfs.default");
    Plateforms::delay(100);

    SetDeviceValue("devalpha", "high", "output", "fragxpl-owfs.default");
    Plateforms::delay(500);
    SetDeviceValue("devbeta", "high", "output", "fragxpl-owfs.default");
    Plateforms::delay(500);
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("control"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("timer"==sch.GetValue("device"));
    assert("start"==sch.GetValue("current"));

    SetDeviceValue("devalpha", "low", "output", "fragxpl-owfs.default");
    Plateforms::delay(500);
    SetDeviceValue("devbeta", "low", "output", "fragxpl-owfs.default");
    Plateforms::delay(500);
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("stop"==sch.GetValue("current"));

    return true;
}

bool TestxPLCalcul::DelAdvConfig()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schAdvCfg(xPL::ISchema::cmnd, "advanceconfig", "current");

    schAdvCfg.SetValue("command", "delete");
    schAdvCfg.SetValue("configname", "EvaluateNight");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-calcul.test");
    ControlSockMock::SetNextRecv(msg);
    Plateforms::delay(500);

    schAdvCfg.ClearValues();
    schAdvCfg.SetValue("command", "delete");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-calcul.test");
    ControlSockMock::SetNextRecv(msg);
    Plateforms::delay(500);

    return true;
}

bool TestxPLCalcul::Stop()
{
    string msg;

    xPLDev.ServicePause(true);
    xPLDev.ServicePause(false);
    xPLDev.ServiceStop();

    msg = ControlSockMock::GetLastSend(10);     //Pass hbeat message
    Plateforms::delay(200);
    return true;
}
