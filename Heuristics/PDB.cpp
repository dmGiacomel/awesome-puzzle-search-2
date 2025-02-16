#ifndef PDB_CPP
#define PDB_CPP
#include "PDB.hpp"

int PDB::evaluate (const Puzzle& puzzle_state){
    return patternValueAt(getIndexes(PDBAbstraction(puzzle_state, pdb_tiles)));
}

//shame on me, it will not be fixed though
bool PDB::build (const Puzzle& initial_state, const Puzzle& goal_state){
    return true;
}

std::tuple<size_t, size_t> PDB::getIndexes(const PDBAbstraction& abs){
    return std::make_tuple(IndexingFunctions::toCombinadicBase(abs.getLocations()),
                           IndexingFunctions::rank(abs.getPermutation()));
}

unsigned char PDB::patternValueAt(const std::tuple<size_t, size_t>& indexes){
    return pattern_values[std::get<0>(indexes)][std::get<1>(indexes)];
}

unsigned char PDB::patternValueAt(size_t locations, size_t permutation){
    return pattern_values[locations][permutation];
}

PDBAbstraction PDB::unindex (size_t locations, size_t permutation){
    return PDBAbstraction(IndexingFunctions::unrank(permutation, n_pdb_tiles), 
                          IndexingFunctions::combinationFromRank(locations, n_pdb_tiles),
                          rows, columns);
}

PDBAbstraction PDB::unindex (const std::tuple<size_t, size_t>& indexes){
    return PDBAbstraction(IndexingFunctions::unrank(std::get<0>(indexes), n_pdb_tiles), 
                          IndexingFunctions::combinationFromRank(std::get<1>(indexes), n_pdb_tiles),
                          rows, columns);
}

void PDB::setPatternValueAt(const std::tuple<size_t, size_t>& indexes, unsigned char value){
    pattern_values[std::get<0>(indexes)][std::get<1>(indexes)] = value;
}

void PDB::setPatternValueAt(size_t locations, size_t permutation, unsigned char value){
    pattern_values[locations][permutation] = value;
}

void PDB::fillPatternArray() {
    PDBAbstraction goal_state = PDBAbstraction(Puzzle(rows, columns), pdb_tiles);
    setPatternValueAt(getIndexes(goal_state), 0);

    unsigned char current_level = 0;
    size_t entries_filled = 1;

    while (entries_filled > 0) {
        // std::cout << "Current Level: " << +current_level << std::endl;
        size_t local_entries_filled = 0;

        omp_set_num_threads(omp_get_max_threads());
        #pragma omp parallel
        {
            size_t private_entries_filled = 0;

            #pragma omp for schedule(dynamic, 15)
            for (size_t locations = 0; locations < total_tile_locations; locations++) {
                for (size_t permutation = 0; permutation < total_tile_permutations; permutation++) {

                    if (patternValueAt(locations, permutation) == current_level) {
                        PDBAbstraction current(unindex(locations, permutation));
                        private_entries_filled += expandAndUpdate(current, current_level);
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
size_t PDB::expandAndUpdate(PDBAbstraction& p, unsigned char current_level){
    size_t updated{0};

    for (auto i : {right, up, left, down}){
        if(p.makeMove(i)){
            auto indexes = getIndexes(p);
            if (patternValueAt(indexes) == INFINITY){
                setPatternValueAt(indexes, current_level + 1);
                updated++;
            }
            p.makeMove(MovementsHandler::getOpposite(i));
        }
    }

    return updated;
}

void PDB::shapePatternArray(){
    
    n_pdb_tiles = pdb_tiles.size() + 1;//+1 because pdb_tiles disregard zero;
    total_tile_locations  = IndexingFunctions::binomialCoef(rows * columns, n_pdb_tiles); 
    total_tile_permutations = IndexingFunctions::factorial(n_pdb_tiles);

    pattern_values = std::vector<std::vector<unsigned char>>(
         total_tile_locations,
         std::vector<unsigned char>(total_tile_permutations, INFINITY)
    );
}

size_t PDB::verify(){
    size_t slots_not_reached = 0;
    for (auto &i : pattern_values){
        for (auto j : i){
            if(j == UCHAR_MAX){
                slots_not_reached++;        
            }
        }
    }

    return slots_not_reached;
}


void PDB::histogram(){
    
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
bool PDB::build (const Puzzle& initial_state, const Puzzle& goal_state, const std::vector<unsigned char>& pdb_tiles){
    rows = initial_state.getBoard().getRows();
    columns = initial_state.getBoard().getColumns();

    //pdb tiles should disregard zero
    this->pdb_tiles = pdb_tiles;
    std::sort(this->pdb_tiles.begin(), this->pdb_tiles.end());

    shapePatternArray();
    fillPatternArray();
    //histogram();

    return true;
}

PDB::PDB(){

}

PDB::~PDB(){

}
#endif