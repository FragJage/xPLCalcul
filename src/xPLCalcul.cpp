#include <cstdlib>
#include "Plateforms/Plateforms.h"
#include "xPLLib/xPLDevice.h"
#include "xPLLib/Schemas/SchemaSensor.h"
#include "xPLLib/Schemas/SchemaControl.h"
#include "xPLCalcul.h"


using namespace std;

xPLCalcul::xPLCalcul() : m_AdvanceConfig(&m_xPLDevice)
{
    m_Log = m_xPLDevice.GetLogHandle();

    m_AdvanceConfig.AddFormat("configname", xPL::AdvanceConfig::ParamType::STRING, xPL::AdvanceConfig::ParamList::NONE);
    m_AdvanceConfig.AddFormat("output", xPL::AdvanceConfig::ParamType::DEVICE, xPL::AdvanceConfig::ParamList::NONE);
    m_AdvanceConfig.AddFormat("type", xPL::AdvanceConfig::ParamType::STRING, xPL::AdvanceConfig::ParamList::CONTROLTYPE);
    m_AdvanceConfig.AddFormat("formula", xPL::AdvanceConfig::ParamType::STRING, xPL::AdvanceConfig::ParamList::NONE);

    m_AdvanceConfig.SetCallBack(this);
    m_xPLDevice.AddExtension(&m_AdvanceConfig);
    m_xPLDevice.AddExtension(this);

    m_xPLDevice.Initialisation("fragxpl", "calcul", "default");
    m_xPLDevice.SetAppName("xPL Calcul", "1.1.0.0");
    m_xPLDevice.SetAnswerAllMsg(true);
    m_bServicePause = false;
    m_bServiceStop = false;
}

xPLCalcul::~xPLCalcul()
{
    ConfigsDelete();
}

void xPLCalcul::ConfigsDelete()
{
    vector<Calcul*>::iterator itCalcul;
    vector<CalculParser*>::iterator itParse;
    std::map<std::string,CacheDevice*>::iterator itCache;


    for(itCalcul=m_Calculs.begin(); itCalcul!=m_Calculs.end(); ++itCalcul)
	{
	    for(itParse=(*itCalcul)->m_CalculParses.begin(); itParse!=(*itCalcul)->m_CalculParses.end(); ++itParse)
            delete *itParse;
        (*itCalcul)->m_CalculParses.clear();

		delete *itCalcul;
	}
	m_Calculs.clear();

    for(itCache=m_CacheDevices.begin(); itCache!=m_CacheDevices.end(); ++itCache)
		delete itCache->second;
	m_CacheDevices.clear();
}

void xPLCalcul::ConfigDelete(const std::string& configName)
{
    int iCalcul, i, nb;
    Calcul* pCalcul;
    vector<CalculParser*>::iterator itParse;
    map<std::string,CacheDevice*>::iterator itCache;


    iCalcul = CalculFind(configName);
    if(iCalcul==-1) return;

    pCalcul = m_Calculs[iCalcul];
    for(itParse=pCalcul->m_CalculParses.begin(); itParse!=pCalcul->m_CalculParses.end(); ++itParse)
        delete *itParse;
    pCalcul->m_CalculParses.clear();

    delete pCalcul;
    m_Calculs.erase(m_Calculs.begin()+iCalcul);

    for(itCache=m_CacheDevices.begin(); itCache!=m_CacheDevices.end(); ++itCache)
    {
        nb = itCache->second->m_Calculs.size();
        for(i=0; i<nb; i++)
        {
            if(itCache->second->m_Calculs[i]==pCalcul)
            {
                itCache->second->m_Calculs.erase(itCache->second->m_Calculs.begin()+i);
                break;
            }
        }
    }
}

void xPLCalcul::ConfigChange(const std::string& configName)
{
    int iCalcul;
    Calcul *pCalcul;
    map<string, string>* config;
    string output;
    string type;
    string formula;


    config = m_AdvanceConfig.GetConfig(configName);
    output  = (*config)["output"];
    type    = (*config)["type"];
    formula = (*config)["formula"];

    iCalcul = CalculFind(configName);
    if(iCalcul>0) ConfigDelete(configName);

    pCalcul = new Calcul(configName, output, type, formula);
    m_Calculs.push_back(pCalcul);

    CalculParse(pCalcul);
}

