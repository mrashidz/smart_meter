#include "Global.h"
#include <iostream>
namespace MISC {
mpz_class convert_CryptoPP_to_GMP(const CryptoPP::SecByteBlock _data) {
    std::string secret_in_hex;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(secret_in_hex));
    encoder.Put(_data.data(), _data.size());
    encoder.MessageEnd();
    return mpz_class(secret_in_hex,16);
}

mpz_class find_co_prime(const mpz_class _order) {
    mpz_class r, tmp;
    gmp_randstate_t rand_state;
    gmp_randinit_default(rand_state);
    unsigned long seed = 0;
    getrandom(&seed,sizeof seed,GRND_NONBLOCK);
    gmp_randseed_ui(rand_state,seed);
    do {
        mpz_urandomm(r.get_mpz_t(),rand_state,_order.get_mpz_t());
        mpz_gcd(tmp.get_mpz_t(),_order.get_mpz_t(),r.get_mpz_t());
    } while (tmp.get_ui() != 1);
    gmp_randclear(rand_state);
    return r;
}

std::vector<mpz_class> generate_random_prime(const unsigned int _bits, const unsigned int _count) {
    std::vector<mpz_class> primes;
    mpz_class tmp, prime;
    gmp_randstate_t rand_state;
    gmp_randinit_mt(rand_state);
    unsigned long seed = 0;
    getrandom(&seed,sizeof seed,GRND_NONBLOCK);
    gmp_randseed_ui(rand_state,seed);
    for (int i = 0; i < _count; i ++) {
        mpz_urandomb(tmp.get_mpz_t(), rand_state, _bits);
        mpz_nextprime(prime.get_mpz_t(),tmp.get_mpz_t());
        if (mpz_sizeinbase(tmp.get_mpz_t(),2) < _bits) //make sure the size satisfies the requirements
            --i;//otherwise loop back
        else
            primes.push_back(prime);
    }
    gmp_randclear(rand_state);
    return primes;
}

void print_int(const char *_name, mpz_class _val) {
    std::cout << "(INT) >>> [ " << _name << " : ";
    std::cout << _val << " ] <<< " << std::endl;
}

void print_int(const char *_name, const mpz_class *_val, const _uint_64_t _len) {
    std::cout << "(INT) >>> [ " << _name << " -- size: ";
    std::cout << _len << " ] <<< " << std::endl;
    for (_uint_64_t i = 0; i < _len; i ++)
        std::cout <<"[ " << i << " ] "<<  _val[i] << std::endl;
    std::cout << std::endl;
}

void print_int(const char *_name, const _uint_64_t _val) {
    std::cout << "(INT) >>> [ " << _name << " : ";
    std::cout << _val << " ] <<< " << std::endl;
}

void print_init(const char *_name) {
    std::cout << "(+++) >>> " << _name << std::endl;
}

void print_done(const char *_name) {
    std::cout << "(---) <<< " << _name << std::endl<< std::endl;
}

void print_pailler_key_pair(const char *_name, Paillier::stu_KeyPair _val) {
    std::cout << "( P ) >>> " << _name << std::endl;
    std::cout << "(...) >>> PublicKey (g, n):" << std::endl;
    std::cout << "(...) >>> [ g : " << _val.PUB.g << " ] <<< " << std::endl;
    std::cout << "(...) >>> [ n : " << _val.PUB.n << " ] <<< " << std::endl;
    std::cout << "(...) >>> [ n^2 : " << _val.PUB.n2 << " ] <<< " << std::endl;
    std::cout << "(...) >>> PrivateKey (Lambda, Mu):" << std::endl;
    std::cout << "(...) >>> [ Lambda : " << _val.PRI.lambda << " ] <<< " << std::endl;
    std::cout << "(...) >>> [ Mu : " << _val.PRI.mu << " ] <<< " << std::endl;
    std::cout << "( P ) <<< " << std::endl;

}

void print_dh_key_pair(const char *_name, DiffieHellman::stu_KeyPairs _val) {
    std::cout << "(D-H) >>> " << _name << std::endl;
    std::cout << "(...) >>> [ PublicKey : " << _val.PUB << " ] <<< " << std::endl;
    std::cout << "(...) >>> [ PrivateKey : " << _val.PRI << " ] <<< " << std::endl;
    std::cout << "(D-H) <<< " << std::endl;

}

