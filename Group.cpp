#include <cryptopp/osrng.h>
#include <iostream>
#include "Group.h"
namespace PrivateDataAgg {
clsGroup::clsGroup() {


}

void clsGroup::init(const unsigned int _noOfHHs, const unsigned int _i) {
    this->ID = _i;   
    this->ActiveHousHoldes = _noOfHHs;
    this->HHs.resize(_noOfHHs);
    Public::INFO::Instance().updateGroup(this->ID);
    this->pairHHs();
}

mpz_class clsGroup::collectData() {
    mpz_class result = 1;
    mpz_class n_2 = Public::INFO::Instance().getUP_PK(this->ID).n2;
    for (unsigned int i = 0; i < this->ActiveHousHoldes; i ++) {
        mpz_class HH_c = this->HHs[i].reportData();
        mpz_mul(result.get_mpz_t(),result.get_mpz_t(),HH_c.get_mpz_t());        
        mpz_mod(result.get_mpz_t(),result.get_mpz_t(),n_2.get_mpz_t());
    }
    return result;
}

void clsGroup::clean() {
    this->HHs.clear();
}

void clsGroup::pairHHs() {
    if (this->ActiveHousHoldes  < 2); //WTF one memeber in a group?!
    else {
        for (unsigned int i = 0; i < this->ActiveHousHoldes ; i += 2) {
            if (i + 1 == this->ActiveHousHoldes )
                this->HHs[i-1].pair(&this->HHs[i],this->ID, i, true); // the last one is single ODD
            else
                this->HHs[i].pair(&this->HHs[i+1],this->ID, i, false);
        }
    }
}
}
