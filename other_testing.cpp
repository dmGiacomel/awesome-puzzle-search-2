#include "Matrix.hpp"
#include "puzzle_n_minus_1.hpp"
#include "Algorithms/AStar.hpp"
#include "Heuristics/Manhattan.hpp"
#include "Utils/MovementsHandler.hpp"
#include <iostream>

const size_t N_RANDOM_INSTANCES = 50000;
const int rows = 3;
const int columns = 4;
const int N_RANDOM_MOVES = 10000;
const int seed_value = 42;

Puzzle getRandomPuzzleInstance(){
    Puzzle p(rows, columns); 

    for(int i = 0; i < N_RANDOM_MOVES; i++)
        p.makeMove(moves(rand() % 4));

    return p;
}

int main(){


    Puzzle p(3,4);
    Puzzle goal(3,4);

    srand(seed_value);

    Heuristics *manhattan = new Manhattan();

    p.printBoard();

    std::cout << std::endl;

    p = getRandomPuzzleInstance();
    p.printBoard();

    AStar algorithm;
    auto answer = algorithm.solve(p, goal, manhattan);
    auto answer_as_string = MovementsHandler::getOppositeNames(MovementsHandler::getOpposites(answer));

    for (auto &i: answer_as_string){
        std::cout << i << " ";
    }

    std::cout << std::endl;
}