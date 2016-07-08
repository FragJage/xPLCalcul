#ifndef XPLCALCUL_H
#define XPLCALCUL_H

#include "Service/Service.h"
#include "xPLLib/xPLDevCfg.h"
#include "xPLLib/Extensions/AdvanceConfig.h"
#include "Calcul.h"
#include "CacheDevice.h"

class xPLCalcul : public Service::IService, public xPL::xPLDevice::IExtension, public xPL::AdvanceConfig::ICallBack
{
    public:
        xPLCalcul();
        ~xPLCalcul();

        void AdvanceConfigure();
        bool MsgAnswer(xPL::SchemaObject& msg);
        void ConfigChange(const std::string& device);
        void ConfigDelete(const std::string& device);
        void ConfigsDelete();

		int ServiceStart(int argc, char* argv[]);
		void ServicePause(bool bPause);
		void ServiceStop();

    private:
        int CalculFind(const std::string& calculName);
        void CalculParse(Calcul* calcul);
        void CalculUpdate(const std::string& device, const std::string& current);
        double CalculDoOper (double G, char op, double D);
        int CalculIsOper(char op);
        unsigned int CalculPriorite (char op);
        double CalculOperat(const char *calcul, double *G_dl, char *Op_dl, unsigned int prio_op_pre, const char **p_calcul);
        void CalculEvaluate(Calcul* calcul);

        void CacheAdd(const std::string& device, Calcul* calcul);
        void CacheUpdate(const std::string& device, const std::string& current, xPL::ISchema::MsgType msgType);
        bool CacheGet(const std::string& device, double* current);

        void RequestValue(const std::string& device);
        void SetValue(Calcul *calcul, double value);

        SimpleLog* m_Log;
        xPL::xPLDevCfg m_xPLDevice;
        xPL::AdvanceConfig m_AdvanceConfig;
        bool m_bServicePause;
        bool m_bServiceStop;
        std::vector<Calcul*> m_Calculs;
        std::map<std::string,CacheDevice*> m_CacheDevices;
};

#endif // XPLCALCUL_H
