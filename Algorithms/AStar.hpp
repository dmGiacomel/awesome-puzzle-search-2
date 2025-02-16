#ifndef A_STAR_HPP
#define A_STAR_HPP
#include "../puzzle_n_minus_1.hpp"
#include "SolvingAlgorithm.hpp"
#include "../Heuristics/Heuristics.hpp"
#include "SearchNode.hpp"
#include <queue>
#include <list>
#include <set>
#include <chrono>

//ISSUE -> DEFAULT CONSTRUCTOR IS CODED IN A VERY DUMB MANNER

class AStar : SolvingAlgorithm{   

public:
    virtual std::list<moves> solve(const Puzzle& initial_state, 
                           const Puzzle& goal_state,
                           Heuristics* heuristic);

    AStar();
    virtual ~AStar();
private:
    Heuristics* heuristics;
    Puzzle initial_state;
    Puzzle goal_state;
    
    size_t nodes_expanded;
    size_t summed_heuristics;
    

    //open and closed set - functor needed on priority queue to create min heap
    struct GreaterF{
        bool operator()(const SearchNode& s1, const SearchNode& s2) const;
    };
    std::priority_queue<SearchNode, std::vector<SearchNode>, GreaterF> open;

    struct HashSearchNode{
        size_t operator()(const SearchNode& s1) const;
    };
    struct EqualsSearchNode{
        bool operator()(const SearchNode& s1, const SearchNode& s2) const;
    };
    
    std::unordered_set<SearchNode, HashSearchNode, EqualsSearchNode> closed;

    std::list<moves> makeMovesList(const SearchNode& goal_state); 
    std::list<moves> driverProcedure();
    void improve(const SearchNode& state);
    std::set<SearchNode> generateSuccessors(const SearchNode& state);

};

#endif