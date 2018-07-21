#ifndef UTILITYPROVIDER_H
#define UTILITYPROVIDER_H

#include "Aggregator.h"
//#include "Group.h"

namespace PrivateDataAgg {
class UtilityProvider
{
public:
    UtilityProvider()
        :n(0), N(0), M(0), K(0),Beta(16),pk_in_bits(0){}
    void runSim();
    void runSim_discardGroup(const _uint_64_t _gid);
    void setupSim();
    void configureSetup(const _uint_64_t _N,
                        const _uint_64_t _K);
    void configureSetup(const _uint_64_t _N,
                        const _uint_64_t _K,
                        const _uint_64_t _beta,
                        const _uint_64_t _n,
                        const _uint_64_t _pk);

private:
    mpz_class calcTheResult(const mpz_class _res);
    void clean();
    Paillier::stu_KeyPair PaillierKeys;
    clsAggregator Agg;    
    _uint_64_t n;
    _uint_64_t M;
    _uint_64_t K;
    _uint_64_t N;
    _uint_64_t Beta;
    _uint_64_t pk_in_bits;
};
}
#endif // UTILITYPROVIDER_H