int xPLCalcul::CalculFind(const std::string& calculName)
{
    int iCalcul, maxCalcul;


    maxCalcul = m_Calculs.size();
    for(iCalcul=0; iCalcul<maxCalcul; iCalcul++)
    {
        if(m_Calculs[iCalcul]->m_Name==calculName) return iCalcul;
    }

    return -1;
}

void xPLCalcul::RequestValue(const string& longDevice)
{
    xPL::SchemaSensorRequest sensorRequest;
    string module;
    string shortDevice;
    size_t pos;


    pos = longDevice.find(":");
    module = longDevice.substr(0, pos);
    shortDevice = longDevice.substr(pos+1);
    sensorRequest.SetDeviceName(shortDevice);
    m_xPLDevice.SendMessage(&sensorRequest, module);
}

void xPLCalcul::SetValue(Calcul *calcul, double value)
{
    xPL::SchemaControlBasic sendCmnd;
    string strValue;


  	LOG_ENTER;

    switch(calcul->m_TypeEnum)
    {
        case xPL::SchemaControlBasic::controlType::flag :
            if(value>0) strValue = "set";
            if(value<0) strValue = "clear";
            if(value==0) strValue = "neutral";
            break;
        case xPL::SchemaControlBasic::controlType::input :
            if(value>0) strValue = "enable";
            if(value<=0) strValue = "disable";
            break;
        case xPL::SchemaControlBasic::controlType::macro :
            if(value>0) strValue = "enable";
            if(value<0) strValue = "disable";
            if(value==0) strValue = "do";
            break;
        case xPL::SchemaControlBasic::controlType::mute :
            if(value>0) strValue = "yes";
            if(value<=0) strValue = "no";
            break;
        case xPL::SchemaControlBasic::controlType::output :
            if(value>0) strValue = "high";
            if(value<=0) strValue = "low";
            break;
        case xPL::SchemaControlBasic::controlType::periodic :
            if(value>0) strValue = "enable";
            if(value<0) strValue = "disable";
            if(value==0) strValue = "started";
            break;
        case xPL::SchemaControlBasic::controlType::scheduled :
            if(value>0) strValue = "enable";
            if(value<0) strValue = "disable";
            if(value==0) strValue = "started";
            break;
        case xPL::SchemaControlBasic::controlType::timer :
            if(value>0) strValue = "start";
            if(value<=0) strValue = "stop";
            break;
        default :
            std::ostringstream stm ;
            stm << value;
            strValue = stm.str();
    }

	if(calcul->m_Value == strValue)
	{
		LOG_VERBOSE(m_Log) << "The value remains unchanged";
        LOG_EXIT_OK;
		return;
	}

	LOG_VERBOSE(m_Log) << "Fix the device " << calcul->m_CanonicDevice << " to " << value;
	calcul->m_Value = strValue;

    if((calcul->m_TypeEnum!=xPL::SchemaControlBasic::controlType::manual)&&
       (calcul->m_TypeEnum!=xPL::SchemaControlBasic::controlType::unset))
        sendCmnd.SetDeviceType(calcul->m_TypeStr);
    sendCmnd.SetDeviceName(calcul->m_DeviceName);
    sendCmnd.SetCurrent(strValue);
    LOG_VERBOSE(m_Log) << "Send " << strValue << " to " << calcul->m_ModuleName << " device " << calcul->m_DeviceName << " (type " << calcul->m_TypeStr << ")";

    m_xPLDevice.SendMessage(&sendCmnd, calcul->m_ModuleName);
   	LOG_EXIT_OK;
}

void xPLCalcul::CacheAdd(const string& device, Calcul* calcul)
{
    std::map<std::string,CacheDevice*>::iterator it;


    it = m_CacheDevices.find(device);
    if(it!=m_CacheDevices.end())
    {
        it->second->m_Calculs.push_back(calcul);
        return;
    }
    m_CacheDevices[device] = new CacheDevice(calcul);

    RequestValue(device);
}

