#include <algorithm>
#include <stdexcept>
#include "Calcul.h"

using namespace std;

Calcul::Calcul()
{
    m_Name = "";
}

Calcul::Calcul(const string& name, const string& device, const string& type, const string& formula)
{
    m_Name = name;
    SetCalcul(device, type, formula);
}

Calcul::~Calcul()
{
}

void Calcul::SetCalcul(const string& device, const string& type, const string& formula)
{
    size_t pos;


    m_CanonicDevice = device;
    m_TypeStr = type;
    m_Formula = formula;

    m_Formula.erase(std::remove(m_Formula.begin(), m_Formula.end(), ' '), m_Formula.end());
    pos = m_CanonicDevice.find(":");
    if(pos==string::npos) throw logic_error("The device name must be in the form vendor-module.instance:device");
    m_ModuleName = m_CanonicDevice.substr(0, pos);
    m_DeviceName = m_CanonicDevice.substr(pos+1);
    m_TypeEnum = xPL::SchemaControlBasic::ToDeviceType(m_TypeStr);
}
