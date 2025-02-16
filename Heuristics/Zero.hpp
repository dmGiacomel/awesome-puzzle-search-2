#ifndef ZERO_HPP
#define ZERO_HPP

#include "Heuristics.hpp"

class Zero : public Heuristics{
public:

    virtual int evaluate (const Puzzle& puzzle_state);

    virtual bool build (const Puzzle& initial_state, const Puzzle& goal_state);

    Zero();

    virtual ~Zero();
private: 

};

#endif