void xPLCalcul::CacheUpdate(const string& device, const string& current, xPL::ISchema::MsgType msgType)
{
    std::map<std::string,CacheDevice*>::iterator it;
    std::vector<Calcul*>::iterator itCalcul;
    bool oldLu;
    double value;


  	LOG_ENTER;

    LOG_VERBOSE(m_Log) << "Device " << device << " into the cache ?";
    it = m_CacheDevices.find(device);
    if(it==m_CacheDevices.end())
    {
        LOG_VERBOSE(m_Log) << "No, thanks";
        LOG_EXIT_OK;
        return;
    }

    value = atof(current.c_str());
    if(current=="set") value = 1;
    if(current=="clear") value = 0;
    if(current=="enable") value = 1;
    if(current=="disable") value = 0;
    if(current=="yes") value = 1;
    if(current=="no") value = 0;
    if(current=="high") value = 1;
    if(current=="low") value = 0;
    if(current=="start") value = 1;
    if(current=="stop") value = 0;

    LOG_VERBOSE(m_Log) << device << "=" << value << " in the cache";
    oldLu = it->second->m_Lu;
    it->second->m_Current = value;
    it->second->m_Lu = true;

    if((msgType==xPL::SchemaObject::stat)&&(oldLu==true))
    {
        LOG_VERBOSE(m_Log) << "Stat message -> no evaluations";
        LOG_EXIT_OK;
        return;
    }

    for(itCalcul=it->second->m_Calculs.begin(); itCalcul!=it->second->m_Calculs.end(); ++itCalcul)
    {
        CalculEvaluate(*itCalcul);
    }

    LOG_EXIT_OK;
}

bool xPLCalcul::CacheGet(const string& device, double* current)
{
    std::map<std::string,CacheDevice*>::iterator it;

    it = m_CacheDevices.find(device);
    if(it==m_CacheDevices.end()) return false;
    if(it->second->m_Lu==false) return false;

    *current = it->second->m_Current;
    return true;
}

double xPLCalcul::CalculDoOper (double G, char op, double D)
{
  double resultat = 0;

  switch (op)
  {
	  case '+':
		  resultat = G + D;
		  break;
	  case '-':
		  resultat = G - D;
		  break;
	  case '*':
		  resultat = G * D;
		  break;
	  case '/':
		  resultat = G / D;
		  break;
	  case '|':
		  resultat = (int)G | (int)D;
		  break;
	  case '&':
		  resultat = (int)G & (int)D;
		  break;
	  case '<':
		  if(G < D)
			  return 1;
		  else
			  return 0;
		  break;
	  case '>':
		  if(G > D)
			  return 1;
		  else
			  return 0;
		  break;
	  default:
		  resultat = 0;
  }

  return resultat;
}

int xPLCalcul::CalculIsOper(char op)
{
  switch (op)
  {
	  case '+':
	  case '-':
	  case '*':
	  case '/':
	  case '|':
	  case '&':
	  case '>':
	  case '<':
		  return 1;
	  default:
		  return 0;
  }
}

unsigned int xPLCalcul::CalculPriorite(char op)
{
  unsigned int prio = 0;

  switch (op)
  {
	  case '+':
	  case '-':
		  prio = 1;
		  break;
	  case '*':
	  case '/':
		  prio = 2;
		  break;
	  case '|':
	  case '&':
	  case '>':
	  case '<':
		  prio = 3;
		  break;
  }

  return prio;
}

