#ifndef APDB_CPP
#define APDB_CPP
#include "APDB.hpp"

AbstractPuzzle::AbstractPuzzle(const Puzzle& p, const std::vector<unsigned char>& pdb_tiles)
    :Puzzle(p)
{

    std::unordered_set<unsigned char> pdb_tiles_set;
    for (auto i : pdb_tiles){
        pdb_tiles_set.insert(i);
    }

    int columns = this->board.getColumns();
    int rows = this->board.getRows();

    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; i < columns; j++){

            if(pdb_tiles_set.find(board.getValueAt(i,j)) == pdb_tiles_set.end()){
                board.setValueAt(i, j, ABSTRACT_TILE);
            }
        }
    }

}

// troca o zero com o tile da posição especificada se ele for um tile abstraído
bool AbstractPuzzle::swapZeroWithGivenAbstractTile(int row, int column){
    
    if (row != column ){

    }
}

AbstractPuzzle::~AbstractPuzzle(){
}



int APDB::evaluate (const Puzzle& puzzle_state){
    if (*tableLocation(puzzle_state) == UCHAR_MAX){
        //std::cout << "bad value!";
        //puzzle_state.printBoard();
    }
    return (*tableLocation(puzzle_state));
}

bool APDB::build (const Puzzle& initial_state, const Puzzle& goal_state){return true;}

// tile_locations is always in ascending order in relation to tiles
// location tile a - location tile b - location tile c - ... with a = 0 < b < c < ...
std::vector<size_t> APDB::getTileLocations(const Puzzle& p){
    size_t pdb_tiles_size = pdb_tiles.size();

    std::vector<size_t> tile_locations(pdb_tiles_size); //+1 because pdb_tiles disregard zero;
    std::vector<unsigned char> dual_state(IndexingFunctions::getDual(p.getPuzzleAsString()));

    for(size_t i = 0; i < pdb_tiles_size; i++){
        tile_locations[i] = dual_state[pdb_tiles[i]];
    }

    return std::move(tile_locations);
}

std::vector<size_t> APDB::getPdbTileLocations(const Puzzle& p){
    size_t pdb_tiles_size = pdb_tiles.size();

    std::vector<size_t> tile_locations(pdb_tiles_size + 1);
    std::vector<unsigned char> dual_state(IndexingFunctions::getDual(p.getPuzzleAsString()));

    tile_locations[0] = dual_state[0];
    for(size_t i = 0; i < pdb_tiles_size; i++){
        tile_locations[i + 1] = dual_state[pdb_tiles[i]];
    }

    return std::move(tile_locations);

}

//--------- QUE FEIO, QUE FEEEEEIO, nao precisaria se tivesse sido mais esperto no passado -------------------------------------
// std::vector<unsigned char> gambiarra(const std::vector<size_t>& perm){
//     std::vector<unsigned char> vetordotipocerto(perm.size());
//     auto perm_size = perm.size();
//     for (size_t i = 0; i < perm_size; i++){
//         vetordotipocerto[i] = perm[i];
//     }
//     return std::move(vetordotipocerto);
// }
//------------------------------------------------------------------------
// takes care of all the indexation logic
// returns an iterator to the pdb table for a specific state

unsigned char* APDB::tableLocation(const Puzzle& p){
    std::vector<size_t> sorted_tile_locations = getTileLocations(p);
    auto permutation = IndexingFunctions::sortPermutation(sorted_tile_locations);
    std::sort(sorted_tile_locations.begin(), sorted_tile_locations.end());

    auto rank_tile_locations = IndexingFunctions::toCombinadicBase(sorted_tile_locations);
    auto rank_permutation = IndexingFunctions::rank(permutation);

    return (&pattern_values[rank_tile_locations][rank_permutation]);
}
    
std::pair<size_t, size_t> APDB::pdbTableLocation(const Puzzle& p){
    std::vector<size_t> sorted_tile_locations = getPdbTileLocations(p);
    auto permutation = IndexingFunctions::sortPermutation(sorted_tile_locations);
    std::sort(sorted_tile_locations.begin(), sorted_tile_locations.end());

    auto rank_tile_locations = IndexingFunctions::toCombinadicBase(sorted_tile_locations);
    auto rank_permutation = IndexingFunctions::rank(permutation);

    return std::make_pair(rank_tile_locations, rank_permutation);
}

struct CustomCompareTuple{
    bool operator()(const std::tuple<Puzzle, unsigned char>& t1, const std::tuple<Puzzle, unsigned char>& t2){
        return std::get<1>(t1) > std::get<1>(t2);
    }
};

