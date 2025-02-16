#include "../puzzle_n_minus_1.hpp"

#include "../Utils/MovementsHandler.hpp"
#include "../Utils/IndexFunctions.hpp"

#include "PDB.hpp"
// #include "Heuristics/APDB.hpp"
// #include "Heuristics/APDBSet.hpp"
// #include "Heuristics/Manhattan.hpp"
// #include "Heuristics/MisplacedTileCount.hpp"
// #include "Heuristics/Zero.hpp"

// #include "Algorithms/AStar.hpp"
// #include "Algorithms/IDAStar.hpp"

//g++ -Ofast -fopenmp 15_PDB_SAMPLING.cpp Heuristics.cpp PDB.cpp  ../Utils/IndexFunctions.cpp ../Utils/MovementsHandler.cpp ../puzzle_n_minus_1.cpp ../PDBAbstractedPuzzle.cpp -o 15_PDB_SAMPLING.x
//time ./15_PDB_SAMPLING.x > 15_PDB_SAMPLING.txt

const size_t N_RANDOM_INSTANCES = 100000000;
const int rows = 4;
const int columns = 4;
const int N_RANDOM_MOVES = 1000;
const int seed_value = 42;

Puzzle getRandomPuzzleInstance(){
    Puzzle p(rows, columns); 

    for(int i = 0; i < N_RANDOM_MOVES; i++)
        p.makeMove(moves(rand() % 4));

    return p;
}

int main (int argc, char **argv){

    Puzzle initial_state(rows, columns);
    Puzzle goal_state(rows, columns);

    srand(seed_value);

    PDB *pdb = new PDB();
    pdb->build(initial_state, goal_state, {15,14,13,12,11,10,9,8});

    for (size_t i = 0; i < N_RANDOM_INSTANCES; i++){
        initial_state = getRandomPuzzleInstance();
        std::cout << i << "\n";
    }

    delete pdb;
}