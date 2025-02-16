#ifndef MOVEMENTS_HANDLER_HPP
#define MOVEMENTS_HANDLER_HPP
#include <string>
#include "../puzzle_n_minus_1.hpp"
#include <list>

namespace MovementsHandler{

    moves getOpposite(moves m);

    std::string getName(moves m);

    std::list<moves> getOpposites(std::list<moves> moves_list);

    std::list<std::string> getNames(std::list<moves> moves_list);

    std::list<std::string> getOppositeNames(std::list<moves> moves_list);
}


#endif