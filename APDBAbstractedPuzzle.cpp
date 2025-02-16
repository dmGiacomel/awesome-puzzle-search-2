#ifndef APDB_ABSTRACTED_PUZZLE_CPP
#define APDB_ABSTRACTED_PUZZLE_CPP 
#include "APDBAbstractedPuzzle.hpp"

bool APDBAbstraction::operator==(const APDBAbstraction& other) const{
    return (
        other.tile_permutation == this->tile_permutation &&
        other.tile_locations == this->tile_locations && 
        this->position_of_empty == other.position_of_empty
        // zero_map.getZeroRegion(this->tile_locations, this->position_of_empty) == zero_map.getZeroRegion(other.tile_locations, other.position_of_empty)
    );
}

bool APDBAbstraction::operator!=(const APDBAbstraction& other) const{
    return !(*this == other);
}

size_t APDBAbstraction::Hash::operator()(const APDBAbstraction &other) const{
    return IndexingFunctions::toCombinadicBase(other.getLocations());
}

size_t APDBAbstraction::Equals::operator()(const APDBAbstraction &apdb1, const APDBAbstraction &apdb2) const{
    return (
        apdb1.tile_permutation == apdb2.tile_permutation &&
        apdb1.tile_locations == apdb2.tile_locations && 
        // this->position_of_empty == other.position_of_empty
        apdb1.zero_map.getZeroRegion(apdb1.tile_locations, apdb1.position_of_empty) == apdb1.zero_map.getZeroRegion(apdb2.tile_locations, apdb2.position_of_empty)
    );
}

//the class object can be reused in order to avoid Puzzle object allocations
APDBAbstraction::APDBAbstraction (const std::vector<unsigned char>& initial_tile_permutation,
                                  const std::vector<unsigned char>& initial_tile_locations,
                                  int puzzle_rows, int puzzle_columns, std::tuple<unsigned char, unsigned char> zero_position)
    :Puzzle(puzzle_rows,puzzle_columns), zero_map(puzzle_rows, puzzle_columns, initial_tile_locations.size())
{
    setBoard(initial_tile_permutation, initial_tile_locations, zero_position);
}

APDBAbstraction::APDBAbstraction (const std::vector<unsigned char>& initial_tile_permutation,
                    const std::vector<unsigned char>& initial_tile_locations,
                    int puzzle_rows, int puzzle_columns, unsigned char zero_tile_region)
    :Puzzle(puzzle_rows,puzzle_columns), zero_map(puzzle_rows, puzzle_columns, initial_tile_locations.size())
{
    auto zero_tile_candidate{*zero_map.getTilesOfRegion(initial_tile_locations, zero_tile_region).begin()};
    setBoard(initial_tile_permutation, initial_tile_locations, std::make_tuple(zero_tile_candidate / puzzle_columns, zero_tile_candidate % puzzle_columns));
}

APDBAbstraction::APDBAbstraction (const Puzzle& p, const std::vector<unsigned char>& pdb_tiles)
    :Puzzle(p), zero_map(p.getBoard().getRows(), p.getBoard().getColumns(), pdb_tiles.size())
{

    size_t pdb_tiles_size = pdb_tiles.size();
    std::vector<unsigned char> tile_locations(pdb_tiles_size); //0 is no counted on pdb_tiles 
    std::vector<unsigned char> dual_state(IndexingFunctions::getDual(p.getPuzzleAsString()));

    for(size_t i = 0; i < pdb_tiles_size; i++){
        tile_locations[i] = dual_state[pdb_tiles[i]];
    }

    auto permutation = IndexingFunctions::sortPermutation(tile_locations);
    
    tile_locations = IndexingFunctions::applyPermutation(tile_locations, permutation);

    setBoard(permutation, tile_locations, position_of_empty);
}


APDBAbstraction::APDBAbstraction(const APDBAbstraction& other)
    :Puzzle(other), tile_locations(other.tile_locations), tile_permutation(other.tile_permutation),
    tile_permutation_dual(other.tile_permutation_dual), zero_map(other.getBoard().getRows(), other.getBoard().getColumns(), other.tile_locations.size())
{
    
}

APDBAbstraction::APDBAbstraction(APDBAbstraction&& other) noexcept
    :zero_map(other.getBoard().getRows(), other.getBoard().getColumns(), other.tile_locations.size()), Puzzle(std::move(other)), tile_locations(std::move(other.tile_locations)), tile_permutation(std::move(other.tile_permutation)),
    tile_permutation_dual(std::move(other.tile_permutation_dual))
{

}

//copy assignement operator
APDBAbstraction& APDBAbstraction::operator=(const APDBAbstraction& other){
    
    if(this != &other){
        Puzzle::operator=(other);  
        tile_locations = other.tile_locations;
        tile_permutation = other.tile_permutation;
        tile_permutation_dual = other.tile_permutation_dual;
    }

    return *this;
}

//move assignement operator
APDBAbstraction& APDBAbstraction::operator=(APDBAbstraction&& other) noexcept{

    if(this != &other){
        Puzzle::operator=(std::move(other));
        tile_locations = std::move(other.tile_locations);
        tile_permutation = std::move(other.tile_permutation);
        tile_permutation_dual = std::move(other.tile_permutation_dual);
    }

    return *this;
}

APDBAbstraction::~APDBAbstraction(){}

