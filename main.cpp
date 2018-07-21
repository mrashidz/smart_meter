#include "UtilityProvider.h"

void help() {
    std::cout << "1 - Demo Paillier Cryptosystem:" << std::endl;
    std::cout << "./3rd_assignment 1 [key_size] [message]" << std::endl <<std::endl;
    std::cout << "2 - Demo the Protocol with auto_config." << std::endl;
    std::cout << "./3rd_assignment 2 [TOTAL_HouseHolds] [number_of_groups]" << std::endl <<std::endl;
    std::cout << "3 - Demo the Protocol with manual_config." << std::endl;
    std::cout << "./3rd_assignment 3 [TOTAL_HouseHolds] [number_of_groups]";
    std::cout << " [BETA_size_in_bits] [pk_size_in_bits] [Paillier_key_size_in_bits]" << std::endl << std::endl;
    std::cout << "4 - Demo the Protocol -- discarding a group in calculating the result." << std::endl;
    std::cout << "./3rd_assignment 4 [TOTAL_HouseHolds] [number_of_groups] [group_number_wish_to_discard]" << std::endl << std::endl;
}

void demo_Paillier(mpz_class _message, unsigned int _bits) {
    auto t1 = Timer::now();
    Paillier::KeyPairs k = Paillier::generate_keys(_bits);
    auto t2 = Timer::now();
    mpz_class cipher = Paillier::encrypt(_message,k.PUB);
    auto t3 = Timer::now();
    mpz_class plain = Paillier::decrypt(cipher,k);
    auto t4 = Timer::now();

    MISC::print_int("MSG",_message);
    MISC::print_pailler_key_pair("KEY",k);
    MISC::print_time("Key generation took: ", t2-t1);
    MISC::print_int("CIPHER",cipher);
    MISC::print_time("Encryption took: ", t3-t2);
    MISC::print_int("PLAIN",plain);
    MISC::print_time("Decryption took: ", t4-t3);
    MISC::print_time("Total time", t4-t1);
}

void demo_Protocol(unsigned int _N, unsigned int _K)  {
    PrivateDataAgg::UtilityProvider UP;
    UP.configureSetup(_N,_K);
    UP.setupSim();
    UP.runSim();
}

void demo_Protocol(unsigned int _N, unsigned int _K, unsigned int _gid)  {
    PrivateDataAgg::UtilityProvider UP;
    UP.configureSetup(_N,_K);
    UP.setupSim();
    UP.runSim_discardGroup(_gid);
}

void demo_Protocol(unsigned int _N, unsigned int _K, unsigned int _beta, unsigned _n, unsigned int _pk) {
    PrivateDataAgg::UtilityProvider UP;
    UP.configureSetup(_N,_K,_beta,_n,_pk);
    UP.setupSim();
    UP.runSim();
}

int main(const int argc,const char **argv) {
    if (argc < 3)
        help();
     _uint_64_t opt = strtol(argv[1], NULL, 10);
    if (opt==1) {
        _uint_64_t n = strtol(argv[2], NULL, 10);
        mpz_class m(argv[3]);
        demo_Paillier(m,n);
    } else if (opt==2) {
        _uint_64_t N = strtol(argv[2], NULL, 10);
        _uint_64_t K = strtol(argv[3], NULL, 10);
        demo_Protocol(N,K);
    } else if (opt==3) {
        _uint_64_t N = strtol(argv[2], NULL, 10);
        _uint_64_t K = strtol(argv[3], NULL, 10);
        _uint_64_t b = strtol(argv[4], NULL, 10);
        _uint_64_t p = strtol(argv[5], NULL, 10);
        _uint_64_t n = strtol(argv[6], NULL, 10);
        demo_Protocol(N,K,b,n,p);
    } else if (opt==4) {
        _uint_64_t N = strtol(argv[2], NULL, 10);
        _uint_64_t K = strtol(argv[3], NULL, 10);
        _uint_64_t g = strtol(argv[4], NULL, 10);
        demo_Protocol(N,K,g);
    } else
        help();
    return 0;
}
