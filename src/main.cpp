#include "xPLCalcul.h"

int main(int argc, char* argv[])
{
    xPLCalcul xPLDev;

	Service* pService = Service::Create("xPLCalcul", "Calculator for xPL protocol", &xPLDev);
	int res = pService->Start(argc, argv);
	Service::Destroy();
	return res;
}
