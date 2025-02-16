#include "puzzle_n_minus_1.hpp"

#include "Utils/MovementsHandler.hpp"
#include "Utils/IndexFunctions.hpp"

#include "Heuristics/PDB.hpp"
#include "Heuristics/APDB.hpp"
#include "Heuristics/APDBSet.hpp"
#include "Heuristics/Manhattan.hpp"
#include "Heuristics/MisplacedTileCount.hpp"
#include "Heuristics/Zero.hpp"

#include "Algorithms/AStar.hpp"
#include "Algorithms/IDAStar.hpp"

int main (int argc, char **argv){

    Puzzle initial_state(4,4);
    Puzzle goal_state(4,4);

    unsigned int seed = 42;
    srand(seed);
    int random_moves = 1000000;
    
    for(int i = 0; i < random_moves; i++)
        initial_state.makeMove(moves(rand() % 4));

    APDBSet *pdb = new APDBSet();
    initial_state = Puzzle({15, 7, 5, 4, 1, 11 ,12, 0, 10, 8, 9, 6, 14 ,3 ,13, 2}, 4, 4);
    //PDB *pdb = new PDB();
    //pdb->build(initial_state, goal_state, {{15, 14, 13, 12, 7}, {6, 5, 4}, {1, 2}, {8,9,10}, {3, 11 }});
    pdb->build(initial_state, goal_state, {{1, 2, 3}, {4, 5, 9}, {6, 7, 10}, {8, 12, 13, 14, 15, 11}, {2, 3, 6, 7, 10}});
    
    //pdb->build(initial_state, goal_state, {15,14,13,12,11,7});
    //Manhattan *pdb = new Manhattan();
    std::cout << "Initial State: \n";
    initial_state.printBoard();
    std::cout << std::endl;

    std::cout << "GoalState: \n";
    goal_state.printBoard();
    std::cout << std::endl;

    IDAStar *ida = new IDAStar();

    std::list<std::string> sequence = MovementsHandler::
                                      getNames(ida->solve(initial_state, goal_state, pdb));
    std::cout << "Solution sequence: ";
    for (auto i : sequence){
        std::cout << i << " ";
        //p.makeMove(MovementsHandler::getOpposite());
    }
    std::cout << std::endl;

    delete pdb; 
    delete ida;
    return 0;
}