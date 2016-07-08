#include <algorithm>
#include "CacheDevice.h"

using namespace std;

CacheDevice::CacheDevice()
{
    m_Lu = false;
}

CacheDevice::CacheDevice(Calcul* calcul)
{
    m_Lu = false;
    m_Calculs.push_back(calcul);
}

CacheDevice::~CacheDevice()
{
}
