#include <algorithm>
#include "CalculParser.h"

using namespace std;

CalculParser::CalculParser()
{
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
