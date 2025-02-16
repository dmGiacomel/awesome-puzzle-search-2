#ifndef MISPLACED_TILE_COUNT_CPP
#define MISPLACED_TILE_COUNT_CPP
#include "MisplacedTileCount.hpp"

int MisplacedTileCount::evaluate (const Puzzle& puzzle_state){

    int count = 0;
    const Matrix<unsigned char>& board = puzzle_state.getBoard();
    int columns = board.getColumns();
    int rows = board.getRows();

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
            unsigned char current_tile = board.getValueAt(i,j);
            if(current_tile != 0 && (i != current_tile / columns || j != current_tile % columns)){
                count++;
            }
        }
    }

    return count;
}

bool MisplacedTileCount::build (const Puzzle& initial_state, const Puzzle& goal_state){ 
    return true;
}

MisplacedTileCount::MisplacedTileCount(){}

MisplacedTileCount::~MisplacedTileCount(){}
#endif