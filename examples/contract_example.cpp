#include <iostream>
#include <string.h>
#include "uccontract.h"
#include "ucontainer.h"
#include "ucio.h"

using namespace JAD;

int main(int argc, char** argv)
{
  try {
    UniversalContainer cspec = uc_from_json_file("contract.json");
    UCContract contract(cspec);
    UniversalContainer uc = uc_from_json_file("data.json");

    contract.compare_and_throw(uc);
    std::cout << "Data matches contract." << std::endl;

    return 0;
  } catch(UniversalContainer uce) {
    print(uce);
    return 1;
  }
}

