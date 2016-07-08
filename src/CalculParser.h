#ifndef CALCULPARSER_H
#define CALCULPARSER_H

#include<string>

class CalculParser
{
    public:
        CalculParser();
        CalculParser(const std::string& device, size_t deb, size_t fin);
        ~CalculParser();

        std::string m_Device;
        size_t m_Deb;
        size_t m_Fin;
};

#endif // CALCULPARSER_H
