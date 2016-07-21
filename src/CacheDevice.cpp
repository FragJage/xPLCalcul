#include <algorithm>
#include "CacheDevice.h"

using namespace std;

CacheDevice::CacheDevice()
{
    m_Lu = false;
    m_Current = 0;
}

CacheDevice::CacheDevice(Calcul* calcul)
{
    m_Lu = false;
    m_Current = 0;
    m_Calculs.push_back(calcul);
}

CacheDevice::~CacheDevice()
{
}
