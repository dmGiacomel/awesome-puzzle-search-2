#include <iostream>
#include "puzzle_n_minus_1.hpp"
#include "Algorithms/AStar.hpp"
#include "Heuristics/Zero.hpp"
#include "Heuristics/Manhattan.hpp"
#include <string>
#include <random>

namespace MovementsHandler{

    moves getOpposite(moves m){
        switch(m){
            case left: 
                return right;
                break;
            case right:
                return left;
                break;
            case down: 
                return up;
                break;
            case up:
                return down;
                break;
            default: 
                return none;
                break;
        };
    }

    std::string getName(moves m){
        switch(m){
            case left: 
                return "L";
                break;
            case right:
                return "R";
                break;
            case down: 
                return "D";
                break;
            case up:
                return "U";
                break;
            default: 
                return "none";
                break;
        };
    }

    std::list<moves> getOpposites(std::list<moves> moves_list){
        
        std::list<moves> opposites;

        for (auto i : moves_list){
                opposites.push_back(getOpposite(i));
        }

        return opposites;
    }


    std::list<std::string> getNames(std::list<moves> moves_list){

        std::list<std::string> moves_names;
        for (auto i : moves_list){
            
            moves_names.push_back(getName(i));
        
        }

        return moves_names;
    }

    std::list<std::string> getOppositeNames(std::list<moves> moves_list){

        return getNames(getOpposites(moves_list));
    }
}

int main(){
    
    Puzzle p(4,4);
    Puzzle goal(4,4);

    Heuristics* zero = new Manhattan();
    AStar a_estrela;

    //arbitrary seed to generate always the same random
    unsigned int seed = 42;
    srand(seed);
    int random_moves = 1000000;
    
    for(int i = 0; i < random_moves; i++)
        p.makeMove(moves(rand() % 4));
    

    std::cout << "Initial State: \n";
    p.printBoard();
    std::cout << std::endl;

    std::cout << "GoalState: \n";
    goal.printBoard();
    std::cout << std::endl;

    std::list<std::string> sequence = MovementsHandler::
                                      getNames(a_estrela.solve(p, goal, zero));
    std::cout << "Solution sequence: ";
    for (auto i : sequence){
        std::cout << i << " ";
        //p.makeMove(MovementsHandler::getOpposite());
    }
    std::cout << std::endl;

    //Matrix<unsigned char> m(3,3);
    //m.printMatrix();

    //Puzzle p(3,3);
    //p.printBoard();  
   
    return 0;
}