#ifndef MANHATTAN_CPP
#define MANHATTAN_CPP
#include "Manhattan.hpp"

int Manhattan::evaluate (const Puzzle& puzzle_state){

    int estimate = 0;
    int columns = puzzle_state.getBoard().getColumns();
    int rows = puzzle_state.getBoard().getRows();

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            unsigned char current_tile = puzzle_state.getTileAt(i, j);
            if (current_tile != 0){
                estimate += std::abs(i - current_tile/columns) + std::abs(j - current_tile % columns);
            }
        }
    }

    return estimate;
}

bool Manhattan::build (const Puzzle& initial_state, const Puzzle& goal_state){
    return true;
}

Manhattan::Manhattan(){

}

Manhattan::~Manhattan(){

}

#endif