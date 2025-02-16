#include "APDBAbstractedPuzzle.hpp"

// g++ apdb_abstraction_test.cpp puzzle_n_minus_1.cpp zero_tile_region_mapping.cpp Utils/IndexFunctions.cpp APDBAbstractedPuzzle.cpp -o apdb_abstraction_test.x


int main (){

    ZeroRegionMapping zero_map(4, 4, 7);

    APDBAbstraction abs1({0, 1, 2, 3, 4, 5, 6}, {1, 4, 5, 6, 7, 9, 13}, 4, 4, std::make_tuple(0, 2));
    APDBAbstraction abs2({0, 1, 2, 3, 4, 5, 6}, {1, 4, 5, 6, 7, 9, 13}, 4, 4, std::make_tuple(0, 3));
    APDBAbstraction abs3({0, 1, 2, 3, 4, 5, 6}, {1, 4, 5, 6, 7, 9, 13}, 4, 4, std::make_tuple(2, 2));
    APDBAbstraction abs4({0, 1, 2, 3, 4, 5, 6}, {1, 4, 5, 6, 7, 9, 13}, 4, 4, std::make_tuple(3, 2));
    APDBAbstraction abs5({0, 1, 2, 3, 4, 6, 5}, {1, 4, 5, 6, 7, 9, 13}, 4, 4, std::make_tuple(3, 2));

    // std::cout << (abs1 == abs2) << std::endl; //true
    // std::cout << (abs1 == abs3) << std::endl; //false
    // std::cout << (abs3 == abs4) << std::endl; //true
    // std::cout << (abs4 == abs5) << std::endl; //false 

    std::cout << "original: \n";
    abs5.printAbstraction(); 

    std::cout << "neighbours: \n";
    auto neighbourhood = abs5.neighbourhood();
    for(auto n: neighbourhood){
        n.printAbstraction();
    }
}