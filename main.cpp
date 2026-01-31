#include <iostream>
#include "mtrand.h"
#include <cstring>

int main()
{
    int z =0;

while (1) {

    PhpMtRand rng;
    rng.srand(z);

    uint8_t privbytes[32];
    for (int i = 0; i < 32; ++i) {
        privbytes[i] = rng.rand(0, i ? 0xff : 0xfe);
    }

    for (int i = 0; i < 32; ++i) {
        printf("%02hhx", privbytes[i]);
    }
    printf("\n");

    z++;
    if (z > 14) break;
}

}
