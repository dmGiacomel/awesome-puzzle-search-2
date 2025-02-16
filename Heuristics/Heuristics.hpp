#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP
#include "../puzzle_n_minus_1.hpp"
#include "../Matrix.hpp"

// enum class HeuristicState : unsigned char {MISSING_CONFIG, // goal or initial states missing
//                                            READY, // ready to use
//                                            BUILDING, //building pre-configurations
//                                            READY_TO_BUILD //ready to run the build functions
//                                            };

class Heuristics {

public: 
    virtual int evaluate (const Puzzle& puzzle_state) = 0;

    virtual bool build (const Puzzle& initial_state, const Puzzle& goal_state) = 0;

    // virtual HeuristicState getHeuristicState() const final;

    Heuristics();

    virtual ~Heuristics();

//protected:

    // HeuristicState current_heuristic_state;

};

#endif