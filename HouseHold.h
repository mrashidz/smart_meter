#ifndef HOUSEHOLD_H
#define HOUSEHOLD_H
#include "Global.h"
namespace PrivateDataAgg {
class clsHouseHold {    
public:
    clsHouseHold();
    ~clsHouseHold();
    mpz_class handshake(DiffieHellman::stu_PublicParameters _dhPpara, mpz_class _pairDH, _uint_64_t _id, _uint_64_t _gid);
    void pair(clsHouseHold *_pair, unsigned int _gid, unsigned int _id, bool _dummy);
    mpz_class handshakeResponse(mpz_class _pairPUB, mpz_class _myPRI, mpz_class _mod);
    mpz_class reportData_DUMMY(mpz_class _ci);
    mpz_class reportData();
private:
    std::string getDateTime();
    unsigned long long GroupID;
    unsigned long long ID;
    mpz_class n_alfa_DUMMY;    ///needed?!
    mpz_class n_alfa;
    bool hasDummy;
    bool isDummy;
};
}
#endif // HOUSEHOLD_H

