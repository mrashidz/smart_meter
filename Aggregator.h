#ifndef AGGREGATOR_H
#define AGGREGATOR_H
#include "Group.h"
namespace PrivateDataAgg {
class clsAggregator {
public:
    clsAggregator(){}
    void init(const _uint_64_t _K,const _uint_64_t _L);
    mpz_class collectData();
    void clean();    
    mpz_class collectData_DiscardGroup(const _uint_64_t _gid);
private:
    std::vector<clsGroup> Groups;
};
}
#endif // AGGREGATOR_H
