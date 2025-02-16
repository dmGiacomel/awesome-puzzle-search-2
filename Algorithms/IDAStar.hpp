#ifndef IDASTAR_HPP
#define IDASTAR_HPP
#include "SolvingAlgorithm.hpp"
#include "../Heuristics/Heuristics.hpp"
#include "../puzzle_n_minus_1.hpp"
#include "SearchNode.hpp"
#include "set"
#include <limits.h>
#include <chrono>
#include "../Utils/MovementsHandler.hpp"

class IDAStar : public SolvingAlgorithm{
public: 
    
    virtual std::list<moves> solve(const Puzzle& initial_state, 
                                   const Puzzle& goal_state,
                                   Heuristics* heuristic);

    IDAStar();
    virtual ~IDAStar();

private: 
    
    Heuristics* heuristics;
    Puzzle initial_state;
    Puzzle goal_state;
    int global_threshold;

    size_t nodes_expanded;
    size_t summed_heuristics;

    std::list<moves> makeMovesList(const SearchNode& goal_state);
    std::list<moves> driverProcedure();
    std::list<moves> idaStar(const SearchNode& state, int upper_bound);
    std::set<SearchNode> generateSuccessors(const SearchNode& state);

};
#endif