void print_dh_pub_para(const char *_name, DiffieHellman::stu_PublicParameters _val) {
    std::cout << "(D-H) >>> " << _name << std::endl;
    std::cout << "(...) >>> [ Generator : " << _val.Generator << " ] <<< " << std::endl;
    std::cout << "(...) >>> [ Modulus : " << _val.Modulus << " ] <<< " << std::endl;
    std::cout << "(...) >>> [ Order : " << _val.GroupOrder << " ] <<< " << std::endl;
    std::cout << "(D-H) <<< " << std::endl;
}

void print_group_info(const _uint_64_t _gid, mpz_class _pk, mpz_class _piyi) {
    std::cout << "(GINFO) > Group : " << _gid << std::endl;    
    print_int("p_k",_pk);
    print_int("P_i*y_i",_piyi);
    std::cout << "(GINFO) < " << std::endl<< std::endl;
}

void print_pairing(const _uint_64_t _hid1, _uint_64_t _hid2, _uint_64_t _gid) {
    std::cout << "(PAIR) >> Pair: " << _hid1 << " & " << _hid2 << " from group: " << _gid << " <<< " << std::endl;
}

void print_UP_info(const _uint_64_t _gk, const _uint_64_t _hl, const Paillier::stu_KeyPair _kp) {
    std::cout << "( UP ) >> UP with " << _gk << " groups and "<<_hl<<" households. " << std::endl;
    print_pailler_key_pair("Paillier KeyPair", _kp);
    std::cout << "( UP ) << " << std::endl<< std::endl;
}

void print_HH_info(const _uint_64_t _gid,const _uint_64_t _hid, const mpz_class _alfa,const mpz_class _n_alfa) {
    std::cout << "(HINFO) > Household " << _hid << " from group "<<_gid<< std::endl;
    print_int("Alfa",_alfa);
    print_int("N_Alfa",_n_alfa);
    std::cout << "(HINFO) < " << std::endl<< std::endl;
}

void print_measurement(const mpz_class _m, const std::string _time, const _uint_64_t _id, const _uint_64_t _gid) {
    std::cout << "(H MES) > Household " << _id << " from group "<<_gid;
    std::cout << " at: " <<_time<<std::endl;
    print_int("Measurement",_m);
    std::cout << "(H MES) < " << std::endl<< std::endl;
}
} //namespace MISC

