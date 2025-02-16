#ifndef ZERO_TILE_REGION_MAPPING_CPP
#define ZERO_TILE_REGION_MAPPING_CPP
#include "zero_tile_region_mapping.hpp"


std::vector<std::vector<std::vector<unsigned char>>> ZeroRegionMapping::look_up_tables;
std::vector<std::vector<std::vector<std::unordered_set<unsigned char>>>> ZeroRegionMapping::zero_tile_sets;

unsigned char ZeroRegionMapping::getZeroRegion(const std::vector<unsigned char>& locations, std::tuple<unsigned char, unsigned char> zero_tile_location) const{
    return look_up_tables[locations.size()][IndexingFunctions::toCombinadicBase(locations)][std::get<0>(zero_tile_location) * puzzle_columns + std::get<1>(zero_tile_location)];
}

unsigned char ZeroRegionMapping::getZeroRegion(const std::vector<unsigned char>& locations, unsigned char zero_tile_location) const{
    return look_up_tables[locations.size()][IndexingFunctions::toCombinadicBase(locations)][zero_tile_location];
}

unsigned char ZeroRegionMapping::getZeroRegion(size_t locations, std::tuple<unsigned char, unsigned char> zero_tile_location) const{
    return look_up_tables[k_abstraction_tiles][locations][std::get<0>(zero_tile_location) * puzzle_columns + std::get<1>(zero_tile_location)];
}

unsigned char ZeroRegionMapping::getZeroRegion(size_t locations, unsigned char zero_tile_location) const{
    return look_up_tables[k_abstraction_tiles][locations][zero_tile_location];
}

std::unordered_set<unsigned char> ZeroRegionMapping::getTilesOfRegion(const std::vector<unsigned char>& locations, unsigned char region) const{
    return zero_tile_sets[locations.size()][IndexingFunctions::toCombinadicBase(locations)][region];
}

std::unordered_set<unsigned char> ZeroRegionMapping::getTilesOfRegion(size_t locations, unsigned char region) const{
    return zero_tile_sets[k_abstraction_tiles][locations][region];
}

unsigned char ZeroRegionMapping::getAmountOfRegions (const std::vector<unsigned char>& locations) const{
    return zero_tile_sets[locations.size()][IndexingFunctions::toCombinadicBase(locations)].size();
}

unsigned char ZeroRegionMapping::getAmountOfRegions (size_t locations) const{
    return zero_tile_sets[k_abstraction_tiles][locations].size();
}

ZeroRegionMapping::ZeroRegionMapping(unsigned char puzzle_rows,
                                     unsigned char puzzle_columns,
                                     unsigned char k_abstraction_tiles)
:k_abstraction_tiles(k_abstraction_tiles), puzzle_columns(puzzle_columns), puzzle_rows(puzzle_rows)
{
    //fillLookUpTables might not have been called before object instantiation
    //if already built, it won't do a thing
    fillLookupTables(puzzle_rows, puzzle_columns);
}

void ZeroRegionMapping::floodRegion(size_t k_level, size_t tile_locations, std::vector<std::vector<unsigned char>> &board, unsigned char puzzle_rows, unsigned char puzzle_columns, unsigned char i_source, unsigned char j_source, unsigned char current_region){
    std::queue<std::tuple<unsigned char, unsigned char>> open;
    open.emplace(std::make_tuple(i_source, j_source));

    while (!open.empty()){
        auto current{open.front()};
        open.pop();

        char current_i = std::get<0>(current);
        char current_j = std::get<1>(current);
        
        //valid neighbourhood
        for (auto offset : (const char[]){-1,1}){
            const char possible_i = current_i + offset;
            const char possible_j = current_j + offset;

            //append new neighbourhood if coordinates are bounded within the board dimensions
            if (possible_i >= 0 && possible_i < puzzle_rows){
                if(board[possible_i][current_j] == OPEN){
                    open.emplace(std::make_tuple(possible_i, current_j));
                }
            }
            if (possible_j >= 0 && possible_j < puzzle_columns){
                if(board[current_i][possible_j] == OPEN){
                    open.emplace(std::make_tuple(current_i, possible_j));
                }
            }
        }

        board[current_i][current_j] = current_region;
    }
}