void APDBAbstraction::setBoard(const std::vector<unsigned char>& initial_tile_permutation,
                              const std::vector<unsigned char>& initial_tile_locations, std::tuple<unsigned char, unsigned char> zero_tile_location){
    
    position_of_empty = zero_tile_location;

    this->tile_locations = initial_tile_locations;

    //just to be sure about it
    std::sort(tile_locations.begin(), tile_locations.end());
    this->tile_permutation = initial_tile_permutation;
    tile_permutation_dual = IndexingFunctions::getDual(initial_tile_permutation);

    auto puzzle_rows = this->board.getRows();
    auto puzzle_columns = this->board.getColumns();

    //setting the tile vector to properly allocate a puzzle
    std::vector<unsigned char> tile_vector(getAbstractVector(puzzle_rows * puzzle_columns));

    //changing superclass atributes from within the derived class in order to properly set
    //Puzzle fields. This seems to be a not so ellegant solution, but will do for now without messing with
    //the current code structure (no time for testing for now, also)
    auto tile_vector_iterator = tile_vector.cbegin();
    for (int i = 0; i < puzzle_rows; i++){
        for (int j = 0; j < puzzle_columns; j++){
            board.setValueAt(i, j, *tile_vector_iterator);
            tile_vector_iterator++;
        }
    }    
}

std::vector<unsigned char> APDBAbstraction::getAbstractVector(int puzzle_size) const{
    std::vector<unsigned char> abstracted_board(puzzle_size);

    for(int i = 0; i < puzzle_size; i++){
        abstracted_board[i] = ABSTRACTED_TILE;
    }

    int tile_perm_index = 0;
    for(auto i: tile_locations){
        abstracted_board[i] = tile_permutation[tile_perm_index++];
    }

    abstracted_board[std::get<0>(position_of_empty) * board.getColumns() + std::get<1>(position_of_empty)] = 0;

    return abstracted_board;
}

const std::vector<unsigned char>& APDBAbstraction::getPermutation() const{
    return tile_permutation;
}

const std::vector<unsigned char>& APDBAbstraction::getLocations() const{
    return tile_locations;
}

void APDBAbstraction::printAbstraction(){

    const auto zero_tile_region = zero_map.getZeroRegion(tile_locations, position_of_empty);
    const auto &zero_region_tile_set = zero_map.getTilesOfRegion(tile_locations, zero_tile_region);

    auto rows = board.getRows();
    auto columns = board.getColumns();
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            //if tile is in zero_tile region
            if (zero_region_tile_set.find(i * columns + j) != zero_region_tile_set.cend()){
                std::cout << "z\t";
            }
            else if (board.getValueAt(i,j) == ABSTRACTED_TILE){
                std::cout << "x\t";
            }else{
                std::cout << +board.getValueAt(i,j) << "\t";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "Permutation:\t";

    for (auto i: tile_permutation){
        std::cout << +i << "\t";
    }
    std::cout << "\n";

    std::cout << "Locations:\t";

    for (auto i: tile_locations){
        std::cout << +i << "\t";
    }
    std::cout << "\n";

    std::cout << "Permutation Dual:\t";

    for (auto i: tile_permutation_dual){
        std::cout << +i << "\t";
    }
    std::cout << "\n";
}

//this code is somewhat obscure, it surely has a lot of room for improvement
bool APDBAbstraction::makeMove(moves move){

    auto old_zero_tile_row = std::get<0>(position_of_empty);
    auto old_zero_tile_column = std::get<1>(position_of_empty);
    bool move_succesful = this->Puzzle::makeMove(move);

    if (move_succesful){
        //get tile that was swaped with zero
        auto swaped_tile = board.getValueAt(old_zero_tile_row, old_zero_tile_column);
        
        auto new_zero_tile_row = std::get<0>(position_of_empty);
        auto new_zero_tile_column = std::get<1>(position_of_empty);
        auto puzzle_columns = this->board.getColumns();

        auto new_moved_tile_vector_position = old_zero_tile_row * puzzle_columns + old_zero_tile_column;
        auto new_zero_tile_vector_position = new_zero_tile_row * puzzle_columns + new_zero_tile_column;

        // if zero changed with abstracted tile. permutation is the same, locations are the same, nothing changed.
        //zero tile region would be the same too  

        //combination changed
        //permutation might have changed too
        if (swaped_tile != ABSTRACTED_TILE){                                  
            
            tile_locations[tile_permutation_dual[swaped_tile]] = new_moved_tile_vector_position;
            std::sort(tile_locations.begin(), tile_locations.end());

            //updating permutation according to new locations
            //doing it in the old fashion way for the sake of clarity
            int tile_permutation_size = tile_permutation.size();
            for(int i{0}; i < tile_permutation_size; i++){
                tile_permutation[i] = board.getValueAt(tile_locations[i] / puzzle_columns, 
                                                       tile_locations[i] % puzzle_columns);
                tile_permutation_dual[tile_permutation[i]] = i;
            }
        }

        return true;
    }else{
        return false;
    }
}

std::unordered_set<APDBAbstraction, APDBAbstraction::Hash, APDBAbstraction::Equals> APDBAbstraction::neighbourhood(){

    std::unordered_set<APDBAbstraction, APDBAbstraction::Hash> visited;
    std::unordered_set<APDBAbstraction, APDBAbstraction::Hash, APDBAbstraction::Equals> transitive_hull;
    std::queue<APDBAbstraction> open;

    open.push(*this);

    //whatever
    APDBAbstraction current(*this);

    while (!open.empty()){

        current = open.front();
        open.pop();
 
        if(!(APDBAbstraction::Equals()(current, (*this)))){
            transitive_hull.insert(current);
            visited.insert(current);
            continue;
        }

        //generate neighbours
        for (auto i : {right, up, left, down}){
            if(current.makeMove(i)){
                if(visited.find(current) == visited.end()){
                    open.push(current);
                }
                current.makeMove(MovementsHandler::getOpposite(i));
            }
        }

        visited.insert(current);
    }

    return transitive_hull;

}
#endif