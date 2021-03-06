#include "sage3basic.h"

#include "conditionalDisable.h"
#ifdef ROSE_BINARY_TEST_DISABLED
#include <iostream>
int main() { std::cout <<"disabled for " <<ROSE_BINARY_TEST_DISABLED <<"\n"; return 1; }
#else

#include "Registers.h"

int
main()
{
    const RegisterDictionary *dict = RegisterDictionary::dictionary_powerpc();
    ROSE_ASSERT(dict!=NULL);
    // std::cout <<*dict;

    const RegisterDescriptor *desc = dict->lookup("spr8");
    ROSE_ASSERT(desc!=NULL);
    // std::cout <<"found r8 = " <<*desc <<"\n";

    /* Register "lr" is the preferred name for "spr8", so that's what we'll get back for the reverse lookup */
    std::string alias = dict->lookup(*desc);
    ROSE_ASSERT(alias=="lr");

    return 0;
}

#endif
