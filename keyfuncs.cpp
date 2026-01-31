#include <iostream>
#include <iomanip>
#include <cstring>
#include <openssl/hmac.h>
#include <secp256k1.h>

#include "base58.h"

bool pubkey_from_priv(secp256k1_context* ctx, const uint8_t priv[32], uint8_t *out_pub, size_t &outlen, bool compressed) {

    if (!secp256k1_ec_seckey_verify(ctx, priv)) {
        return false;
    }

    secp256k1_pubkey pk;
    if (!secp256k1_ec_pubkey_create(ctx, &pk, priv)) {
        return false;
    }

    outlen = compressed ? 33 : 65;
    unsigned int flags = compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED;
    if (!secp256k1_ec_pubkey_serialize(ctx, out_pub, &outlen, &pk, flags)) {
        return false;
    }

    return true;
}

bool derive_keys(const uint8_t priv[32], pairSet& result) {

    secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);

    uint8_t pub_comp[33];
    size_t pub_comp_len = 33;
    if (!pubkey_from_priv(ctx, priv, pub_comp, pub_comp_len, true)) {
        secp256k1_context_destroy(ctx);
        return false;
    }

    uint8_t pub_uncomp[65];
    size_t pub_uncomp_len = 65;
    if (!pubkey_from_priv(ctx, priv, pub_uncomp, pub_uncomp_len, false)) {
        secp256k1_context_destroy(ctx);
        return false;
    }

    //result.wif_compressed_pubkey = p2pkh_from_pub(pub_comp, pub_comp_len);
    //result.wif_compressed_privkey = wif_from_priv(priv, true);
    result.wif_uncompressed_pubkey = p2pkh_from_pub(pub_uncomp, pub_uncomp_len);
    //result.wif_uncompressed_privkey = wif_from_priv(priv, false);

    secp256k1_context_destroy(ctx);
    return true;
}
