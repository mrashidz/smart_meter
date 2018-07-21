#ifndef GLOBAL_H
#define GLOBAL_H
#include <iostream>
#include <gmpxx.h>
#include <string>
#include <sys/random.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/sha3.h>
#include <chrono>
#include <stdio.h>
#include <time.h>
#define DH_KEY_LEN 1024
typedef unsigned long long _uint_64_t;
typedef std::chrono::high_resolution_clock Timer;

namespace Paillier {
typedef struct PublicKey {
    mpz_class g;
    mpz_class n;
    mpz_class n2;
    PublicKey(){}
    PublicKey(mpz_class _g,mpz_class _n,mpz_class _n2):g(_g),n(_n),n2(_n2){}
}stu_PublicKey;
typedef struct PrivateKey {
    mpz_class lambda;
    mpz_class mu;
    PrivateKey(){}
    PrivateKey(mpz_class _l,mpz_class _m):lambda(_l),mu(_m){}
}stu_PrivateKey;
typedef struct KeyPairs{
    stu_PublicKey PUB;
    stu_PrivateKey PRI;
    KeyPairs(){}
    KeyPairs(stu_PublicKey _pub,stu_PrivateKey _pri):PUB(_pub),PRI(_pri){}    
}stu_KeyPair;

    stu_KeyPair generate_keys(const unsigned int _bits);
    mpz_class decrypt(const mpz_class _c,const stu_KeyPair _k);
    mpz_class encrypt(const mpz_class _m,const stu_PublicKey _pk);
    mpz_class encrypt_with_hash(const mpz_class _m,const std::string _h,const mpz_class _n_alfa,const stu_PublicKey _pk);
    mpz_class function_L(const mpz_class _x,const mpz_class _n);
}

namespace DiffieHellman {
typedef struct PublicParameters
{
    mpz_class Modulus;
    mpz_class Generator;
    mpz_class GroupOrder;
    PublicParameters(){}
    PublicParameters(mpz_class _p, mpz_class _g, mpz_class _q)
        :Modulus(_p), Generator(_g), GroupOrder(_q){}
}stu_PublicParameters;
typedef struct KeyPairs {
        mpz_class PUB;
        mpz_class PRI;
        KeyPairs(mpz_class _pub, mpz_class _pri):PUB(_pub),PRI(_pri){}
}stu_KeyPairs;
stu_PublicParameters generate_public_parameters(const unsigned int _bits);
stu_KeyPairs generateDHKeys(const stu_PublicParameters _dhPpara);
}

namespace Public {
class INFO
{
public:    
    static INFO& Instance() {
        static INFO s;
        return s;
    }
    ~INFO(){}
    void setPrimes(std::vector<mpz_class> _gpk);
    void updateGroup(const unsigned int _i);
    inline void setUP_PK(Paillier::stu_PublicKey _ppk){this->UP_PK = _ppk;}    
    inline Paillier::stu_PublicKey getUP_PK(const _uint_64_t _gid) {
        return Paillier::stu_PublicKey(this->GroupGenertators_gk[_gid],
                                       this->UP_PK.n, this->UP_PK.n2);
    }    
    inline mpz_class getGroupPrimes(const unsigned int _i) {return this->GroupPrimes_pk[_i];}    
    inline _uint_64_t getK() {return this->K;}
    inline _uint_64_t getL() {return this->L;}    
    inline void setL(const _uint_64_t _l) {this->L=_l;}
    void setK(const _uint_64_t _k);
    inline mpz_class getGroupPrimesProduct() {return this->GroupPrimesProducts;}
    inline CryptoPP::SHA3_512 getHASHFunction() {return this->SHA;}
    void checkSetup();
    void clean();

private:
    INFO();
    std::vector<mpz_class> GroupGenertators_gk;
    std::vector<mpz_class> GroupPrimes_pk;    
    mpz_class GroupPrimesProducts;
    Paillier::stu_PublicKey UP_PK;
    mpz_class ReportedResult;
    CryptoPP::SHA3_512 SHA;
    _uint_64_t K;
    _uint_64_t L;    
};

}

namespace MISC {
//std::string getDateTime();
std::vector<mpz_class> generate_random_prime(const unsigned int _bits, const unsigned int _count);
mpz_class find_co_prime(const mpz_class _order);
mpz_class convert_CryptoPP_to_GMP(const CryptoPP::SecByteBlock _data);
void print_int(const char *_name,const _uint_64_t _val);
void print_int(const char *_name,const mpz_class _val);
void print_int(const char *_name,const mpz_class *_val,const _uint_64_t _len);
void print_pailler_key_pair(const char *_name, Paillier::stu_KeyPair _val);
void print_dh_key_pair(const char *_name,const DiffieHellman::stu_KeyPairs _val);
void print_dh_pub_para(const char *_name, DiffieHellman::stu_PublicParameters _val);
void print_group_info(const _uint_64_t _gid, mpz_class _pk, mpz_class _piyi);
void print_pairing(const _uint_64_t _hid1,const _uint_64_t _hid2,const _uint_64_t _gid);
void print_init(const char *_name);
void print_done(const char *_name);
void print_UP_info(const _uint_64_t _gk,const _uint_64_t _hl,const Paillier::stu_KeyPair _kp);
void print_HH_info(const _uint_64_t _gid,const _uint_64_t _hid,const mpz_class _alfa,const mpz_class _n_alfa);
void print_measurement(const mpz_class _m, const std::string _time, const _uint_64_t _id, const _uint_64_t _gid);
inline void print_time(const char * _message,const std::chrono::nanoseconds _time) {
    std::cout << "(.:.) >>> " << _message << std::chrono::duration_cast<std::chrono::milliseconds>(_time).count() << " ms <<<" <<std::endl<<std::endl;
}
}

#endif // GLOBAL_H
