#ifndef ADDITIVE_PDB_CPP
#define ADDITIVE_PDH_CPP
#include "AdditivePDB.hpp"

const unsigned char INFINITY = UCHAR_MAX;

int APDB::evaluate (const Puzzle& puzzle_state){
    return patternValueAt(getIndexes(APDBAbstraction(puzzle_state, pdb_tiles)));
}

//shame on me, it will not be fixed though
bool APDB::build (const Puzzle& initial_state, const Puzzle& goal_state){
    return true;
}

std::tuple<size_t, size_t> APDB::getIndexes(const APDBAbstraction& abs){
    return std::make_tuple(IndexingFunctions::toCombinadicBase(abs.getLocations()),
                           IndexingFunctions::rank(abs.getPermutation()));
}

unsigned char APDB::patternValueAt(const std::tuple<size_t, size_t>& indexes){
    return pattern_values[std::get<0>(indexes)][std::get<1>(indexes)];
}

unsigned char APDB::patternValueAt(size_t locations, size_t permutation){
    return pattern_values[locations][permutation];
}

APDBAbstraction APDB::unindex (size_t locations, size_t permutation, unsigned char zero_tile_region){
    return APDBAbstraction(IndexingFunctions::unrank(permutation, n_pdb_tiles), 
                          IndexingFunctions::combinationFromRank(locations, n_pdb_tiles),
                          rows, columns, zero_tile_region);
}

APDBAbstraction APDB::unindex (const std::tuple<size_t, size_t, unsigned char>& indexes){
    return APDBAbstraction(IndexingFunctions::unrank(std::get<0>(indexes), n_pdb_tiles), 
                          IndexingFunctions::combinationFromRank(std::get<1>(indexes), n_pdb_tiles),
                          rows, columns, std::get<2>(indexes));
}

void APDB::setPatternValueAt(const std::tuple<size_t, size_t>& indexes, unsigned char value){
    pattern_values[std::get<0>(indexes)][std::get<1>(indexes)] = value;
}

void APDB::setPatternValueAt(size_t locations, size_t permutation, unsigned char value){
    pattern_values[locations][permutation] = value;
}

//in this particular case it will fill the zpdb. After that it will reduce to the apdb
void APDB::fillPatternArray() {
    // ZeroRegionMapping zero_mapping(rows, columns, n_pdb_tiles);

    APDBAbstraction goal_state = APDBAbstraction(Puzzle(rows, columns), pdb_tiles);
    setPatternValueAtZPDB(getIndexesZPDB(goal_state), 0);

    std::cout << "total tile locations: " << total_tile_locations << std::endl;
    std::cout << "total tile permutations: " << total_tile_permutations << std::endl;

    unsigned char current_level = 0;
    size_t entries_filled = 1;

    while (entries_filled > 0) {
        std::cout << "Current Level: " << +current_level << std::endl;
        size_t local_entries_filled = 0;

        omp_set_num_threads(omp_get_max_threads());
        #pragma omp parallel
        {
            size_t private_entries_filled = 0;

            #pragma omp for schedule(dynamic, 15)
            for (size_t locations = 0; locations < total_tile_locations; locations++) {
                for (size_t permutation = 0; permutation < total_tile_permutations; permutation++) {
                    auto n_zero_tile_regions = temp_zpdb[locations][permutation].size();
                    // auto n_zero_tile_regions = zero_mapping.getAmountOfRegions(locations);
                    for(unsigned char zero_tile_region = 0; zero_tile_region < n_zero_tile_regions; zero_tile_region++){
                        // std::cout << "level, locations, permutation, zero_tile_region: " << +current_level << "\t" << locations << "\t" << permutation << "\t" << +zero_tile_region << std::endl; 
                        if (patternValueAtZPDB(locations, permutation, zero_tile_region) == current_level) {
                            // std::cout << "entrando no if" << std::endl;
                            // std::cout << "level, locations, permutation, zero_tile_region: " << +current_level << "\t" << locations << "\t" << permutation << "\t" << +zero_tile_region << std::endl; 
                            APDBAbstraction current(unindexZPDB(locations, permutation, zero_tile_region));
                            private_entries_filled += expandAndUpdate(current, current_level);
                            // local_entries_filled += expandAndUpdate(current, current_level);
                        }
                        // std::cout << "level, locations, permutation, zero_tile_region: " << +current_level << "\t" << locations << "\t" << permutation << "\t" << +zero_tile_region << std::endl; 
                    }
                }
            }

            #pragma omp atomic
            local_entries_filled += private_entries_filled;
        }

        entries_filled = local_entries_filled; 
        current_level++;
    }
}

//all using references and accessing caller's memory in the hope of better performance
//should change neighbours size if needed
size_t APDB::expandAndUpdate(APDBAbstraction& p, unsigned char current_level){
    size_t updated{0};

    auto neighbourhood = p.neighbourhood();
    for (auto &i : neighbourhood){
        auto indexes = getIndexesZPDB(i);
        if (patternValueAtZPDB(indexes) == INFINITY){
            setPatternValueAtZPDB(indexes, current_level + 1);
            updated++;
        }
    }

    return updated;
}