namespace Paillier {

stu_KeyPair generate_keys(const unsigned int _bits) {
    MISC::print_init("Initializing Paillier CryptoSystem");
    std::vector<mpz_class> pANDq = MISC::generate_random_prime(_bits/2,2);
    //computing lambda
    mpz_class p_1 = pANDq[0] - 1;
    mpz_class q_1 = pANDq[1] - 1;
    mpz_class lambda;
//    mpz_lcm(lambda.get_mpz_t(),p_1.get_mpz_t(),q_1.get_mpz_t());
    mpz_mul(lambda.get_mpz_t(),p_1.get_mpz_t(),q_1.get_mpz_t());
    //computing n and n^2
    mpz_class n,n2;
    mpz_mul(n.get_mpz_t(), pANDq[0].get_mpz_t(), pANDq[1].get_mpz_t());
    mpz_mul(n2.get_mpz_t(), n.get_mpz_t(), n.get_mpz_t());
    //find a g in Z*_n^2    
    mpz_class g = n + 1;
    //computing mu
//    mpz_class glanda;
    mpz_class mu;
//    mpz_powm(glanda.get_mpz_t(),g.get_mpz_t(),lambda.get_mpz_t(),n2.get_mpz_t());
//    mpz_class l_glanda = function_L(glanda,n);
//    mpz_invert(mu.get_mpz_t(),l_glanda.get_mpz_t(),n.get_mpz_t());
    mpz_invert(mu.get_mpz_t(),lambda.get_mpz_t(),n.get_mpz_t());
    MISC::print_done("Paillier Key Pairs Generated");    
    return stu_KeyPair(stu_PublicKey(g,n,n2),stu_PrivateKey(lambda,mu));
}

mpz_class encrypt(const mpz_class _m, const stu_PublicKey _pk) {
    mpz_class cipher = 0,gm,rn,gm_rn;
    if (!mpz_cmp(_pk.n.get_mpz_t(),_m.get_mpz_t())>0) {
        return cipher;//error? what to do... TODO handle
    }
    mpz_class r = MISC::find_co_prime(_pk.n);
    mpz_powm(gm.get_mpz_t(),_pk.g.get_mpz_t(), _m.get_mpz_t(), _pk.n2.get_mpz_t());
    mpz_powm(rn.get_mpz_t(), r.get_mpz_t(),_pk.n.get_mpz_t(),_pk.n2.get_mpz_t());
    mpz_mul(gm_rn.get_mpz_t(), gm.get_mpz_t(), rn.get_mpz_t());
//    gm_rn = gm * rn;
    mpz_mod(cipher.get_mpz_t(),gm_rn.get_mpz_t(),_pk.n2.get_mpz_t());
    return cipher;
}

mpz_class decrypt(const mpz_class _c, const stu_KeyPair _k) {
    ///@TODO check for _c if it is in the domain
    mpz_class plain,l,l_mu,l_f;
    mpz_powm(l.get_mpz_t(),_c.get_mpz_t(),_k.PRI.lambda.get_mpz_t(),_k.PUB.n2.get_mpz_t());
    l_f = function_L(l,_k.PUB.n);
    mpz_mul(l_mu.get_mpz_t(), l_f.get_mpz_t(),_k.PRI.mu.get_mpz_t());
    mpz_mod(plain.get_mpz_t(),l_mu.get_mpz_t(),_k.PUB.n.get_mpz_t());
    return plain;
}

mpz_class encrypt_with_hash(const mpz_class _m, const std::string _h, const mpz_class _n_alfa, const stu_PublicKey _pk) {
    mpz_class cipher = 0,gm,rn,gm_rn, gcd, h;
    if (!mpz_cmp(_pk.n.get_mpz_t(),_m.get_mpz_t())>0) {
        return cipher;//error? what to do... TODO handle
    }
    mpz_class r = MISC::find_co_prime(_pk.n);
    mpz_powm(gm.get_mpz_t(),_pk.g.get_mpz_t(), _m.get_mpz_t(), _pk.n2.get_mpz_t());
    mpz_powm(rn.get_mpz_t(), r.get_mpz_t(),_pk.n.get_mpz_t(),_pk.n2.get_mpz_t());
    mpz_mul(gm_rn.get_mpz_t(), gm.get_mpz_t(), rn.get_mpz_t());
    mpz_mod(gm_rn.get_mpz_t(),gm_rn.get_mpz_t(),_pk.n2.get_mpz_t());
//    gm_rn = gm * rn;
    byte digested[CryptoPP::SHA3_512::DIGESTSIZE];
    CryptoPP::SHA3 hash_fun = Public::INFO::Instance().getHASHFunction();
//    do {
        hash_fun.CalculateDigest(digested,(const byte *)_h.c_str(),_h.size());
        h = MISC::convert_CryptoPP_to_GMP(CryptoPP::SecBlock<byte>(digested,sizeof digested));
//        mpz_gcd(gcd.get_mpz_t(),h.get_mpz_t(),_pk.n.get_mpz_t());
//    } while(gcd.get_ui() != 1);
    //h^n_alfa
    mpz_powm(h.get_mpz_t(),h.get_mpz_t(),_n_alfa.get_mpz_t(),_pk.n2.get_mpz_t());
    //r^n*g^m * mod n^2
    mpz_mul(gcd.get_mpz_t(),h.get_mpz_t(),gm_rn.get_mpz_t());
    mpz_mod(cipher.get_mpz_t(),gcd.get_mpz_t(),_pk.n2.get_mpz_t());
    return cipher;
}

mpz_class function_L(mpz_class _x, mpz_class _n) {
    mpz_class result, x_1 = _x - 1;
    mpz_div(result.get_mpz_t(),x_1.get_mpz_t(),_n.get_mpz_t());
    return result;
}
}// namespace Paillier

