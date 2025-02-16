#ifndef SOLVING_ALGORITHM_HPP
#define SOLVING_ALGORITHM_HPP
#include <unordered_set>
#include <list>
#include "../Matrix.hpp"
#include "../puzzle_n_minus_1.hpp"
#include "../Heuristics/Heuristics.hpp"

// enum class SolvingAlgorithmState : unsigned char {
//                                                   READY_TO_RUN, //ready to run
//                                                   RUNNING, 
//                                                   DONE //algorithm done 
//                                                  };

class SolvingAlgorithm{
public:

    virtual std::list<moves> solve(const Puzzle& initial_state,
                                   const Puzzle& goal_state,
                                   Heuristics* heuristic = nullptr) = 0;

    //virtual SolvingAlgorithmState getSolvingAlgorithmState() const final;

//protected:

    //SolvingAlgorithmState current_algorithm_state; 

};

// SolvingAlgorithmState SolvingAlgorithm::getSolvingAlgorithmState() const{
//     return current_algorithm_state;
// }

#endif