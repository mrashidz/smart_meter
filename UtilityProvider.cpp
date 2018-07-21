#include "UtilityProvider.h"
#include "HouseHold.h"
#include <math.h>
namespace PrivateDataAgg {

void UtilityProvider::setupSim() {
    auto t1 = Timer::now();
    this->PaillierKeys = Paillier::generate_keys(this->n);
    auto t2 = Timer::now();
    Public::INFO::Instance().setK(this->K);
    Public::INFO::Instance().setL(this->N);

    Public::INFO::Instance().setUP_PK(this->PaillierKeys.PUB);
//    MISC::print_UP_info(this->K,this->N,this->PaillierKeys);
    auto t3 = Timer::now();
    Public::INFO::Instance().setPrimes(MISC::generate_random_prime(this->pk_in_bits/*18*/,this->K));
    this->Agg.init(this->K, this->N);
    auto t4 = Timer::now();
    MISC::print_time("Generating Paillier Keys Took: ",t2-t1);
    MISC::print_time("System Initiallization Took: ",t4-t3);
    MISC::print_done("Simulation Setup Finished -- Offline Phase");
    MISC::print_time("Offline Phase Took: ",t4-t1);
    Public::INFO::Instance().checkSetup();
}

void UtilityProvider::configureSetup(const _uint_64_t _N, const _uint_64_t _K, const _uint_64_t _beta, const _uint_64_t _n, const _uint_64_t _pk)
{
    this->N = _N;
    this->n = _n;
    this->K = _K;
    this->Beta = _beta;
    this->pk_in_bits = _pk;
}

void UtilityProvider::runSim() {
    MISC::print_init("Protocol Run (Data Collection) Initiated -- Online Phase -- Normal Mode");
    auto t1 = Timer::now();
    mpz_class result = this->calcTheResult(Paillier::decrypt(this->Agg.collectData(),this->PaillierKeys));
    auto t2 = Timer::now();
    MISC::print_done("Protocol Run (Data Collection) -- Online Phase -- Normal Mode");
    MISC::print_time("Online Phase Took: ",t2-t1);
    mpz_class reportedResult(0xFFFF);
    MISC::print_int("Calced REs",result);
    mpz_mul_ui(reportedResult.get_mpz_t(),reportedResult.get_mpz_t(),this->N);
    MISC::print_int("report Res",reportedResult);
    std::cout << this->N << " * 0xFFFF" << std::endl;
    if (!mpz_cmp(result.get_mpz_t(),reportedResult.get_mpz_t()))
        MISC::print_init("Results are the same");
    else
        MISC::print_done("Results differ");
    this->clean();
}

void UtilityProvider::runSim_discardGroup(const _uint_64_t _gid)
{
    MISC::print_init("Protocol Run (Data Collection) Initiated -- Online Phase -- Normal Mode");
    std::cout << this->N << "Omitting results for group " << _gid << std::endl;
    auto t1 = Timer::now();
    mpz_class tmp,result, cipher = Paillier::decrypt(this->Agg.collectData_DiscardGroup(_gid),this->PaillierKeys);
    for (int i = 0; i < this->K; i++) {
        if(_gid==i) continue;
        mpz_mod(tmp.get_mpz_t(),cipher.get_mpz_t(),Public::INFO::Instance().getGroupPrimes(i).get_mpz_t());
//        std::cout << "Group: " << i << " ";
//        MISC::print_int("Result",tmp);
        mpz_add(result.get_mpz_t(),result.get_mpz_t(),tmp.get_mpz_t());
    }
    auto t2 = Timer::now();
    MISC::print_done("Protocol Run (Data Collection) -- Online Phase -- Normal Mode");
    MISC::print_time("Online Phase Took: ",t2-t1);

    mpz_class reportedResult(0xFFFF);
    MISC::print_int("Calced REs",result);
    unsigned int newN = this->N - this->M; //discarding a group means M households
    mpz_mul_ui(reportedResult.get_mpz_t(),reportedResult.get_mpz_t(),newN);
    MISC::print_int("report Res",reportedResult);
    std::cout << newN << " * 0xFFFF" << std::endl;
    if (!mpz_cmp(result.get_mpz_t(),reportedResult.get_mpz_t()))
        MISC::print_init("Results are the same");
    else
        MISC::print_done("Results differ");
    this->clean();

}

mpz_class UtilityProvider::calcTheResult(const mpz_class _res) {
//    MISC::print_init("All data have been collected, calulating the result");
//    MISC::print_int("aggretaro collected",_res);
    mpz_class result;
    mpz_class tmp;
    for (int i = 0; i < this->K; i++) {
        mpz_mod(tmp.get_mpz_t(),_res.get_mpz_t(),Public::INFO::Instance().getGroupPrimes(i).get_mpz_t());
//        std::cout << "Group: " << i << " ";
//        MISC::print_int("Result",tmp);
        mpz_add(result.get_mpz_t(),result.get_mpz_t(),tmp.get_mpz_t());
    }
//    MISC::print_int("Total Result",result);
//    MISC::print_done("Result calculation finished");
    return result;
}

void UtilityProvider::configureSetup(_uint_64_t _N, _uint_64_t _K) {
    this->N = _N;
    this->K = _K;
    this->n = this->Beta * this->N;
    this->M = (this->N/this->K);
    if (this->N%this->K) this->M++;//for dummy
    this->pk_in_bits = this->M*this->Beta;
    ++this->pk_in_bits;
    _uint_64_t tmp = std::log2(n);    
    this->n = std::pow(2,tmp+1);
    if (this->n < 1024)
        this->n = 1024;
//    this->n = 2048;
    std::cout << "N: " << this->N << " K: " << this->K
              << " Beta: " << this->Beta << " n: " << this->n << " M: " << this->M
              << " P_K " << this->pk_in_bits <<std::endl;
}

void UtilityProvider::clean()
{
//    MISC::print_init("Cleaning");
    this->Agg.clean();
//    MISC::print_done("Finished");
}



}
