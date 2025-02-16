#ifndef ZERO_TILE_REGION_MAPPING_HPP
#define ZERO_TILE_REGION_MAPPING_HPP
#include "Utils/IndexFunctions.hpp"
#include <iostream>
#include <unordered_set>
#include <queue>
#include <tuple>

const unsigned char OCCUPIED = 255;
const unsigned OPEN = 254;
const unsigned CLOSED = 253;

//this considers that we are working with only one size of puzzle per time. could be templated? absolutely. no time for now.
class ZeroRegionMapping {
public: 

    ZeroRegionMapping(unsigned char puzzle_rows,
                      unsigned char puzzle_columns,
                      unsigned char k_abstraction_tiles);
    
    unsigned char getZeroRegion(const std::vector<unsigned char>& locations, std::tuple<unsigned char, unsigned char> zero_tile_location) const;
    unsigned char getZeroRegion(const std::vector<unsigned char>& locations, unsigned char zero_tile_location) const;
    unsigned char getZeroRegion(size_t locations, std::tuple<unsigned char, unsigned char> zero_tile_location) const;
    unsigned char getZeroRegion(size_t locations, unsigned char zero_tile_location) const;

    std::unordered_set<unsigned char> getTilesOfRegion(const std::vector<unsigned char>& locations, unsigned char region) const;
    std::unordered_set<unsigned char> getTilesOfRegion(size_t locations, unsigned char region) const;

    unsigned char getAmountOfRegions (const std::vector<unsigned char>& locations) const;
    unsigned char getAmountOfRegions (size_t locations) const;

    static void fillLookupTables(unsigned char puzzle_rows, unsigned char puzzle_columns); 


private:
    
    unsigned char k_abstraction_tiles;
    unsigned char puzzle_rows;
    unsigned char puzzle_columns;

    //look_up_tables [k_abstraction_tiles][locations][zero_tile_position]
    //done in a sparse way in the name of performance
    //kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk
    static std::vector<std::vector<std::vector<unsigned char>>> look_up_tables;
    static std::vector<std::vector<std::vector<std::unordered_set<unsigned char>>>> zero_tile_sets;

    static void fillLevel(size_t k_level, size_t tile_locations, unsigned char puzzle_rows, unsigned char puzzle_columns);
    static void findAndFillBlankRegions(size_t k_level, size_t tile_locations, std::vector<std::vector<unsigned char>> &board, unsigned char puzzle_rows, unsigned char puzzle_columns);
    static void floodRegion(size_t k_level, size_t tile_locations, std::vector<std::vector<unsigned char>> &board, unsigned char puzzle_rows, unsigned char puzzle_columns, unsigned char i_source, unsigned char j_source, unsigned char current_region);

};

#endif