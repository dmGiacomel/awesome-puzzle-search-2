#ifndef INDEX_FUNCTIONS_CPP
#define INDEX_FUNCTIONS_CPP
#include "IndexFunctions.hpp"
#include <numeric>
#include <iostream>
#include <gmpxx.h>

size_t IndexingFunctions::factorial(size_t n) {
    size_t result = 1;
    for (size_t i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

//co-lex order
//só deus sabe como isso funciona, mas isso funciona 
//https://computationalcombinatorics.wordpress.com/2012/09/10/ranking-and-unranking-of-combinations-and-permutations/

size_t IndexingFunctions::toCombinadicBase(const std::vector<size_t>& permutation){
    size_t result = 0;
    size_t k = permutation.size();
    
    for (size_t i = 0; i < k; i++){
        //std::cout << "summing this combination:\t" << permutation[i] << "\t" << i + 1;
        //std::cout << "combination result:\t" << binomialCoef(permutation[i], i + 1) << "\n";
        result += binomialCoef(permutation[i], i + 1);
        //std::cout << "\n";
    }

    return result;
}

size_t IndexingFunctions::toCombinadicBase(const std::vector<unsigned char>& permutation){
    size_t result = 0;
    size_t k = permutation.size();
    
    for (size_t i = 0; i < k; i++){
        //std::cout << "summing this combination:\t" << permutation[i] << "\t" << i + 1;
        //std::cout << "combination result:\t" << binomialCoef(permutation[i], i + 1) << "\n";
        result += binomialCoef(permutation[i], i + 1);
        //std::cout << "\n";
    }

    return result;
}


//só deus sabe como isso funciona, mas isso funciona 
//https://computationalcombinatorics.wordpress.com/2012/09/10/ranking-and-unranking-of-combinations-and-permutations/
std::vector<unsigned char> IndexingFunctions::combinationFromRank(size_t m, size_t k){
    std::vector<unsigned char> s(k);
    std::iota(s.begin(), s.end(), 0);

    long long int i = k - 1;

    while(i >= 0){
        long long int l = i;
        //std::cout << "l: " << l << "\ti + 1: " << i + 1 << "\n";
        while (binomialCoef(l, i + 1) <= m){
            l++;
        }
        s[i] = l - 1;
        m -= binomialCoef(l - 1, i + 1);
        i--;
    }
    //std::cout << "terminei antes do segfault!\n";
    return s;
}

static std::vector<std::vector<size_t>> binomial_coef_table;
static bool is_bin_coef_table_calculated = false;

void IndexingFunctions::build_binomial_coef_table(){
    
    binomial_coef_table = std::vector<std::vector<size_t>>(binomial_coef_max_height);
    for (size_t i = 0; i < binomial_coef_max_height; i++){
        binomial_coef_table[i] = std::vector<size_t>(i + 1);
    }

    for (size_t i = 0; i < binomial_coef_max_height; i++){
        binomial_coef_table[i][0] = 1;
        binomial_coef_table[i][binomial_coef_table[i].size() - 1] = 1;
    }
    
    // pascal's triangle level
    for (size_t i = 2; i < binomial_coef_max_height; i++){
        
        // iterator inside the list
        size_t rightmost_limit = binomial_coef_table[i].size() - 1;
        for(size_t j = 1; j < rightmost_limit; j++){
            binomial_coef_table[i][j] = binomial_coef_table[i - 1][j - 1] + binomial_coef_table[i - 1][j];
        }
    }
    
}

size_t IndexingFunctions::binomialCoef(size_t n, size_t k){

    //std::cout << "combination i'm trying to calculate:\t" << n << "\t" << k << std::endl << "\n";
    if (k < 0 || k > n) {
        return 0;
    }
    if (k == 0 || k == n) {
        return 1;
    }
    if(is_bin_coef_table_calculated){
        return binomial_coef_table[n][k];
    }else{
        build_binomial_coef_table();
        is_bin_coef_table_calculated = true;
        return binomial_coef_table[n][k];
    }
    // size_t p = std::min(k, n - k);
    // size_t res = 1;
    // for (size_t i = 0; i < p; i++){
    //     res *= (n - i);
    //     res /= i + 1;
    // }
    //return res;
}

std::vector<unsigned char> IndexingFunctions::getDual (const std::vector<unsigned char>& perm){
    size_t perm_size = perm.size();
    std::vector<unsigned char> dual(perm_size);

    for (size_t i = 0; i < perm_size; i++){
        dual[perm[i]] = i;
    }

    return dual;
}


std::vector<size_t> IndexingFunctions::getDual (const std::vector<size_t>& perm){
    size_t perm_size = perm.size();
    std::vector<size_t> dual(perm_size);

    for (size_t i = 0; i < perm_size; i++){
        dual[perm[i]] = i;
    }

    return dual;
}

size_t raw_rank (size_t perm_size, std::vector<size_t>& perm, std::vector<size_t>& inv){

    if (perm_size == 1){
        return 0;
    }
    size_t current_index = perm_size - 1;
    size_t aux = perm[current_index];

    std::swap(perm[current_index], perm[inv[current_index]]);
    std::swap(inv[aux], inv[current_index]);
    return (aux + perm_size * raw_rank(perm_size - 1, perm, inv));
}

size_t IndexingFunctions::rank(const std::vector<size_t>& perm){

    std::vector<size_t> perm_copy(perm);
    std::vector<size_t> inv = getDual(perm);

    return raw_rank(perm.size(), perm_copy, inv);
}


size_t raw_rank (size_t perm_size, std::vector<unsigned char>& perm, std::vector<unsigned char>& inv){

    if (perm_size == 1){
        return 0;
    }
    size_t current_index = perm_size - 1;
    size_t aux = perm[current_index];

    std::swap(perm[current_index], perm[inv[current_index]]);
    std::swap(inv[aux], inv[current_index]);
    return (aux + perm_size * raw_rank(perm_size - 1, perm, inv));
}

size_t IndexingFunctions::rank(const std::vector<unsigned char>& perm){

    std::vector<unsigned char> perm_copy(perm);
    std::vector<unsigned char> inv = getDual(perm);

    return raw_rank(perm.size(), perm_copy, inv);
}

void unrank_raw (size_t perm_size, size_t rank, std::vector<unsigned char>& id){

    if (perm_size > 0){
        std::swap(id[perm_size - 1], id[rank % perm_size]);
        unrank_raw(perm_size - 1, rank/perm_size, id);
    }
}

std::vector<unsigned char> IndexingFunctions::unrank(size_t rank, size_t perm_size){
    std::vector<unsigned char> id(perm_size);
    std::iota(id.begin(), id.end(), 0 );
    unrank_raw(perm_size, rank, id);
    return id;
}

// std::vector<unsigned char> IndexingFunctions::getInversion(const std::vector<unsigned char>& permutation){
//     size_t k{permutation.size()};
//     std::vector<unsigned char> inversion_vector(k);

//     mpz_class m;
//     mpz_ui_pow_ui(m.get_mpz_t(), 2, k);
//     m = (mpz_class(1) << k) - mpz_class(1); 

//     for (size_t i = 0; i < k; i++){

//         mpz_class bitmask = mpz_class(1) << permutation[i]; 
//         mpz_class popcount = (mpz_popcount(m.get_mpz_t())) & bitmask - mpz_class(1); 
//         inversion_vector[i] = static_cast<unsigned char>(popcount.get_ui());

//         m &= ~(bitmask);
//     }
    
//     return inversion_vector;
// }


// due to https://stackoverflow.com/users/1828879/timothy-shields-----------------------------------------------------------------------
// https://stackoverflow.com/questions/17074324/how-can-i-sort-two-vectors-in-the-same-way-with-criteria-that-uses-only-one-of

std::vector<std::size_t> IndexingFunctions::sortPermutation(const std::vector<size_t>& vec){
    std::vector<std::size_t> p(vec.size());
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(),
        [&](std::size_t i, std::size_t j){ return vec[i] < vec[j]; });
    return p;
}

std::vector<unsigned char> IndexingFunctions::sortPermutation(const std::vector<unsigned char>& vec){

    std::vector<unsigned char> p(vec.size());
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(),
        [&](std::size_t i, std::size_t j){ return vec[i] < vec[j]; });
    return p;
}

std::vector<size_t> IndexingFunctions::applyPermutation(const std::vector<size_t>& vec, const std::vector<std::size_t>& p)
{
    std::vector<size_t> sorted_vec(vec.size());
    for (std::size_t i = 0; i < vec.size(); ++i) {
        sorted_vec[i] = vec[p[i]];
    }
    return sorted_vec;
}


std::vector<unsigned char> IndexingFunctions::applyPermutation(const std::vector<unsigned char>& vec, const std::vector<unsigned char>& p)
{    
    std::vector<unsigned char> sorted_vec(vec.size());
    for (std::size_t i = 0; i < vec.size(); ++i) {
        sorted_vec[i] = vec[p[i]];
    }

    return sorted_vec;
}
#endif