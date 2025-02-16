#ifndef ZERO_CPP
#define ZERO_CPP 
#include "Zero.hpp"

Zero::Zero(){
    //std::cout << "Zero Heuristic Built\n";
    //current_heuristic_state = HeuristicState::READY;   
    
}

Zero::~Zero(){

    //std::cout << "Zero Heuristic Destroyed\n";
}

int Zero::evaluate(const Puzzle &puzzle_state){
    return 0;
}

bool Zero::build(const Puzzle& initial_state, const Puzzle& goal_state){
    return true;
}
#endif