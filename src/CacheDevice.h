#ifndef CACHEDEVICE_H
#define CACHEDEVICE_H

#include<string>
#include<vector>
#include "Calcul.h"

class CacheDevice
{
    public:
        CacheDevice();
        CacheDevice(Calcul* calcul);
        ~CacheDevice();

        bool m_Lu;
        double m_Current;
        std::vector<Calcul*> m_Calculs;
};

#endif // CACHEDEVICE_H
