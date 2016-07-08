#ifndef CALCUL_H
#define CALCUL_H

#include<string>
#include<vector>
#include "xPLLib/Schemas/SchemaControl.h"
#include "CalculParser.h"

class Calcul
{
    public:
        Calcul();
        Calcul(const std::string& name, const std::string& device, const std::string& type, const std::string& formula);
        ~Calcul();
        void SetCalcul(const std::string& device, const std::string& type, const std::string& formula);

        std::string m_Name;
        std::string m_CanonicDevice;
        std::string m_DeviceName;
        std::string m_ModuleName;
        std::string m_TypeStr;
        xPL::SchemaControlBasic::controlType m_TypeEnum;
        std::string m_Formula;
        std::string m_Value;
        std::vector<CalculParser*> m_CalculParses;
};

#endif // CALCUL_H
