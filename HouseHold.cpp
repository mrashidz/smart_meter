#include "HouseHold.h"
#include <iostream>
#include <stdexcept>
using std::runtime_error;
//#include <cryptopp/secblock.h>


namespace PrivateDataAgg {
clsHouseHold::clsHouseHold(): isDummy(false),hasDummy(false),n_alfa(0),n_alfa_DUMMY(0){
}

clsHouseHold::~clsHouseHold()
{
//    this->n_alfa.
}

void clsHouseHold::pair(clsHouseHold *_pair, unsigned int _gid, unsigned int _id, bool _dummy) {
    this->ID = _id;
    this->hasDummy = _dummy;
    this->GroupID = _gid;
//    MISC::print_pairing(_id,_id+1,_gid);
    DiffieHellman::stu_PublicParameters dhPpara = DiffieHellman::generate_public_parameters(DH_KEY_LEN);
    DiffieHellman::stu_KeyPairs thisDH = DiffieHellman::generateDHKeys(dhPpara);
    mpz_class pairPubKeys = _pair->handshake(dhPpara, thisDH.PUB,_id+1,this->GroupID);
    if (_dummy)
        this->n_alfa_DUMMY = this->handshakeResponse(pairPubKeys,thisDH.PRI,dhPpara.Modulus);
    else
        this->n_alfa = this->handshakeResponse(pairPubKeys,thisDH.PRI,dhPpara.Modulus);
}

mpz_class clsHouseHold::handshake(DiffieHellman::stu_PublicParameters _dhPpara, mpz_class _pairDHPub, _uint_64_t _id, _uint_64_t _gid) {
    mpz_class alfa;
    this->GroupID = _gid;
    this->ID = _id;
    DiffieHellman::stu_KeyPairs thisDH = DiffieHellman::generateDHKeys(_dhPpara);
    mpz_powm(alfa.get_mpz_t(),_pairDHPub.get_mpz_t(),thisDH.PRI.get_mpz_t(),_dhPpara.Modulus.get_mpz_t());
    mpz_sub(this->n_alfa.get_mpz_t(),
            Public::INFO::Instance().getUP_PK(this->GroupID).n.get_mpz_t(),
            alfa.get_mpz_t());
//    if (isDummy)
//        MISC::print_HH_info(this->GroupID,this->ID,alfa,this->n_alfa);
    return thisDH.PUB;
}

mpz_class clsHouseHold::handshakeResponse(mpz_class _pairPUB, mpz_class _myPRI, mpz_class _mod)
{
    mpz_class alfa, n_alfa;
    mpz_powm(alfa.get_mpz_t(),_pairPUB.get_mpz_t(),_myPRI.get_mpz_t(),_mod.get_mpz_t());
    mpz_add(n_alfa.get_mpz_t(),
            Public::INFO::Instance().getUP_PK(this->GroupID).n.get_mpz_t(),
            alfa.get_mpz_t());
    return n_alfa;
}

mpz_class clsHouseHold::reportData()
{
    unsigned short m = 0xFFFF;
    std::string time = this->getDateTime();
    mpz_class ci = Paillier::encrypt_with_hash(m,time,this->n_alfa,
                                              Public::INFO::Instance().getUP_PK(this->GroupID));
    if (hasDummy)
        return this->reportData_DUMMY(ci);
    else
        return ci;


}




mpz_class clsHouseHold::reportData_DUMMY(mpz_class _ci)
{
    std::string time = this->getDateTime();
    mpz_class cipher = Paillier::encrypt_with_hash(0,time,this->n_alfa_DUMMY,Public::INFO::Instance().getUP_PK(this->GroupID));
    mpz_mul(cipher.get_mpz_t(), _ci.get_mpz_t(),cipher.get_mpz_t());
    mpz_mod(cipher.get_mpz_t(),cipher.get_mpz_t(),Public::INFO::Instance().getUP_PK(this->GroupID).n2.get_mpz_t());
    return cipher;
}

std::string clsHouseHold::getDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return "2017-05-11.12:00:00";
    return buf;
}

}
