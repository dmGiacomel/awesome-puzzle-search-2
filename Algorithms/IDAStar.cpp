#ifndef IDASTAR_CPP
#define IDASTAR_CPP
#include "IDAStar.hpp"

//mudar isso depois

static int avaliacoes_inconsistentes = 0;

std::list<moves> IDAStar::solve(const Puzzle& initial_state, 
                                const Puzzle& goal_state,
                                Heuristics* heuristic)
{

    this->heuristics = heuristic;
    this->initial_state = initial_state;
    this->goal_state = goal_state;
    global_threshold = 0;

    nodes_expanded = 0; 
    summed_heuristics = 0;

    auto start = std::chrono::high_resolution_clock::now();

    std::list<moves> path = driverProcedure();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << " ";
    std::cout << nodes_expanded << " ";
    std::cout << double(summed_heuristics)/nodes_expanded;
    return path;
}

//ISSUE -> THIS CONSTRUCTORS ARE DUMB. initials doesn't mean nothing
//'cause the only way to interact wtih the object is through solve() method

IDAStar::IDAStar()
    :heuristics(nullptr), initial_state(3,3), goal_state(3,3)
{
}

IDAStar::~IDAStar(){}

std::list<moves> IDAStar::driverProcedure(){
    
    global_threshold = heuristics->evaluate(initial_state);         //initializing global threshold
    std::list<moves> best_path;                                         //initializing solution path
    double initial_g = 0;
    double initial_h = heuristics->evaluate(initial_state);
        //std::cout << "IDA running...\n ";

    while (best_path.empty() && global_threshold != INT_MAX){
        //std::cout << "Current global threshold: " << global_threshold << "\n";
        int local_threshold = global_threshold;
        global_threshold = INT_MAX;

        best_path = idaStar(SearchNode(nullptr, none, initial_state, initial_g + initial_h, initial_g, initial_h), local_threshold);
    }

    return best_path;
}

// side_effect -> update global threshold
std::list<moves> IDAStar::idaStar(const SearchNode& current, int upper_bound){

    if (current.state.isSolved()){
        //std::cout << "f-value da solucao: " << current.f << "\n";
        //std::cout << "g-value da solucao: " << current.g << "\n";
        //std::cout << "h-value da solucao: " << current.h << "\n";

        //std::cout << "found!\n";
        return makeMovesList(current);
    }

    std::set<SearchNode> successors = generateSuccessors(current);

    for (auto& current_node : successors){
        // verificar se o custo é maior que 
        //leaf node in the search tree

        if (current_node.f > upper_bound){                    //cost exceeds old bound 
            if(current_node.f < global_threshold){            //cost exceeds new bound 

                global_threshold = current_node.f;             //update new bound
            }

        }else{

        // -------------------------------------------
        //if(current_node.f > 22) {
        //    std::cout << "Nó com avaliação inconsistente foi expandido (f, g, h) = : "<< current_node.f << ", " << current_node.g << ", " << current_node.h << "\n";
        //}
        //-----------------------------------------------

        //--------------------------------------------

            std::list<moves> path = idaStar(current_node, upper_bound);
            if(!path.empty()){
                return path;
            }
        }
    }

    return std::list<moves>();
}

std::set<SearchNode> IDAStar::generateSuccessors(const SearchNode& node){

    std::set<SearchNode> successors;
    std::set<moves> available_moves = node.state.availableMoves();

    double g = node.g + 1;  //unit cost abreviation for w(parent, node)
    for (auto i: available_moves){        
        Puzzle temp = node.state;
        temp.makeMove(i);
        double h = heuristics->evaluate(temp); 
        successors.insert(SearchNode(const_cast<SearchNode*>(&node), i, temp, h + g, g, h)).second;
    }
    
    nodes_expanded += successors.size();
    for(auto &i : successors){
        summed_heuristics += i.h;
    }
    return successors;
}

std::list<moves> IDAStar::makeMovesList(const SearchNode& goal_state){

    std::list<moves> path;
    
    SearchNode aux_node = goal_state;
    while(aux_node.parent != nullptr){
        path.push_back(aux_node.generated_by);
        aux_node = *aux_node.parent; 
    }

    path.reverse();
    return path;
}

#endif