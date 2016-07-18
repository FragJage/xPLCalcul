#include <algorithm>
#include "CalculParser.h"

using namespace std;

CalculParser::CalculParser()
{
    m_Device = "";
    m_Deb = 0;
    m_Fin = 0;
}

CalculParser::CalculParser(const string& device, size_t deb, size_t fin)
{
    m_Device = device;
    m_Deb = deb;
    m_Fin = fin;
}

CalculParser::~CalculParser()
{
}
