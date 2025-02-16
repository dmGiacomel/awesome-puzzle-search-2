#ifndef A_STAR_CPP
#define A_STAR_CPP
#include "AStar.hpp"
#include <vector>

#define DEBUGGING_MODE_ASTAR___done___

//depois eu vejo isso
//ISSUE -> THIS CONSTRUCTORS ARE DUMB. initials doesn't mean nothing
//'cause the only way to interact wtih the object is through solve() method

AStar::AStar()
    :heuristics(nullptr), initial_state(3,3), goal_state(3,3)
{
}
AStar::~AStar(){}

std::list<moves> AStar::solve(const Puzzle& initial_state, 
                              const Puzzle& goal_state,
                              Heuristics* heuristic)
{
    
    heuristics = heuristic;

    this->initial_state = initial_state;
    this->goal_state = goal_state;

    #ifdef DEBUGGING_MODE_ASTAR
        std::cout << "initial_state: \n";
        initial_state.printBoard();
        std::cout << std::endl;
        std::cout << "goal_state: \n";
        goal_state.printBoard();
        std::cout << std::endl;
    #endif

    //making sure open and closed are empty
    while (!open.empty())
        open.pop();
    closed.clear();

    nodes_expanded = 0; 
    summed_heuristics = 0;

    auto start = std::chrono::high_resolution_clock::now();

    std::list<moves> path = driverProcedure();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << " ";
    std::cout << nodes_expanded << " " ;
    std::cout << double(summed_heuristics)/nodes_expanded;

    return path;
}

//may good help us all if the heuristic is non-consistent
std::list<moves> AStar::driverProcedure(){
    
    //starting the open set with the initial state
    //closed set starts empty (guaranteed by solve())
    double initial_state_heuristics = heuristics->evaluate(initial_state); //initialize estimate
    open.push(SearchNode(nullptr, moves(none), initial_state, initial_state_heuristics, 0.0, initial_state_heuristics));
    //as long as there are frontier nodes

    #ifdef DEBUGGING_MODE_ASTAR
        unsigned long long int iteration(0);
    #endif

    while(!open.empty()){
        #ifdef DEBUGGING_MODE_ASTAR
            std::cout<< "a_star while loop iteration: " << iteration++ << "\n";
        #endif
        
        //done this way so we can retrieve the path easily after the execution
        //avoid dangling pointers - closed will take care of memory
        SearchNode node = std::move(open.top());
        open.pop();                  
        const SearchNode& current = *closed.emplace(std::move(node)).first;  

        if(current.state.isSolved()) {
            //std::cout << "done!! :DD\n";
            return makeMovesList(current);                      //goal found, return solution
        }else{
            std::set<SearchNode> successors = generateSuccessors(current);

            for (auto& i : successors){
                improve(i);
            }
        }
    }

    #ifdef DEBUGGING_MODE_ASTAR
        std::cout << "no solution at all\n";
    #endif
    return std::list<moves>();              //no solution exists
}

//no duplicate detection 
//may good help us all if the heuristic is non-consistent
void AStar::improve(const SearchNode& state){
    auto state_in_closed = closed.find(state);
    if(state_in_closed != closed.end()){
        if(state.g < state_in_closed->g){
            open.emplace(*state_in_closed);
            closed.extract(state_in_closed);
        }
    }else{ //with consistent heuristics it always falls here

        #ifdef DEBUGGING_MODE_ASTAR
            std::cout << "improve method - state is new!\n";
        #endif

        open.emplace(state);
    }
}

std::set<SearchNode> AStar::generateSuccessors(const SearchNode& node){
    
    #ifdef DEBUGGING_MODE_ASTAR
        std::cout << "inside generate successor function\n"; 
    #endif
    std::set<SearchNode> successors;
    std::set<moves> available_moves = node.state.availableMoves();

    #ifdef DEBUGGING_MODE_ASTAR
        std::cout << "available moves: ";
        for(auto i: available_moves){
            std::cout << +i << " ";
        }
        std::cout << "\n";
    #endif

    double g = node.g + 1;  //unit cost abreviation for w(parent, node)
    for (auto i: available_moves){        
        Puzzle temp = node.state;
        temp.makeMove(i);
        double h = heuristics->evaluate(temp); 

        #ifdef DEBUGGING_MODE_ASTAR
            bool inserted = 
        #endif
        
        successors.insert(SearchNode(const_cast<SearchNode*>(&node), i, temp, h + g, g, h)).second;

        #ifdef DEBUGGING_MODE_ASTAR
            std::cout << "New node inserted?:" << inserted << "\n"; 
        #endif
    }

    #ifdef DEBUGGING_MODE_ASTAR
        std::cout << "number of states in successors: " << successors.size() << "\n";
        std::cout << "successors found: \n";              
        for (auto i : successors){
            i.state.printBoard();
            std::cout << "\n";
        }
    #endif

    #ifdef DEBUGGING_MODE_ASTAR
        std::cout << "end of generate successors\n"; 
    #endif
    
    nodes_expanded += successors.size();
    for(auto &i : successors){
        summed_heuristics += i.h;
    }
    return successors;
}

std::list<moves> AStar::makeMovesList(const SearchNode& goal_state){

    std::list<moves> path;
    
    SearchNode aux_node = goal_state;
    while(aux_node.parent != nullptr){
        path.push_back(aux_node.generated_by);
        aux_node = *aux_node.parent; 
    }

    path.reverse();
    return path;
}

bool AStar::GreaterF::operator()(const SearchNode& s1, const SearchNode& s2) const{
    return s1.f > s2.f;
}

//ARRUMAR DEPOISSSSSSSSSSSSSSSSS
size_t AStar::HashSearchNode::operator()(const SearchNode& s) const{

    std::vector<unsigned char> state = s.state.getPuzzleAsString();

    std::hash<unsigned char> hasher;
    size_t hash = 0;

    for (unsigned char i : state) {
        hash ^= hasher(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }

    return hash;
}

bool AStar::EqualsSearchNode::operator()(const SearchNode& s1, const SearchNode& s2) const{
    return s1.state == s2.state;
}



#endif