struct hashPuzzle{
    size_t operator()(const Puzzle& p) const{
        std::vector<char> puzzle_string = p.getPuzzleAsSignedString();
        puzzle_string.push_back(0);
        std::string p_str(puzzle_string.begin(), puzzle_string.end());
        return std::hash<std::string>{}(p_str);
    }
};

void APDB::fillPatternArray(){

    //tuple is <state, associated cost>    
    std::priority_queue<std::tuple<Puzzle, unsigned char>, std::vector<std::tuple<Puzzle, unsigned char>>, CustomCompareTuple> open;
    //std::queue<std::tuple<Puzzle, unsigned char>> open;
    Puzzle initial_state(rows, columns);
    *tableLocation(initial_state) = 0;
    open.push(std::make_tuple(initial_state, 0));

    size_t iteracao = 0; 
    while (!open.empty()){
        
        auto current = std::move(open.top());
        open.pop();
        Puzzle& current_puzzle_state = std::get<0>(current);
        const unsigned char current_state_value = std::get<1>(current);
        //std::cout << "current state value : " << +current_state_value << "\n";
        auto current_index = tableLocation(current_puzzle_state);
        
        //std::cout << "Iteração atual: " << iteracao++ << " -----------------------------------";
        //std::cout << "Open size: " << open.size() << "\n";

        std::list<Puzzle> neighbours = transitiveHullOfZeroCostActions(current_puzzle_state);
    
        //closed.insert(std::move(current_puzzle_state));
        auto visited_indexes = pdbTableLocation(current_puzzle_state);
        pdb_bitmap[visited_indexes.first][visited_indexes.second] = true;

        for (Puzzle &i : neighbours){
            
            auto neighbour_index = tableLocation(i);

            visited_indexes = pdbTableLocation(i);
            if (!pdb_bitmap[visited_indexes.first][visited_indexes.second]){

                //if(current_state_value < *neighbour_index){
                
                //if(current_state_value + 1 <  *tableLocation(i))
                    //*neighbour_index = current_state_value + 1;
                    //pen.push(std::make_tuple(std::move(i), current_state_value + 1));
                
                //std::cout << "entrei aqui !!0\n";
                //if it did change state, verify if new state has better cost than older state. if it is the case
                if (current_index != neighbour_index){     
                    //novo estado eh nao inicializado
                    if (*neighbour_index < UCHAR_MAX){
                        *neighbour_index = current_state_value + 1;
                        open.push(std::make_tuple(std::move(i), current_state_value + 1));
                    }
                    // else{          // sanity check - if things are done properly it should never enter this branch
                    //     if(current_state_value + 1 < *neighbour_index){
                    //         *neighbour_index = current_state_value + 1;
                    //         open.push(std::make_tuple(std::move(i), current_state_value + 1));
                    //     }else{
                    //         //open.push(std::make_tuple(std::move(i), current_state_value ));
                    //     }
                    // }
                }else{
                    //open.push(std::make_tuple(std::move(i), current_state_value ));

                }
                
                //}else{
                //    open.push(std::make_tuple(std::move(i), current_state_value+1));
                //}
            }
        }
    }
}


