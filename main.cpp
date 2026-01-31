#include <iostream>
#include "db.h"
#include "mtrand.h"
#include "keyfuncs.h"
#include <cstring>

int main()
{
    int z;
    PhpMtRand rng;
    pairSet result;

    z = 0;

    //load addr into mem
    initdb();

    while (1){

        //calculate private key
        rng.srand(z);
        uint8_t privbytes[32];
        for (int i = 0; i < 32; ++i) {
            privbytes[i] = rng.rand(0, i ? 0xff : 0xfe);
        }

        //get public key
        derive_keys(privbytes, result);
        printf("\r%d - %s", z, result.wif_uncompressed_pubkey.c_str());

        if (searchaddress(result.wif_uncompressed_pubkey) > 0) {
            printf("\n%d - %s\n FOUND\n", z, result.wif_uncompressed_pubkey.c_str());
        }

        z++;
    }

}
