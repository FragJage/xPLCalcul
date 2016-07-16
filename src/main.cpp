#include <iostream>
#include <exception>
#include "xPLCalcul.h"

int main(int argc, char* argv[])
{
  int res = 0;

  try
  {
    xPLCalcul xPLDev;
  
  	Service* pService = Service::Create("xPLCalcul", "Calculator for xPL protocol", &xPLDev);
  	res = pService->Start(argc, argv);
  	Service::Destroy();
  }
  catch(const exception &e)
  {
      std::cout << e.what();
  }
	return res;
}
