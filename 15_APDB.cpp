#include "puzzle_n_minus_1.hpp"

#include "Utils/MovementsHandler.hpp"
#include "Utils/IndexFunctions.hpp"

// #include "Heuristics/PDB.hpp"
#include "Heuristics/AdditivePDB.hpp"
#include "Heuristics/APDBSet.hpp"
// #include "Heuristics/Manhattan.hpp"
// #include "Heuristics/MisplacedTileCount.hpp"
// #include "Heuristics/Zero.hpp"

#include "Algorithms/AStar.hpp"
#include "Algorithms/IDAStar.hpp"

// g++ -Ofast -fopenmp 15_PDB.cpp Algorithms/IDAStar.cpp Heuristics/Heuristics.cpp Heuristics/PDB.cpp Algorithms/SearchNode.cpp Algorithms/AStar.cpp Utils/IndexFunctions.cpp Utils/MovementsHandler.cpp puzzle_n_minus_1.cpp PDBAbstractedPuzzle.cpp -o 8_PDB.x

const size_t N_RANDOM_INSTANCES = 200;
const int rows = 4;
const int columns = 4;
const int N_RANDOM_MOVES = 1000;
const int seed_value = 42;

Puzzle getRandomPuzzleInstance()
{
    Puzzle p(rows, columns);

    for (int i = 0; i < N_RANDOM_MOVES; i++)
        p.makeMove(moves(rand() % 4));

    return p;
}

int main(int argc, char **argv)
{

    Puzzle initial_state(rows, columns);
    Puzzle goal_state(rows, columns);

    srand(seed_value);

    APDBSet *apdb = new APDBSet();
    apdb->build(initial_state, goal_state, {{1,4,5,8}, {3,7,11,12,13,14,15}, {2,6,9,10}});

    // saida em linha separado por espaços, uma linha para cada execucao
    // <iteracao> <runtime> <estados gerados> <heuristica media> <sol_size>
    for (size_t i = 0; i < N_RANDOM_INSTANCES; i++)
    {
        AStar *a = new AStar();
        initial_state = getRandomPuzzleInstance();
        std::cout << i << " ";
        auto result = a->solve(initial_state, goal_state, apdb);
        std::cout << " " << result.size();
        std::cout << std::endl;
        delete a;
    }

    std::cout << "-" << std::endl;
    srand(seed_value);

    for (size_t i = 0; i < N_RANDOM_INSTANCES; i++)
    {
        IDAStar *ida = new IDAStar();

        initial_state = getRandomPuzzleInstance();
        std::cout << i << " ";
        auto result = ida->solve(initial_state, goal_state, apdb);
        std::cout << " " << result.size();
        std::cout << std::endl;
        // std::cin.get();
        delete ida;
    }

    delete apdb;
    return 0;
}