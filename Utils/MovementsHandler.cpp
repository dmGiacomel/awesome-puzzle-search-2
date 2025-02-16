#ifndef MOVEMENTS_HANDLER_CPP
#define MOVEMENTS_HANDLER_CPP
#include "MovementsHandler.hpp"

moves MovementsHandler::getOpposite(moves m){
    switch(m){
        case left: 
            return right;
            break;
        case right:
            return left;
            break;
        case down: 
            return up;
            break;
        case up:
            return down;
            break;
        default: 
            return none;
            break;
    };
}

std::string MovementsHandler::getName(moves m){
    switch(m){
        case left: 
            return "L";
            break;
        case right:
            return "R";
            break;
        case down: 
            return "D";
            break;
        case up:
            return "U";
            break;
        default: 
            return "none";
            break;
    };
}

std::list<moves> MovementsHandler::getOpposites(std::list<moves> moves_list){

    std::list<moves> opposites;

    for (auto i : moves_list){
            opposites.push_back(getOpposite(i));
    }

    return opposites;
}


std::list<std::string> MovementsHandler::getNames(std::list<moves> moves_list){

    std::list<std::string> moves_names;
    for (auto i : moves_list){
  
        moves_names.push_back(getName(i));

    }

    return moves_names;
}

std::list<std::string> MovementsHandler::getOppositeNames(std::list<moves> moves_list){

    return getNames(getOpposites(moves_list));
}

#endif