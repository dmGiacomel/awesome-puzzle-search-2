#include "zero_tile_region_mapping.hpp"


//  g++ -Ofast zero_region_test.cpp zero_tile_region_mapping.cpp Utils/IndexFunctions.cpp -o zero_region_test.x

int main(){
    
    ZeroRegionMapping::fillLookupTables(4, 4);
    ZeroRegionMapping z_map(4, 4, 4); 

    std::cout << "\n" << +z_map.getZeroRegion({0, 3, 5, 6, 8, 10, 13}, std::make_tuple(1, 3)) << std::endl;
    std::cout << "\n" << +z_map.getZeroRegion({0, 3, 5, 6, 8, 10, 13}, std::make_tuple(2, 1)) << std::endl;
    std::cout << "\n" << +z_map.getZeroRegion({0, 3, 5, 6, 8, 10, 13}, std::make_tuple(2, 3)) << std::endl;
    std::cout << "\n" << +z_map.getZeroRegion({0, 3, 5, 6, 8, 10, 13}, std::make_tuple(0, 0)) << std::endl;
    std::cout << "\n" << +z_map.getZeroRegion({1, 4}, std::make_tuple(0, 1)) << std::endl;
    std::cout << "\n" << +z_map.getZeroRegion({1, 4}, std::make_tuple(1, 0)) << std::endl;

    std::getchar();

    return 0;
}