void APDB::shapePatternArray(){
    std::cout << "shaping pattern array" << std::endl;
    n_pdb_tiles = pdb_tiles.size();
    total_tile_locations  = IndexingFunctions::binomialCoef(rows * columns, n_pdb_tiles); 
    total_tile_permutations = IndexingFunctions::factorial(n_pdb_tiles);

    pattern_values = std::vector<std::vector<unsigned char>>(
         total_tile_locations,
         std::vector<unsigned char>(total_tile_permutations, INFINITY)
    );
}

//the correct thing to do would be making a huge block of memory and align pointers. it is not that difficult, but development 
//time is very constrained now.
void APDB::shapeTempZpdb(){

    std::cout << "shaping temp zpd" << std::endl;

    n_pdb_tiles = pdb_tiles.size();
    total_tile_locations  = IndexingFunctions::binomialCoef(rows * columns, n_pdb_tiles); 
    total_tile_permutations = IndexingFunctions::factorial(n_pdb_tiles);

    temp_zpdb = std::vector<std::vector<std::vector<unsigned char>>>(
        total_tile_locations, std::vector<std::vector<unsigned char>> (total_tile_permutations)
    );

    ZeroRegionMapping zero_mapping(rows, columns, n_pdb_tiles);

    for (size_t locations{0}; locations < total_tile_locations; locations++){
        auto n_zero_tile_regions = zero_mapping.getAmountOfRegions(locations);
        for (size_t permutation{0}; permutation < total_tile_permutations; permutation++){
            temp_zpdb[locations][permutation] = std::vector<unsigned char>(n_zero_tile_regions, INFINITY);
        }
    }
}

size_t APDB::verify(){
    size_t slots_not_reached = 0;
    for (auto &i : pattern_values){
        for (auto j : i){
            if(j == UCHAR_MAX){
                slots_not_reached++;        
            }
        }
    }

    std::cout << "slots not reached: " << slots_not_reached << std::endl;
    return slots_not_reached;
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
    shapeTempZpdb();

    std::cout << "everything filled\n";

    fillPatternArray();
    collapseZpdbToApdb();
    clearZpdb();
    histogram();
    verify();

    return true;
}

APDB::APDB(){

}

APDB::~APDB(){

}

void APDB::collapseZpdbToApdb(){
    for (size_t locations{0}; locations < total_tile_locations; locations++){
        for (size_t permutation{0}; permutation < total_tile_permutations; permutation++){
            pattern_values[locations][permutation] = *std::min_element(std::begin(temp_zpdb[locations][permutation]), std::end(temp_zpdb[locations][permutation]));
        }
    }
}

void APDB::clearZpdb(){
    temp_zpdb.clear(); //thank the god of OOP for the destructors
}

std::tuple<size_t, size_t, unsigned char> APDB::getIndexesZPDB(const APDBAbstraction& abs){
    ZeroRegionMapping zero_mapping(rows, columns, abs.getLocations().size());
    auto locations = IndexingFunctions::toCombinadicBase(abs.getLocations());
    return std::make_tuple(
        locations,
        IndexingFunctions::rank(abs.getPermutation()),
        zero_mapping.getZeroRegion(locations, abs.getPositionOfEmpty())
    );
}

APDBAbstraction APDB::unindexZPDB (size_t locations, size_t permutation, unsigned char zero_tile_region){
    return APDBAbstraction(
        // IndexingFunctions::combinationFromRank(locations, n_pdb_tiles),
        IndexingFunctions::unrank(permutation, n_pdb_tiles),
        IndexingFunctions::combinationFromRank(locations, n_pdb_tiles),

        rows, columns, zero_tile_region
    );
} 

APDBAbstraction APDB::unindexZPDB (const std::tuple<size_t, size_t, unsigned char>& indexes){
     return APDBAbstraction(
        // IndexingFunctions::combinationFromRank(std::get<0>(indexes), n_pdb_tiles),
        IndexingFunctions::unrank(std::get<1>(indexes), n_pdb_tiles),
        IndexingFunctions::combinationFromRank(std::get<0>(indexes), n_pdb_tiles),

        rows, columns, std::get<2>(indexes)
    );
}

unsigned char APDB::patternValueAtZPDB(const std::tuple<size_t, size_t, unsigned char>& indexes){
    return temp_zpdb[std::get<0>(indexes)][std::get<1>(indexes)][std::get<2>(indexes)];
}

//nao eh aqui o problema
unsigned char APDB::patternValueAtZPDB(size_t locations, size_t permutation, unsigned char zero_tile_region){

    // std::cout << "davi + ";
    auto desespero_total = temp_zpdb[locations][permutation][zero_tile_region];
    // std::cout << "bruno = <3\n";

    return desespero_total;
}

void APDB::setPatternValueAtZPDB(const std::tuple<size_t, size_t, unsigned char>& indexes, unsigned char value){
    temp_zpdb[std::get<0>(indexes)][std::get<1>(indexes)][std::get<2>(indexes)] = value;
}

void APDB::setPatternValueAtZPDB(size_t locations, size_t permutation, unsigned char zero_tile_location, unsigned char value){
    temp_zpdb[locations][permutation][zero_tile_location] = value;
}

#endif