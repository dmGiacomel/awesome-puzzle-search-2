#ifndef APDBSET_CPP
#define APDBSET_CPP
#include "APDBSet.hpp"

int APDBSet::evaluate (const Puzzle& puzzle_state){
    int total_h = 0;
    for (auto &i : apdb_set){
        total_h += i.evaluate(puzzle_state);
    }
    return total_h;
}

bool APDBSet::build (const Puzzle& initial_state, const Puzzle& goal_state, const std::vector<std::vector<unsigned char>>& disjoint_tile_sets){
    
    this->initial_state = initial_state;
    this->goal_state = goal_state;
    this->apdb_tiles = disjoint_tile_sets;

    for (auto &i: apdb_tiles){
        std::sort(i.begin(), i.end());
    }

    apdb_set = std::vector<APDB> (
        apdb_tiles.size(),
        APDB()
    );

    for (size_t i = 0; i < apdb_tiles.size(); i++){
        apdb_set[i].build(initial_state, goal_state, disjoint_tile_sets[i]);
    }

    return true;
}

APDBSet::APDBSet()
    :initial_state(2,2), goal_state(2,2)
{
}

APDBSet::~APDBSet(){

}

bool APDBSet::build (const Puzzle& initial_state, const Puzzle& goal_state){
    return true;
}

#endif