#include "IndexFunctions.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

void printVector(const std::vector<unsigned char>& v){

    for (auto i : v){
        std::cout << "\t" << +i;
    }
    std::cout << std::endl;
}

int main(){

    //---------------------tests for ranking permutations ----------------------------------------------------------------------------------
    //40320 = 8! 

    // std::vector<std::vector<unsigned char>> permutation_set(40320);
    // int errors = 0;

    // //iterating over possible indexes
    // for (auto i = 0; i < 40320; i++){

    //     std::cout << "test iteration: " << i << "\t\t---------------\n";
    //     //getting permutation that matches said index
    //     auto permutation = IndexingFunctions::unrank(i, 8);
    //     printVector(permutation);
    //     //if it has already been generated, tell me
    //     if (std::find(permutation_set.cbegin(), permutation_set.cend(), permutation) != permutation_set.cend()){
    //         std::cout << "Permutation already generated: \tindex: " << i <<std::endl;
    //         printVector(permutation);
    //         std::cout << std::endl;

    //         errors++;
    //         std::cin.get();
    //     }

    //     permutation_set[i] = permutation;

    //     auto rank_from_permutation = IndexingFunctions::rank(permutation);

    //     if (rank_from_permutation != i){
    //         std::cout << "ranking and unranking not matching results ---------------" << std::endl;
    //         std::cout << "expected: " << i << "\t" << "actual: " << rank_from_permutation << "\t" << std::endl;
    //         printVector(permutation);
    //         std::cout << std::endl;

    //         errors++;
    //         std::cin.get();
    //     }

    // }

    // std::cout << "end of testing for ranking functions\n";
    // std::cout << "errors found: " << errors << std::endl;

    //(15,8) = 6435

    std::vector<std::vector<unsigned char>> permutation_set(6435);
    int errors = 0;

    //iterating over possible indexes
    for (auto i = 0; i < 6435; i++){

        std::cout << "test iteration: " << i << "\t\t---------------\n";
        //getting permutation that matches said index
        auto permutation = IndexingFunctions::combinationFromRank(i, 8);
        printVector(permutation);

        //if it has already been generated, tell me
        if (std::find(permutation_set.cbegin(), permutation_set.cend(), permutation) != permutation_set.cend()){
            std::cout << "Permutation already generated: \tindex: " << i <<std::endl;
            printVector(permutation);
            std::cout << std::endl;

            errors++;
            std::cin.get();
        }

        permutation_set[i] = permutation;

        auto rank_from_permutation = IndexingFunctions::toCombinadicBase(permutation);

        if (rank_from_permutation != i){
            std::cout << "ranking and unranking not matching results ---------------" << std::endl;
            std::cout << "expected: " << i << "\t" << "actual: " << rank_from_permutation << "\t" << std::endl;
            printVector(permutation);
            std::cout << std::endl;

            errors++;
            std::cin.get();
        }

    }

    std::cout << "end of testing for ranking functions\n";
    std::cout << "errors found: " << errors << std::endl;

    return 0;
}