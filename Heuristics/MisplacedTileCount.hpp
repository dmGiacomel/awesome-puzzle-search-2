#ifndef MISPLACE_TILE_COUNT_HPP
#define MISPLACE_TILE_COUNT_HPP
#include "Heuristics.hpp"

class MisplacedTileCount: public Heuristics{
public: 
    virtual int evaluate (const Puzzle& puzzle_state);

    virtual bool build (const Puzzle& initial_state, const Puzzle& goal_state);

    MisplacedTileCount();

    virtual ~MisplacedTileCount();
};

#endif