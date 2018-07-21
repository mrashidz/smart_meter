#include "Aggregator.h"
namespace PrivateDataAgg {
void clsAggregator::init(const _uint_64_t _K, const _uint_64_t _L){
    this->Groups.resize(_K);
    unsigned int eachGroupShareOfHH = _L / _K;
    unsigned int leftoverHHs = _L % _K;
//    MISC::print_int("Number of Households in each group",eachGroupShareOfHH);
//    MISC::print_int("Number of Leftover Households",leftoverHHs);
    for (int i = 0; i < _K; i ++) {
        if (leftoverHHs) {
            this->Groups[i].init(eachGroupShareOfHH+1,i);
            --leftoverHHs;
        } else
            this->Groups[i].init(eachGroupShareOfHH,i);
    }
}

mpz_class clsAggregator::collectData() {
    mpz_class result = 1;
    mpz_class n_2 = Public::INFO::Instance().getUP_PK(0).n2;
    for (unsigned int i = 0; i <Public::INFO::Instance().getK(); i ++) {
        mpz_mul(result.get_mpz_t(),result.get_mpz_t(),this->Groups[i].collectData().get_mpz_t());       
        mpz_mod(result.get_mpz_t(),result.get_mpz_t(),n_2.get_mpz_t());
    }
    return result;
}

void clsAggregator::clean() {
    _uint_64_t K = Public::INFO::Instance().getK();
    for (_uint_64_t i = 0; i < K; i ++)
        this->Groups[i].clean();
    this->Groups.clear();
    Public::INFO::Instance().clean();
}

mpz_class clsAggregator::collectData_DiscardGroup(const _uint_64_t _gid) {
    mpz_class result = 1;
    mpz_class n_2 = Public::INFO::Instance().getUP_PK(0).n2;
    mpz_class beta,pm_invert,T_kt, beta_invert,p_k;
    mpz_class pm = Public::INFO::Instance().getGroupPrimes(_gid);
    for (unsigned int i = 0; i <Public::INFO::Instance().getK() ; i ++) {
        if (i == _gid)
            continue;
        p_k = Public::INFO::Instance().getGroupPrimes(i);
        T_kt = this->Groups[i].collectData();
        mpz_invert(pm_invert.get_mpz_t(),pm.get_mpz_t(),p_k.get_mpz_t());
        mpz_mul(beta.get_mpz_t(),pm.get_mpz_t(),pm_invert.get_mpz_t());
        mpz_invert(beta_invert.get_mpz_t(),beta.get_mpz_t(),p_k.get_mpz_t());
        mpz_mul(beta.get_mpz_t(),T_kt.get_mpz_t(),beta_invert.get_mpz_t());
        mpz_mul(result.get_mpz_t(),result.get_mpz_t(),beta.get_mpz_t());
        mpz_mod(result.get_mpz_t(),result.get_mpz_t(),n_2.get_mpz_t());
    }
    return result;
}
}
