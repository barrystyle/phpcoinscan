#ifndef KEYFUNCS_H
#define KEYFUNCS_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <openssl/hmac.h>
#include <secp256k1.h>

#include "base58.h"

bool derive_keys(const uint8_t priv[32], pairSet& result);

#endif // KEYFUNCS_H
