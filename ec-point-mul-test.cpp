 #include <openssl/bn.h>
 #include <openssl/ec.h>
 #include <openssl/obj_mac.h>

 #include "perf-data.h"

int main(){
    EC_GROUP* curve = EC_GROUP_new_by_curve_name(NID_secp256k1);
    BN_CTX* ctx = BN_CTX_new();
    EC_POINT* result = EC_POINT_new(curve);
    BIGNUM *rnd = BN_new();
    BN_rand(rnd, 256, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);

    PerfData::getInstance().StartTime("time");
    for(int i = 0; i < 1000; i++){
        EC_POINT_mul(curve, result, rnd, nullptr, nullptr, ctx);
    }
    PerfData::getInstance().EndTime("time");
    PerfData::getInstance().PrintMs();
}