namespace Public {

INFO::INFO():GroupPrimesProducts(0),ReportedResult(0) {}

void INFO::setPrimes(std::vector<mpz_class> _gpk) {
    //MISC::print_int("Group Primes", _gpk, this->getK());
    this->GroupPrimes_pk = _gpk;
    this->GroupPrimesProducts = 1;
    for (unsigned int i = 0; i < this->K; i ++)
    mpz_mul(this->GroupPrimesProducts.get_mpz_t(),
            this->GroupPrimesProducts.get_mpz_t(),
            this->GroupPrimes_pk[i].get_mpz_t());
//    MISC::print_int("Group Primes Product", this->GroupPrimesProducts);
}

void INFO::updateGroup(const unsigned int _i) {
    mpz_class P_i,y_i,Pi_Yi;
    mpz_divexact(P_i.get_mpz_t(),this->GroupPrimesProducts.get_mpz_t(),this->GroupPrimes_pk[_i].get_mpz_t());
    mpz_invert(y_i.get_mpz_t(),P_i.get_mpz_t(),this->GroupPrimes_pk[_i].get_mpz_t());
    mpz_mul(Pi_Yi.get_mpz_t(),P_i.get_mpz_t(),y_i.get_mpz_t());
//    MISC::print_group_info(_i,this->GroupInfo[_i]);
    mpz_mod(Pi_Yi.get_mpz_t(),Pi_Yi.get_mpz_t(),this->GroupPrimesProducts.get_mpz_t());
    mpz_powm(GroupGenertators_gk[_i].get_mpz_t(), this->UP_PK.g.get_mpz_t(),Pi_Yi.get_mpz_t(),this->UP_PK.n2.get_mpz_t());


}

void INFO::setK(const _uint_64_t _k) {
    this->K=_k;
    this->GroupGenertators_gk.resize(_k);
    this->GroupPrimes_pk.resize(_k);
}

void INFO::checkSetup() {
    _uint_64_t M = (this->L / this->K);
    if(this->L % this->K)M++;
    unsigned int mb = 16*M,len;
    unsigned int kmb = mb*this->K;
    unsigned int p = mpz_sizeinbase(this->GroupPrimesProducts.get_mpz_t(),2);
    unsigned int n = mpz_sizeinbase(this->UP_PK.n.get_mpz_t(),2);
    std::cout<< "P(" << p << ") > KM*16("<<kmb << ")"<<std::endl;
    std::cout << " n = " << n << std::endl;
    std::cout << " N = " << this->L << " n/16 = "<< n/16 <<  std::endl;
    for (_uint_64_t i = 0; i < this->K; i ++) {
        len = mpz_sizeinbase(this->GroupPrimes_pk[i].get_mpz_t(),2);
        std::cout<< "p_k( " << len << ") > M*16("<<mb << ")\t";
    }
    std::cout<<std::endl;
}

void INFO::clean() {
    this->GroupPrimesProducts = 0;
    this->K = 0;
    this->ReportedResult = 0;
    this->L = 0;
    this->GroupGenertators_gk.clear();
    this->GroupPrimes_pk.clear();    
}
}// namespace Public

namespace DiffieHellman {

stu_PublicParameters generate_public_parameters(const unsigned int _bits) {
    std::vector<mpz_class> mo = MISC::generate_random_prime(_bits,1);
    mpz_class generator,order = mo[0]-1,tmp;
    gmp_randstate_t rand_state;
    gmp_randinit_default(rand_state);
    unsigned long seed = 0;
    getrandom(&seed,sizeof seed,GRND_NONBLOCK);
    gmp_randseed_ui(rand_state,seed);
    do {
        mpz_urandomm(generator.get_mpz_t(),rand_state,mo[0].get_mpz_t());
        mpz_powm(tmp.get_mpz_t(),generator.get_mpz_t(),order.get_mpz_t(),mo[0].get_mpz_t());
    } while (tmp.get_ui() != 1);
    gmp_randclear(rand_state);
    return stu_PublicParameters(mo[0],generator,order);
}

stu_KeyPairs generateDHKeys(const stu_PublicParameters _dhPpara) {
    mpz_class pri,pub;
    gmp_randstate_t rand_state;
    gmp_randinit_default(rand_state);
    unsigned long seed = 0;
    getrandom(&seed,sizeof seed,GRND_NONBLOCK);
    gmp_randseed_ui(rand_state,seed);
    mpz_urandomm(pri.get_mpz_t(),rand_state,_dhPpara.Modulus.get_mpz_t());
    gmp_randclear(rand_state);
    mpz_powm(pub.get_mpz_t(),_dhPpara.Generator.get_mpz_t(),pri.get_mpz_t(),_dhPpara.Modulus.get_mpz_t());
    DiffieHellman::stu_KeyPairs kp(pub,pri);
//    MISC::print_dh_key_pair("HouseHold",kp);
    return kp;
}

}//namespace DiffieHellman