double xPLCalcul::CalculOperat(const char *calcul, double *G_dl, char *Op_dl,
                  unsigned int prio_op_pre, const char **p_calcul)
{
  double G, D;                /* opérande de gauche et opérande de droite */
  char op, op_suivant;

  /* *** initialisations *** */
  /* Si G_dl est un pointeur valide */
  if (G_dl != NULL)
  {
      G = *G_dl;
  }
  else
  {                            /* Sinon on lit G */
      if (*calcul == '(')
        G = CalculOperat(calcul + 1, NULL, NULL, 0, &calcul);
      else
        G = strtod(calcul, (char **) &calcul);
  }

  /* Si Op_dl est un pointeur valide */
  if (Op_dl != NULL)
  {
      op = *Op_dl;
  }
  else
  {                            /* Sinon on lit l'opérateur */
      op = *calcul;
      calcul++;
  }

  /* *** boucle des calculs *** */
  while (op != '\0' && op != ')' && CalculPriorite(op) > prio_op_pre)
  {
      /* Lecture de l'opérande de droite */
      if (*calcul == '(')
        D = CalculOperat(calcul + 1, NULL, NULL, 0, &calcul);
      else
        D = strtod(calcul, (char **) &calcul);

      /* Opérateur suivant */
      op_suivant = *calcul;
      calcul++;

      if (CalculIsOper(op_suivant) && CalculPriorite(op_suivant) > CalculPriorite(op))
      {
        D = CalculOperat(calcul, &D, &op_suivant, CalculPriorite(op), &calcul);
      }

      G = CalculDoOper(G, op, D);
      op = op_suivant;
  }
  /* *** fin de la boucle des calculs *** */

  /* Mise à jour de l'opérateur suivant pour la fonction appelante */
  if (Op_dl != NULL)
      *Op_dl = op_suivant;

  /* A pour effet d'indiquer à la fonction appelante jusqu'où
      la fonction appelée a lu la chaine 'calcul' */
  if (p_calcul != NULL)
      *p_calcul = calcul;

  return G;
}

void xPLCalcul::CalculEvaluate(Calcul* calcul)
{
    ostringstream oss;
    double current;
    string formula;
    size_t pos;
    double value;
    vector<CalculParser*>::iterator itParse;


	LOG_ENTER;
    LOG_VERBOSE(m_Log) << "Evaluate " << calcul->m_Name << " => " << calcul->m_Formula;

    pos = 0;
    for(itParse=calcul->m_CalculParses.begin(); itParse!=calcul->m_CalculParses.end(); ++itParse)
    {
        if(!CacheGet((*itParse)->m_Device, &current))
        {
            LOG_INFO(m_Log) << "Unable to evaluate " << calcul->m_Name << ", device " << (*itParse)->m_Device << " is not in the cache";
            LOG_DEBUG(m_Log) << "*** Exit KO ***";
            return;
        }
        oss << calcul->m_Formula.substr(pos, (*itParse)->m_Deb-pos);
        oss << current;
        pos = (*itParse)->m_Fin+1;
    }

    if(pos<calcul->m_Formula.size()) oss << calcul->m_Formula.substr(pos);
    formula = oss.str();

   	LOG_VERBOSE(m_Log) << "Translated formula " << formula;
	value = CalculOperat(formula.c_str(), NULL, NULL, 0, NULL);
	LOG_VERBOSE(m_Log) << " -> value = " << value;

	SetValue(calcul, value);
  	LOG_EXIT_OK;
}

void xPLCalcul::CalculParse(Calcul* calcul)
{
    ostringstream oss;
    string formule, device;
    size_t iStr, maxStr;
    size_t debDevice, finDevice;
    char currentSigne;
    char prevSigne='0';
    char nextSigne='0';
    xPL::SchemaSensorRequest sensorRequest;


	LOG_ENTER;

    //Filtrer la formule
    LOG_VERBOSE(m_Log) << "Analyse of " << calcul->m_Name << " => " << calcul->m_Formula;
    maxStr = calcul->m_Formula.length();
    for(iStr=0; iStr<maxStr; iStr++)
    {
        currentSigne = calcul->m_Formula.at(iStr);
        if(iStr<maxStr-1)
            nextSigne = calcul->m_Formula.at(iStr+1);
        else
            nextSigne = '0';

        if((currentSigne=='+')||(currentSigne=='*')||(currentSigne=='/')||(currentSigne=='&')||(currentSigne=='|')||(currentSigne=='>')||(currentSigne=='<')||(currentSigne=='(')||(currentSigne==')')) currentSigne = '#';
        if((currentSigne=='-')&&(((prevSigne>='0')&&(prevSigne<='9'))||((nextSigne>='0')&&(nextSigne<='9')))) currentSigne = '#';
        oss << currentSigne;
        prevSigne = currentSigne;
    }
    formule = oss.str();
    LOG_VERBOSE(m_Log) << "Filtered formule " << formule;

    //Rechercher les devices
    debDevice=0;
    while(debDevice<maxStr)
    {
        iStr = formule.find("#", debDevice);
        if(iStr==string::npos)
            finDevice = maxStr;
        else
            finDevice = iStr-1;

        device = formule.substr(debDevice, finDevice-debDevice+1);
        if(!m_xPLDevice.isDevice(device))
        {
            LOG_VERBOSE(m_Log) << device << " is not a device";
            debDevice = finDevice+2;
            continue;
        }

        calcul->m_CalculParses.push_back(new CalculParser(device, debDevice, finDevice));
        CacheAdd(device, calcul);
        LOG_VERBOSE(m_Log) << "Device found : " << device;
        debDevice = finDevice+2;
    }

    RequestValue(calcul->m_CanonicDevice);

  	LOG_EXIT_OK;
}

