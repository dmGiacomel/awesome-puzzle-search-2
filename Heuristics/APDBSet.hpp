#ifndef APDBSET_HPP
#define APDBSET_HPP
#include "AdditivePDB.hpp"

class APDBSet : public Heuristics {
public:

    virtual int evaluate (const Puzzle& puzzle_state);

    virtual bool build (const Puzzle& initial_state, const Puzzle& goal_state, const std::vector<std::vector<unsigned char>>& pdb_tiles);

    APDBSet();

    virtual ~APDBSet();

    virtual bool build (const Puzzle& initial_state, const Puzzle& goal_state);

private:

    Puzzle initial_state; 
    Puzzle goal_state;

    std::vector<std::vector<unsigned char>> apdb_tiles;
    std::vector<APDB> apdb_set;
};

#endif