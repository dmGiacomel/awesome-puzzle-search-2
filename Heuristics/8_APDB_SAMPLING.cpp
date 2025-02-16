#include "../puzzle_n_minus_1.hpp"

#include "../Utils/MovementsHandler.hpp"
#include "../Utils/IndexFunctions.hpp"

// #include "PDB.hpp"
#include "AdditivePDB.hpp"
#include "APDBSet.hpp"
// #include "Heuristics/Manhattan.hpp"
// #include "Heuristics/MisplacedTileCount.hpp"
// #include "Heuristics/Zero.hpp"

// #include "Algorithms/AStar.hpp"
// #include "Algorithms/IDAStar.hpp"

//g++ -Ofast -fopenmp 8_APDB_SAMPLING.cpp Heuristics.cpp APDBSet.cpp AdditivePDB.cpp ../Utils/IndexFunctions.cpp ../Utils/MovementsHandler.cpp ../puzzle_n_minus_1.cpp ../APDBAbstractedPuzzle.cpp -o 8_APDB_SAMPLING.x

//time ./8_APDB_SAMPLING.x > 8_APDB_SAMPLING.txt

const size_t N_RANDOM_INSTANCES = 50000;
const int rows = 3;
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

    APDBSet *apdb = new APDBSet();
    apdb->build(initial_state, goal_state, {{1, 3, 4}, {2, 5, 6, 7, 8}});

    for (size_t i = 0; i < N_RANDOM_INSTANCES; i++){
        initial_state = getRandomPuzzleInstance();
        std::cout << apdb->evaluate(initial_state) << "\n";
    }

    delete apdb;
}