void xPLCalcul::CalculUpdate(const string& device, const string& current)
{
    std::vector<Calcul*>::iterator it;
    xPL::SchemaSensorRequest sensorRequest;


	LOG_ENTER;

    for(it=m_Calculs.begin(); it!=m_Calculs.end(); ++it)
    {
        if((*it)->m_CanonicDevice==device)
        {
            if((*it)->m_Value!=current)
            {
                LOG_VERBOSE(m_Log) << "Update calcul " << (*it)->m_Name << " value " << (*it)->m_Value << " to " << current;
                (*it)->m_Value = current;
            }
            break;
        }
    }

	LOG_EXIT_OK;
}

void xPLCalcul::AdvanceConfigure()
{
    int i, nb;
    unsigned iCalcul, maxCalcul;


    LOG_ENTER;

    ConfigsDelete();

    nb = m_AdvanceConfig.GetNbConfig();
    for(i=0; i<nb; i++)
    {
        std::map<std::string, std::string>* config;
        string name;
        string output;
        string type;
        string formula;


        config = m_AdvanceConfig.GetConfig(i);

        name    = (*config)["configname"];
        output  = (*config)["output"];
        type    = (*config)["type"];
        formula = (*config)["formula"];

        LOG_VERBOSE(m_Log) << "Create " << name <<" : "<< output <<"("<< type <<") = "<< formula;
        try
        {
            m_Calculs.push_back(new Calcul(name, output, type, formula));
        }
        catch(string e)
        {
            LOG_WARNING(m_Log) << "Bad calcul format (" << e << ")";
        }
    }

    maxCalcul = m_Calculs.size();
    for(iCalcul=0; iCalcul<maxCalcul; iCalcul++) CalculParse(m_Calculs[iCalcul]);

	LOG_EXIT_OK;
}

bool xPLCalcul::MsgAnswer(xPL::SchemaObject& msg)
{
    string source;
    string device;
    string current;
    xPL::ISchema::MsgType msgType;


    msgType = msg.GetMsgType();
    if((msgType != xPL::SchemaObject::trig)&&(msgType != xPL::SchemaObject::stat)) return true;

    device = msg.GetValue("device");
    if(device=="") return true;

    current = msg.GetValue("current");
    if(current=="") return true;

    source = msg.GetSource();
    if(source=="") return true;

    device = source+":"+device;

    CacheUpdate(device, current, msgType);
    CalculUpdate(device, current);

    return true;
}

int xPLCalcul::ServiceStart(int argc, char* argv[])
{
    m_bServiceStop = false;
    if(argc > 1) m_xPLDevice.SetConfigFileName(argv[1]);
    m_xPLDevice.Open();

    while(!m_bServiceStop)
    {
        if(m_bServicePause)
            Plateforms::delay(500);
        else
            m_xPLDevice.WaitRecv(500);
    }

    m_xPLDevice.Close();
    return 0;
}

void xPLCalcul::ServicePause(bool bPause)
{
    m_bServicePause = bPause;
}

void xPLCalcul::ServiceStop()
{
    m_bServiceStop = true;
}
