#include <iostream>
#include <vector>
#include <string>

unsigned long long fact (unsigned long long n){
    return (n == 0 || n == 1) ? 1 : n * fact(n - 1);
}

//could be done in a simpler way, it's done this way so as to avoid overflow as much as possible
unsigned long long arrange (unsigned long long n, unsigned long long p){

    return fact(n)/fact(n - p);
}

size_t rank (size_t perm_size, std::vector<unsigned char>& perm, std::vector<unsigned char>& inv){

    if (perm_size == 1){
        return 0;
    }
    size_t current_index = perm_size - 1;
    unsigned char aux = perm[current_index];

    std::swap(perm[current_index], perm[inv[current_index]]);
    std::swap(inv[aux], inv[current_index]);
    return (aux + perm_size * rank(perm_size - 1, perm, inv));
}

/*
size_t rankArrangment(size_t n_positions, std::vector<unsigned char>& perm, std::vector<unsigned char>& inv){

}*/

void unrank (size_t perm_size, size_t rank, std::vector<unsigned char>& id){

    if (perm_size > 0){
        std::swap(id[perm_size - 1], id[rank % perm_size]);
        unrank(perm_size - 1, rank/perm_size, id);
    }
}

size_t rankArr(size_t n, size_t p, std::vector<unsigned char>& perm, std::vector<unsigned char>& inv){

    return
}

//perm inv refers only to the pdb tiles
size_t rankArrangement(size_t n, size_t p, std::vector<unsigned char>& perm_inv){

    //arbitrar
    std::vector<unsigned char> 
    return rankArr(p, perm_inv);
}

void unrankArrangement (size_t n, size_t p, size_t rank, std::vector<unsigned char>& id){
    if (n > 0 && p > 0){
        std::swap(id[n - 1], id[rank % n]);
        unrankArrangement(n - 1, p - 1, rank/n, id);
    }
}

std::vector<unsigned char> unrankArrangement (size_t n, size_t p, size_t rank){
    std::vector<unsigned char> id(n);
    for (size_t i = 0; i < n; i++){
        id[i] = i;
    }
    
    unrankArrangement(n, p, rank, id);
    return std::vector<unsigned char>(id.begin() + (n-p), id.end());
}


std::vector<unsigned char> getDual (std::vector<unsigned char> perm){
    size_t perm_size = perm.size();
    std::vector<unsigned char> dual(perm_size);

    for (size_t i = 0; i < perm_size; i++){
        dual[perm[i]] = i;
    }

    return dual;
}

void printVector (std::vector<unsigned char> v){

    for (auto i : v){
        std::cout << +i << "\t"; 
    }
    std::cout << std::endl;
}

int main(int argc, char **argv){

    //size_t r = std::atoi(argv[1]);

    size_t n = std::atoi(argv[1]);
    size_t p = std::atoi(argv[2]);

    //actual permutation
    std::vector<unsigned char> id({0, 1, 2, 3});

    //vector that labels the pdb tiles (in the overall permutation, where are those tiles?)
    //dual for the pdb of {2, 3}
    std::vector<unsigned char> pdb_sim({2, 3});
    
    //testar arrange unranking (should generate every possible permutation of p elements choosen in n elements)
    size_t n_perm = arrange(n,p);
    for(size_t rank = 0; rank < n_perm; rank++){
        std::cout << "rank " << rank << ": ";
        printVector(unrankArrangement(n, p, rank));
        std::cout << "\n";
    }


    /*
    std::cout << "rank: " << r << "\n";
    unrank(id.size(), r, id);

    std::cout << "corresponding permutation: " << "\n"; 
    printVector(id);

    std::vector<unsigned char> dual = getDual(id);
    std::cout << "ranking the permutation (should be the same initial value): " << rank(id.size(), id, dual) << "\n";
    */
    return 0;
}