std::list<Puzzle> APDB::transitiveHullOfZeroCostActions(const Puzzle& initial_state ){

    std::list<Puzzle> transitive_hull;
    //Estado inicial
    std::queue<Puzzle> open;
    open.push(initial_state);

    auto hash_puzzle = [&](const Puzzle& p){
        auto zero_position = p.getPositionOfEmpty();
        std::hash<size_t> hasher;
        size_t seed = 0;
        seed ^= hasher(std::get<0>(zero_position)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(std::get<1>(zero_position)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(size_t(tableLocation(p))) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        return seed;
    };

    auto puzzle_equals = [&](const Puzzle& p1, const Puzzle& p2){
        if(p1.getPositionOfEmpty() == p2.getPositionOfEmpty() && tableLocation(p1) == tableLocation(p2)){
            return true;
        } 
        return false;
    };

    std::unordered_set<Puzzle, decltype(hash_puzzle), decltype(puzzle_equals)> local_closed(10, hash_puzzle, puzzle_equals);

    while(!open.empty()){
        Puzzle current = std::move(open.front());
        open.pop();
        local_closed.insert(current);

        std::list<Puzzle> neighbours = expand(current);
        for (auto &i: neighbours){
            
            // se ainda nao gerado
            if (local_closed.find(i) == local_closed.end()){

                // se o apdb não mudou, deve-se inserir esse estado na fila da busca em largura
                // senao, este cara faz parte do fecho e deve ser inserido no conjunto do fecho 
                if (tableLocation(current) == tableLocation(i)){
                    open.push(std::move(i));
                }else{
                    transitive_hull.push_back(i);
                    //adiciona em closed para evitar reexpansao
                    local_closed.insert(i);
                }
            }
        }
    }

    return std::move(transitive_hull);
}



std::list<Puzzle> APDB::expand(const Puzzle& p){
    std::list<Puzzle> neighbours;

    auto available_moves = p.availableMoves();
    for (auto i : available_moves){
        Puzzle temp = p;
        temp.makeMove(i);
        neighbours.push_back(std::move(temp));
    }
    return std::move(neighbours);
}

void APDB::shapePatternArray(){

    size_t tile_combinations = IndexingFunctions::binomialCoef(rows * columns, pdb_tiles.size()); 
    size_t tile_perms_per_combination = IndexingFunctions::factorial(pdb_tiles.size());

    //std::cout << "tile_combinations: " << tile_combinations << "\n";
    //std::cout << "tile_permutations: " << tile_perms_per_combination << "\n";

    pattern_values = std::vector<std::vector<unsigned char>>(
        tile_combinations,
        std::vector<unsigned char>(tile_perms_per_combination, UCHAR_MAX)
    );
}

size_t APDB::verify(){
    size_t slots_not_reached = 0;
    for (auto &i : pattern_values){
        for (auto &j : i){
            if(j == UCHAR_MAX){
                slots_not_reached++;
            }  
        }
    }
    return slots_not_reached;
}

void APDB::shapePdbPatternArray(){

    size_t tile_combinations  = IndexingFunctions::binomialCoef(rows * columns, pdb_tiles.size() + 1); 
    size_t tile_perms_per_combination = IndexingFunctions::factorial(pdb_tiles.size() + 1);

    //std::cout << "tile_combinations: " << tile_combinations << "\n";
    //std::cout << "tile_permutations: " << tile_perms_per_combination << "\n";

    //std::cout << tile_combinations * tile_perms_per_combination << " ";

    pdb_bitmap = std::vector<std::vector<bool>>(
        tile_combinations,
        std::vector<bool>(tile_perms_per_combination, false)
    );
}

void APDB::destroyPdbArray(){
    pdb_bitmap.clear();
}

void APDB::histogram(){
        
        std::map<unsigned char, size_t> hist;

        for (const auto& innerVec : pattern_values) {
            for (unsigned char value : innerVec) {
                hist[value]++;
            }
        }

        // Print the histogram
        std::cout << "Histogram:" << std::endl;
        for (const auto& pair : hist) {
            std::cout << static_cast<int>(pair.first) << ": " << pair.second << std::endl;
        }

    }



//---------------------------- DO NOT PASS 0 AS ARGUMENT INSIDE OF PDB_TILES ------------------
//---------------------------- please ---------------------------------------------------------
bool APDB::build (const Puzzle& initial_state, const Puzzle& goal_state, const std::vector<unsigned char>& pdb_tiles){
    rows = initial_state.getBoard().getRows();
    columns = initial_state.getBoard().getColumns();

    //pdb tiles should disregard zero
    this->pdb_tiles = pdb_tiles;
    std::sort(this->pdb_tiles.begin(), this->pdb_tiles.end());

    shapePatternArray();
    //std::cout << "shaped!" << "\tcombination array size: " << pattern_values.size() << "\tpermutation array size: " << pattern_values[0].size() << std::endl;
    shapePdbPatternArray();
    fillPatternArray();
    //secondProposalfillPatternArray();
    destroyPdbArray();
    //td::cout << "filled!\n";
    //std::cout << "Slots intocados: " << verify() << "\n";
    //std::cin.get();

    return true;
}

//void APDB::secondProposalfillPatternArray(){

//}

std::pair<size_t, size_t> APDB::apdbTableLocation(const Puzzle& p){
    std::vector<size_t> sorted_tile_locations = getTileLocations(p);
    auto permutation = IndexingFunctions::sortPermutation(sorted_tile_locations);
    std::sort(sorted_tile_locations.begin(), sorted_tile_locations.end());

    auto rank_tile_locations = IndexingFunctions::toCombinadicBase(sorted_tile_locations);
    auto rank_permutation = IndexingFunctions::rank(permutation);

    return std::make_pair(rank_tile_locations, rank_permutation);
}

APDB::APDB(){

}

APDB::~APDB(){

}
#endif