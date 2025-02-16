#ifndef MANHATTAN_HPP
#define MANHATTAN_HPP
#include "Heuristics.hpp"

class Manhattan : public Heuristics{

public:
    virtual int evaluate (const Puzzle& puzzle_state);

    virtual bool build (const Puzzle& initial_state, const Puzzle& goal_state);

    Manhattan();

    virtual ~Manhattan();

};

#endif