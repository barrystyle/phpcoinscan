#include <iostream>
#include "db.h"
#include "mtrand.h"
#include "keyfuncs.h"
#include <cstring>

#define max_thr 4

void search_thread(int thr_id)
{
    int z = (std::numeric_limits<int>::max() / max_thr) * thr_id;
    PhpMtRand rng;
    pairSet result;

    while (1) {

        //calculate private key
        rng.srand(z);
        uint8_t privbytes[32];
        for (int i = 0; i < 32; ++i) {
            privbytes[i] = rng.rand(0, i ? 0xff : 0xfe);
        }

        //get public key
        derive_keys(privbytes, result);
        printf("\033[%d;0H %d - %s", thr_id+1, z, result.wif_uncompressed_pubkey.c_str());

        if (searchaddress(result.wif_uncompressed_pubkey) > 0) {
            printf("\n%d - %s\n FOUND\n", z, result.wif_uncompressed_pubkey.c_str());
        }

        z++;
    }
}

int main()
{
    //load addr into mem
    initdb();

    //threads
    std::vector<std::thread> search_threads;
    for (int i=0; i<max_thr; i++) {
        search_threads.push_back(std::thread(search_thread, i));
    }

    //wait
    for (int i=0; i<max_thr; i++) {
        if (search_threads[i].joinable()) {
            search_threads[i].join();
        }
    }

    return 1;
}
