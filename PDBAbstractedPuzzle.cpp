#ifndef PDB_ABSTRACTED_PUZZLE_CPP
#define PDB_ABSTRACTED_PUZZLE_CPP
#include "PDBAbstractedPuzzle.hpp"


//the class object can be reused in order to avoid Puzzle object allocations
PDBAbstraction::PDBAbstraction (const std::vector<unsigned char>& initial_tile_permutation,
                                const std::vector<unsigned char>& initial_tile_locations,
                                int puzzle_rows, int puzzle_columns)
    :Puzzle(puzzle_rows,puzzle_columns)
{
    setBoard(initial_tile_permutation, initial_tile_locations);
}

PDBAbstraction::PDBAbstraction (const Puzzle& p, const std::vector<unsigned char>& pdb_tiles)
    :Puzzle(p)
{

    size_t pdb_tiles_size = pdb_tiles.size();

    std::vector<unsigned char> tile_locations(pdb_tiles_size + 1); //0 is no counted on pdb_tiles 
    std::vector<unsigned char> dual_state(IndexingFunctions::getDual(p.getPuzzleAsString()));

    tile_locations[0] = dual_state[0];
    for(size_t i = 0; i < pdb_tiles_size; i++){
        tile_locations[i + 1] = dual_state[pdb_tiles[i]];
    }

    auto permutation = IndexingFunctions::sortPermutation(tile_locations);
    
    tile_locations = IndexingFunctions::applyPermutation(tile_locations, permutation);


    setBoard(permutation, tile_locations);

}


PDBAbstraction::PDBAbstraction(const PDBAbstraction& other)
    :Puzzle(other), tile_locations(other.tile_locations), tile_permutation(other.tile_permutation),
    tile_permutation_dual(other.tile_permutation_dual) 
{
    
}

PDBAbstraction::PDBAbstraction(PDBAbstraction&& other) noexcept
    :Puzzle(std::move(other)), tile_locations(std::move(other.tile_locations)), tile_permutation(std::move(other.tile_permutation)),
    tile_permutation_dual(std::move(other.tile_permutation_dual))
{

}

//copy assignement operator
PDBAbstraction& PDBAbstraction::operator=(const PDBAbstraction& other){
    
    if(this != &other){
        Puzzle::operator=(other);  
        tile_locations = other.tile_locations;
        tile_permutation = other.tile_permutation;
        tile_permutation_dual = other.tile_permutation_dual;
    }

    return *this;
}

//move assignement operator
PDBAbstraction& PDBAbstraction::operator=(PDBAbstraction&& other) noexcept{

    if(this != &other){
        Puzzle::operator=(std::move(other));
        tile_locations = std::move(other.tile_locations);
        tile_permutation = std::move(other.tile_permutation);
        tile_permutation_dual = std::move(other.tile_permutation_dual);
    }

    return *this;
}
PDBAbstraction::~PDBAbstraction(){}

void PDBAbstraction::setBoard(const std::vector<unsigned char>& initial_tile_permutation,
                              const std::vector<unsigned char>& initial_tile_locations){
     

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

    //not checking for errors at all. use it perfectly under the proper conditions
    auto position_of_empty_in_tile_vector = tile_locations[tile_permutation_dual[0]];

    position_of_empty = std::make_tuple((unsigned char)(position_of_empty_in_tile_vector / puzzle_columns), 
                                        (unsigned char)(position_of_empty_in_tile_vector % puzzle_columns));
}

std::vector<unsigned char> PDBAbstraction::getAbstractVector(int puzzle_size) const{
    std::vector<unsigned char> abstracted_board(puzzle_size);

    for(int i = 0; i < puzzle_size; i++){
        abstracted_board[i] = ABSTRACTED_TILE;
    }

    int tile_perm_index = 0;
    for(auto i: tile_locations){
        abstracted_board[i] = tile_permutation[tile_perm_index++];
    }

    return abstracted_board;
}

const std::vector<unsigned char>& PDBAbstraction::getPermutation() const{
    return tile_permutation;
}

const std::vector<unsigned char>& PDBAbstraction::getLocations() const{
    return tile_locations;
}


void PDBAbstraction::printAbstraction(){

    auto rows = board.getRows();
    auto columns = board.getColumns();
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            if (board.getValueAt(i,j) == ABSTRACTED_TILE){
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
bool PDBAbstraction::makeMove(moves move){

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

        if (swaped_tile != ABSTRACTED_TILE){    //combination is the same

            std::swap(tile_permutation[tile_permutation_dual[0]],
                      tile_permutation[tile_permutation_dual[swaped_tile]]);

            std::swap(tile_permutation_dual[0], tile_permutation_dual[swaped_tile]);

        }else{                                  //combination changed

            //updating and sorting tile locations with new location of blank tile
            //if there were time to do it, changing the locations to a std::set would suit better
            //because of red black tree properties. no sorting would be needed. the board sizes and PDB
            //wont be very large anyways.
            tile_locations[tile_permutation_dual[0]] = new_zero_tile_vector_position;
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

#endif