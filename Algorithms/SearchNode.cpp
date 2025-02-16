#ifndef SEARCH_NODE_CPP
#define SEARCH_NODE_CPP
#include "SearchNode.hpp"

//default constructor
    SearchNode::SearchNode(SearchNode* parent, moves generated_by, const Puzzle& state, double f, double g, double h)
        :parent(parent), generated_by(generated_by), state(state), f(f), g(g), h(h)
    {
    }

    //copy constructor
    SearchNode::SearchNode(const SearchNode& other)
        :parent(other.parent), generated_by(other.generated_by), state(other.state), f(other.f), g(other.g), h(other.h)
    {
    }

    //copy assignement operator
    SearchNode& SearchNode::operator= (const SearchNode& other)
    {
        if (this != &other){
            parent = other.parent;
            state = other.state;
            generated_by = other.generated_by;
            f = other.f;
            g = other.g; 
            h = other.h;
        }

        return *this;
    }

    //destructor
    //parent should take care of it's own allocation, since this will mimic linked lists
    SearchNode::~SearchNode(){}

    //whatever i guess? ...
    bool SearchNode::operator<(const SearchNode& other) const{
        return this->state.getPuzzleAsString() < other.state.getPuzzleAsString();
    }

    // bool SearchNode::operator<(const SearchNode& other) const{
    //     return this->f < other.f;
    // }

    bool SearchNode::operator==(const SearchNode& other) const{
        return this->state == other.state;
    }
    
#endif