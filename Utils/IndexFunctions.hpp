#ifndef INDEX_FUNCTIONS_HPP
#define INDEX_FUNCTIONS_HPP
#include <vector>
#include <cstdint>
#include <gmpxx.h>

const size_t binomial_coef_max_height = 1000;

namespace IndexingFunctions{

    std::size_t rank(const std::vector<size_t>& permutation);

    std::size_t rank(const std::vector<unsigned char>& permutation);

    std::vector<unsigned char> unrank(std::size_t rank, std::size_t perm_size);

    std::vector<unsigned char> getInversion(std::vector<unsigned char> permutation);

    size_t toCombinadicBase(const std::vector<size_t>& combination);

    size_t toCombinadicBase(const std::vector<unsigned char>& permutation);

    std::vector<unsigned char> combinationFromRank(size_t rank, size_t k);

    size_t binomialCoef(size_t n, size_t p);

    size_t factorial(size_t n); 

    std::vector<unsigned char> getDual (const std::vector<unsigned char>& perm);

    std::vector<size_t> getDual (const std::vector<size_t>& perm);

    std::vector<std::size_t> sortPermutation(const std::vector<size_t>& vec);

    std::vector<unsigned char> sortPermutation(const std::vector<unsigned char>& vec);

    std::vector<std::size_t> applyPermutation(const std::vector<size_t>& vec, const std::vector<std::size_t>& p);

    std::vector<unsigned char> applyPermutation(const std::vector<unsigned char>& vec, const std::vector<unsigned char>& p);

    void build_binomial_coef_table();
}   

#endif