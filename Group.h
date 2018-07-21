#ifndef GROUP_H
#define GROUP_H
#include <cryptopp/dh.h>
#include "HouseHold.h"
namespace PrivateDataAgg {
class clsGroup
{
public:
    clsGroup();
    void init(const unsigned int _noOfHHs,const unsigned int _i);
    mpz_class collectData();
    void clean();
private:
    void pairHHs();    
    std::vector<clsHouseHold> HHs;
    unsigned int ActiveHousHoldes;
    unsigned int ID;
};
}
#endif // GROUP_H
