#include "AdditivePDB.hpp"

int main(){
    ZeroRegionMapping::fillLookupTables(4, 4);

    APDB *apdb = new APDB();
    apdb->build(Puzzle(4,4), Puzzle(4,4), {7,11,12,13,14,15});
    return 0;
}