void ZeroRegionMapping::findAndFillBlankRegions(size_t k_level, size_t tile_locations, std::vector<std::vector<unsigned char>> &board, unsigned char puzzle_rows, unsigned char puzzle_columns){

    unsigned char current_region{0};

    for (unsigned char i{0}; i < puzzle_rows; i++){
        for (unsigned char j{0}; j < puzzle_columns; j++){
            
            if(board[i][j] == OPEN){
                floodRegion(k_level, tile_locations, board, puzzle_rows, puzzle_columns, i, j, current_region++);
            }

        }
    }

    //filling zero region sets information
    //i'm so, so sorry for this code part
    zero_tile_sets[k_level][tile_locations].resize(current_region);

    for (unsigned char i{0}; i < current_region; i++){
        for (unsigned char row{0}; row < puzzle_rows; row++){
            for (unsigned char column{0}; column < puzzle_columns; column ++){
                if (board[row][column] != OCCUPIED){
                    zero_tile_sets[k_level][tile_locations][board[row][column]].insert(row * puzzle_columns + column);
                }
            }
        }
    }
}

void ZeroRegionMapping::fillLevel(size_t k_level, size_t tile_locations, unsigned char puzzle_rows, unsigned char puzzle_columns){
    auto puzzle_combination = IndexingFunctions::combinationFromRank(tile_locations, k_level);
    std::vector<std::vector<unsigned char>> board(puzzle_rows, std::vector<unsigned char>(puzzle_columns, OPEN));

    //setting puzzle combination to 2d board 
    for(auto i: puzzle_combination){
        board[i / puzzle_columns][i % puzzle_columns] = OCCUPIED;
    }

    findAndFillBlankRegions(k_level, tile_locations, board, puzzle_rows, puzzle_columns);

    //properly filling llok-up table locations
    unsigned char it{0};
    for (auto &i: board){
        for (auto j: i){
            look_up_tables[k_level][tile_locations][it++] = j;
        }
    }
}

void ZeroRegionMapping::fillLookupTables(unsigned char puzzle_rows, unsigned char puzzle_columns){
    if (look_up_tables.empty()){
        size_t puzzle_size = puzzle_rows * puzzle_columns;

        //there shouldn't be apds with puzzle_size tiles. it shall always disregard zero
        look_up_tables = std::vector<std::vector<std::vector<unsigned char>>>(puzzle_size - 1);
        zero_tile_sets = std::vector<std::vector<std::vector<std::unordered_set<unsigned char>>>>(puzzle_size - 1);

        for(size_t k = 0; k < puzzle_size - 1; k++){
            size_t n_tile_locations = IndexingFunctions::binomialCoef(puzzle_size, k);
            look_up_tables[k] = std::vector<std::vector<unsigned char>>(n_tile_locations, 
                                                                        std::vector<unsigned char> (puzzle_size, OCCUPIED));
            zero_tile_sets[k] = std::vector<std::vector<std::unordered_set<unsigned char>>>(n_tile_locations);
        }

        //edge cases - k = 0 and k = 1 -> only one zero tile region -> the 0th
        //technially wrong, as i'm overwriting the occupied tiles. i don't think it really matters. no time.
        look_up_tables[0][0][0] = 0;
        for (auto &i: look_up_tables[1]){
            //puzzle_size = binomial(puzzle_size, 1)
            for (size_t j = 0; j < puzzle_size; j++){
                i[j] = 0;
            }
        }

        for (size_t k = 2; k < puzzle_size - 1; k++){
            std::cout << "level " << k << std::endl;
            auto n_locations = look_up_tables[k].size();
            for(size_t locations = 0; locations < n_locations; locations++){
                fillLevel(k, locations, puzzle_rows, puzzle_columns);
            }
        }
    }
}

#endif